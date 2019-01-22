/*
 * Warnings.h
 *
 *  Created on: 18 нояб. 2015 г.
 *      Author: cno
 */

#ifndef PULT_MENUMODELS_WARNINGS_H_
#define PULT_MENUMODELS_WARNINGS_H_

#include <Pult/MenuModels/Warnings.h>
#include <Pult/MenuModels/ViewLists.h>

typedef enum WarningType {
   WT_INFO = 0, //Зеленый
   WT_CRITICAL_INFO,
   WT_WARNING, //Оранжевый
   WT_CRITICAL_WARNING, //Красный
   WT_FAULT
} WarningType;

class Warning {
public:
	Warning(const char* message, WarningType priority):message(message),priority(priority){};
	Warning(const char* message):message(message),priority(WT_FAULT){};
    const char* message;
    const WarningType priority;
};

#define WARNINGS_COUNT 32
#define RUN_STR_WARNINGS_COUNT 4

class Warnings {
private:
	StaticList<Warning*, WARNINGS_COUNT> allWarningList;
	StaticList<Warning*, RUN_STR_WARNINGS_COUNT> runStrWarningList;
public:
	Warnings();
	//Возвращает текущий список аварий, признак конца - пустая строку
	List<Warning*>*  getRunStrWarnings();
};

#endif /* PULT_MENUMODELS_WARNINGS_H_ */
