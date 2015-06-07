/*
 ============================================================================
 Name        : Main.c
 Author      : Alexander af Trolle
 Version     : 1.0
 Description : Simple Shell Program
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Main.h"
#include "signalHandler.h"
#include "InputHandler.h"

/* Commands won't be more than 70 characters */
#define BUFFERSIZE 70
/* Maximum amount of arguments is 5 */
#define ARGBUFFERSIZE 6

/* Line read from STD-IN  */
char inputBuffer[BUFFERSIZE];
/* Holds formated version of inputBuffer*/
char *shellArgs[ARGBUFFERSIZE];
/* Number of arguments from input */
int argsCounter = 0;

/* Main part of the program, will loop forever until exit command has been issued */
int main(void) {

	initSignalHandlers();

	/* once a command has been performed loop again for more input */
	while (1) {

		/* get STD-IN reading from shell*/
		getInput();

		/* Separate arguments to separate arrays */
		formatInput();

		/* Handle given arguments */
		handelInput(shellArgs,argsCounter);

		/* Check if any background processes has terminated if pooling method is selected*/
		if (SIGNALDETECTION == 0) {
			pollingHandler();
		}

		/* release allocated args memory */
		freeArgs();
	}

	/*never called*/
	return EXIT_SUCCESS;
}

/* read STD-IN for user shell commands */
void getInput() {

	printf("==>>");
	/* read STD-IN */
	if (fgets(inputBuffer, BUFFERSIZE, stdin) == NULL) {
		return;
	}
}

/* Separate the arguments to their on char array
 * Requires that & for background processes is divided by a space-" "*/
void formatInput() {

	char *ptr = inputBuffer;
	char *saveptr = 0;
	char *token;

	/* loops through input and divides the input on spaces  */
	while ((token = strtok_r(ptr, " ", &saveptr)) != NULL) {
		/*allocates memory for input argument */
		shellArgs[argsCounter] = malloc(strlen(token));
		/* copy argument to args array */
		strcpy(shellArgs[argsCounter], token);
		/* move header of input buffer */
		ptr = saveptr;
		/* increase argscounter*/
		argsCounter++;
	}

	/* removes new line from last argument and replace it with \0 empty char */
	int len;
	len = strlen(shellArgs[argsCounter - 1]);
	if (shellArgs[argsCounter - 1][len - 1] == '\n')
		shellArgs[argsCounter - 1][len - 1] = '\0';
}


void freeArgs(){
	//free memory, and reset argscounter so it is ready for input.
	while(argsCounter > 0){
		argsCounter--;
		free (shellArgs[argsCounter]);
	}

}

