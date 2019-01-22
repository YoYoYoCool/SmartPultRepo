/*
 * EqualaserRefPoints.cpp
 *
 *  Created on: 26 но€б. 2015 г.
 *      Author: AhrameshinGN
 */
#include "EqualaserRefPoints.hpp"

Point panReferencePoint[EQUALASER_LINES_COUNT];
Point tiltReferencePoint[EQUALASER_LINES_COUNT];
Point dutchReferencePoint[EQUALASER_LINES_COUNT];
Point zoomReferencePoint[EQUALASER_LINES_COUNT];
EqualaserRefPoints::EqualaserRefPoints()
{
	for(UInt8 i=0;i!=EQUALASER_LINES_COUNT;i++)
	{
		 panReferencePointList.add(&(panReferencePoint[i]));
		 tiltReferencePointList.add(&tiltReferencePoint[i]);
		 dutchReferencePointList.add(&dutchReferencePoint[i]);
		 zoomReferencePointList.add(&zoomReferencePoint[i]);
	}
}


