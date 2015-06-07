/*
 * InputHandler.h
 *
 *  Created on: May 12, 2015
 *      Author: alex
 */

#ifndef INPUTHANDLER_H_
#define INPUTHANDLER_H_

void handelInput(char * shellArgs[],int numArgs);
void execCommand(char * shellArgs[], int numArgs);
void startForegroundprocess(char * shellArgs[]);
void startBackGroundProcess(char * shellArgs[]);
void checkBackgroundProcesses();

void initSigHAndler();
void pollingHandler();

	#endif /* INPUTHANDLER_H_ */
