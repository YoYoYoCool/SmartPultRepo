/*
 * PultGlobalDefinitions.h
 *
 *  Created on: 13 ����� 2017 �.
 *      Author: Grigoriy
 */

#ifndef PULTGLOBALDEFINITIONS_H_
#define PULTGLOBALDEFINITIONS_H_

//#define PULT_DEVELOPING_BOARD
#define LANDSCAPE       //isDisplayOrientation

//#define Garanin
#define USAEdition
#define joyPult
//#define crane
//#define myPanBar
//#define PanBarDebug
//#define joyPultRussian

#ifdef joyPult
#else
#define WhellSmartPult
#endif
//#define objDef
//��� ��������� � �������� ������
//#define testPult

//��� �������
#define bigWhell
//#define panWheelDebug
//#define rollWheelDebug
//#define tiltWheelDebug
//#define debugWheel

#endif /* PULTGLOBALDEFINITIONS_H_ */






