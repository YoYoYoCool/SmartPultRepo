/*
 * WeelCorrectionRefPoints.cpp
 *
 *  Created on: 04 февр. 2016 г.
 *      Author: AhrameshinGN
 */

#include "WeelCorrectionRefPoints.hpp"

Point refPoints[10];


WeelCorrectionPoints::WeelCorrectionPoints()
{
	refPoints[0].x=0;		refPoints[0].y=0.99;
	refPoints[1].x=232;		refPoints[1].y=0.89;
	refPoints[2].x=464;		refPoints[2].y=0.7;
	refPoints[3].x=696;		refPoints[3].y=0.4;
	refPoints[4].x=928;		refPoints[4].y=0.2;
	refPoints[5].x=1160;	refPoints[5].y=0.15;
	refPoints[6].x=1392;	refPoints[6].y=0.1;
	refPoints[7].x=1624;	refPoints[7].y=0.07;
	refPoints[8].x=1860;	refPoints[8].y=0.01;
//	refPoints[9].x=9;		refPoints[0].y=1;



	for(UInt8 i=0;i!=10;i++){	referencePointList.add(&(refPoints[i])); }
}



