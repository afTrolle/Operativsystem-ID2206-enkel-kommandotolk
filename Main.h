/*
 * Main.h
 *
 *  Created on: May 12, 2015
 *      Author: alex
 */

#ifndef MAIN_H_
#define MAIN_H_

void getInput();
void formatInput();
void freeArgs();

void decrementNumArgs();
#ifndef SIGNALDETECTION
#define SIGNALDETECTION 0 /* SIGNALDETECTION == 0 , pooling method*/
#endif /* SIGNALDETECTION */


#endif /* MAIN_H_ */
