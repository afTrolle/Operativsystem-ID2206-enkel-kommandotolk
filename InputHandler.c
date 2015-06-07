/*
 * InputHandler.c
 *
 *  Created on: May 12, 2015
 *      Author: alex
 */
#include "InputHandler.h"
#include "Main.h"
#include "signalHandler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

/* function to change working directory */
void changeDir(char * shellArgs[]) {
	/* naive way to set directory by using second argument */
	if (chdir(shellArgs[1]) == -1) {
		/* if chdir fails try HOME environment variable instead */
		perror("error changing directory to set argument");

		char *env = NULL;
		if ((env = getenv("HOME")) != NULL) {

			if (chdir(env) == -1) {
				perror("Error: failed to change working directory to HOME environment variable");
				return;
			}
		} else {
			printf("HOME environment variable isn't set \n");
			return;
		}
	}

	/* prints out the current working directory*/
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		fprintf(stdout, "changed working dir to: %s\n", cwd);
	else {
		perror("can't print working-dir");
	}
}

/* check if any specific input has been made (exit , cd) */
void handelInput(char * shellArgs[], int numArgs) {

	/*user typed exit time  so we exit here*/
	if (strcmp(shellArgs[0], "exit") == 0) {

		printf("Shutting down program \n");
		/*if any child process is running kill it with sigint */
		kill(0, SIGINT);
		/*use pooling handler to remove zombie processes*/
		pollingHandler();
		/* exit */
		exit(0);

	} else if (strcmp(shellArgs[0], "cd") == 0) {
		/*user want's to change working directory*/
		changeDir(shellArgs);
	} else {
		/* no special command has been made use exec command "launch program"*/
		execCommand(shellArgs, numArgs);
	}
}

/* Check launch the program as a foreground process or background process */
void execCommand(char * shellArgs[], int numArgs) {

	/* Check if command is a foreground or background process */
	int ret = strcmp(shellArgs[numArgs - 1], "&");

	if (ret == 0) {
		/* remove & from process call */
		shellArgs[numArgs - 1][0] = '\0';
		startBackGroundProcess(shellArgs);
	} else {
		startForegroundprocess(shellArgs);
	}
}

void startBackGroundProcess(char * shellArgs[]) {

	/* child process id, so we know what's what */
	pid_t backgroundprocess;

	/* fork creates the child process */
	backgroundprocess = fork();

	if (backgroundprocess == 0) {
		/* This code will only be run by child process*/

		/* start function */
		if (execvp(shellArgs[0], shellArgs) == -1) {
			perror("Problem Executing command ");
		}

		/* exec failed so exit child with status code -1  */
		exit(-1);

	} else if (backgroundprocess < 0) {

		perror("couldn't create process");
		exit(-1);

	} else if (backgroundprocess > 0) {

		printf("Spawned backgroundprocess pid: %d  \n", backgroundprocess);

		/* disable ctrl + c for shell until child processes are done */
		setIgnoreSIGINT();
	}
}

/* checks if any child process has ended
 * if child has ended prints pid and exit code
 * also reap the zombie process
 * also if no more childs exists enable exit with Ctrl + C */
void pollingHandler() {

	pid_t processid;
	while ((processid = waitpid((pid_t) (-1), 0, WNOHANG)) > 0) {
		if (processid > 0) {
			/*check status that process child has exited or been killed */
			if (WIFEXITED(processid)) {
				printf("Child:%d exited with RC=%d\n", (int) processid,
						WEXITSTATUS(processid));
			}
			if (WIFSIGNALED(processid)) {
				printf("Child:%d killed via signal %d\n", (int) processid,
						WTERMSIG(processid));
			}
		}
	}

	/* if there are no more child processes available then enable ctrl + c for shell*/
	if (processid == -1) {
		if (errno == ECHILD) {
			removeIgnoreSIGINT();
		}
	}
}

void startForegroundprocess(char * shellArgs[]) {

	/* child process id, so we know what's what */
	pid_t foregroundprocess;
	struct timespec t0;
	struct timespec t1;
	int status;

	/* if we are using signals then block SIGCHLD
	 * while foreground processes is running */
	if (SIGNALDETECTION != 0) {
		startBlockofSIGCHLD();
	}

	clock_gettime(CLOCK_MONOTONIC, &t0);

	/* fork creates the child process */
	foregroundprocess = fork();

	if (foregroundprocess == 0) { /* This code will only be run by child process*/

		/* Run specific child task as child process*/
		/* start function */
		if (execvp(shellArgs[0], shellArgs) == -1) {
			perror("Problem Executing command ");
		}

		/* exec failed so exit child with status code -1  */
		exit(-1);

	} else if (foregroundprocess < 0) {

		perror("couldn't create foreground process");
		exit(-1);

	} else if (foregroundprocess > 0) {
		printf("Spawned foreground process pid: %d  \n", foregroundprocess);

		/* register < Ctrl + C >  signal catcher*/
		setIgnoreSIGINT();

		/*wait for child process to finish */
		waitpid(foregroundprocess, &status, 0);
		clock_gettime(CLOCK_MONOTONIC, &t1);

		/* print execution info */
		printf("Foreground process %d terminated \n", foregroundprocess);
		double elapsedmilisec = ((t1.tv_sec - t0.tv_sec) * 1000)
				+ ((t1.tv_nsec - t0.tv_nsec) / 1.0e6); // Convert nanoseconds to milliseconds
		printf("Execution time: %.3f msec \n", elapsedmilisec);

		/* remove block, will send SIGCHLD signals that we sent during foreground execution */
		if (SIGNALDETECTION != 0) {
			removeBlockofSIGCHILD();
		}
	}
}

