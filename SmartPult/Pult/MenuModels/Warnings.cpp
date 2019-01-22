/*
 * Warnings.cpp
 *  Created on: 18 но€б. 2015 г.
 *      Author: cno
 */

#include <Pult/MenuModels/Warnings.h>


#define WARNINGS_MESSAGES_COUNT 32

Warning wrn0("Head disconnected!", WT_WARNING),
        wrn1("Pan avs Fault!1"),
        wrn2("Pan avs Fault!2"),
        wrn3("Pan avs Fault!3");


Warnings::Warnings() {
//   allWarningList.add(&wrn0);
//   allWarningList.add(&wrn1);
//   allWarningList.add(&wrn2);
//   allWarningList.add(&wrn3);
   runStrWarningList.addAll(&allWarningList);
}

List<Warning*>* Warnings::getRunStrWarnings() {
	return &runStrWarningList;
}

