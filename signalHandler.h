/*
 * signalHandler.h
 *
 *  Created on: Jun 6, 2015
 *      Author: alex
 */

#ifndef SIGNALHANDLER_H_
#define SIGNALHANDLER_H_

/* catch signalhandler */
void regSIGINT();
void reg2SIGINT();
void  INThandler(int sig);

void setIgnoreSIGINT();
void removeIgnoreSIGINT();

void initSignalHandlers();
void setChildSigHandler();

void startBlockofSIGCHLD();
void removeBlockofSIGCHILD();

#endif /* SIGNALHANDLER_H_ */
