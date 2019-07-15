/*
 * PultGlobalDefinitions.h
 *
 *  Created on: 13 марта 2017 г.
 *      Author: Grigoriy
 */

#ifndef PULTGLOBALDEFINITIONS_H_
#define PULTGLOBALDEFINITIONS_H_

//#define PULT_DEVELOPING_BOARD
#define LANDSCAPE       //isDisplayOrientation

//#define Garanin
#define USAEdition
#define joyPult
#define PanBarDebug


#ifdef joyPult
#else
#define WhellSmartPult
#endif
//#define objDef
//для настройки и проверки пульта
//#define testPult

//для отладки
//#define panWheelDebug
//#define rollWheelDebug
//#define tiltWheelDebug
//#define debugWheel


#endif /* PULTGLOBALDEFINITIONS_H_ */
