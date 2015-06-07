/*
 * signalHandler.c
 *
 *  Created on: Jun 6, 2015
 *      Author: alex
 */

#include  <stdio.h>
#include  <signal.h>
#include "Main.h"
#include "InputHandler.h"
#include "signalHandler.h"

void initblock();

/******************* SIGINT handling < Ctrl + C > **********************/

/* This can only be called when at least one childprocesses is running
 * and should kill all the children so we enable ctrl c for shell program! */
void sigINTFunction(int sig){
	if (sig == SIGINT){
		signal(SIGINT,SIG_DFL);
	}
}

/* ignore SIGINT signals */
void setIgnoreSIGINT(){

	struct sigaction temp;
	/*set Inthandler function to handle SIGINT Signals*/
	temp.sa_handler = sigINTFunction;
    /* Restart the system call, if at all possible.
     * waiting for input for example */
	temp.sa_flags = SA_RESTART;

   if( sigaction(SIGINT, &temp, NULL) != 0 ){
	   perror("can't register SIGINT signal listener");
   }
}
/* set SIGINTs default signalhandler to handle sigint calls */
void removeIgnoreSIGINT(){
	signal(SIGINT,SIG_DFL);
}


/******************* SIGCHLD handling **********************/

/*starts SIGCHLD handling when signal method,
 *  handles background processes termination */
void initSignalHandlers(){
	/* check that we are using signal method for detecting terminated children */
	if (SIGNALDETECTION != 0){
		initblock();
		setChildSigHandler();
	}
}


/* SIGCHLD Handler, checks if any children has terminated,
 * and prints that childs info*/
void SigChildHandler(int sig){
	if (sig == SIGCHLD){
		/* will require same functionality as looping method when gotten a signal
		 * so can use the same function can be used. */
		pollingHandler();
	}
}

sigset_t x;
/* setup a SIGCHLD signal */
void initblock(){
	sigemptyset(&x);
	sigaddset(&x,SIGCHLD);
}

/* start blocking the SIGCHLD Signals */
void startBlockofSIGCHLD(){
	sigprocmask(SIG_BLOCK,&x,NULL);
}

/* stop blocking the SIGCHLD Signals,
 * will send signals that was blocked during the run */
void removeBlockofSIGCHILD(){
	sigprocmask(SIG_UNBLOCK,&x,NULL);
}

/*setup */
void setChildSigHandler(){

	struct sigaction act;
	/*set SigChildHandler function to handle SIGCHLD Signals */
	    act.sa_handler = SigChildHandler;
	    /* Restart the system call, if at all possible.
	     * waiting for input for example */
	    act.sa_flags = SA_RESTART;
	    act.sa_mask = x;

	   if( sigaction(SIGCHLD, &act, NULL) != 0 ){
		   perror("can't register SIGCHLD signal listener");
	   }
}




