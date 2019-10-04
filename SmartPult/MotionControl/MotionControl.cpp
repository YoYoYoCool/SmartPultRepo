/*
 * MotionControl.cpp
 *
 *  Created on: 08 дек. 2015 г.
 *      Author: AhrameshinGN
 */

#include "MotionControl.hpp"

#include "../Pult/Pult.h"

#define STR_(n)             #n
#define STR(n)              STR_(n)
#define DRIVE_NUM           0

ExtendedMotionTrack motionTrack1("fat:"STR(DRIVE_NUM)":track1.txt");
ExtendedMotionTrack motionTrack2("fat:"STR(DRIVE_NUM)":track2.txt");
ExtendedMotionTrack motionTrack3("fat:"STR(DRIVE_NUM)":track3.txt");
ExtendedMotionTrack motionTrack4("fat:"STR(DRIVE_NUM)":track4.txt");
ExtendedMotionTrack motionTrack5("fat:"STR(DRIVE_NUM)":track5.txt");
ExtendedMotionTrack motionTrack6("fat:"STR(DRIVE_NUM)":track6.txt");


//StaticList<MotionTrack*,6> motionTrackList;

MotionControl::MotionControl( MotionControlListener* pl):synchroPrescalerReload(42000),pult(pl)
{
    motionTrackList.setMaxSize(6);
	motionTrackList.add(&motionTrack1);
	motionTrackList.add(&motionTrack2);
	motionTrackList.add(&motionTrack3);
	motionTrackList.add(&motionTrack4);
	motionTrackList.add(&motionTrack5);
	motionTrackList.add(&motionTrack6);
	tracks=&motionTrackList;

	moduleState=MOTION_CONTROL_READY;
	trackID=0;
	reversePlay=false;
	synchroPrescaler=0;
	freeSpace=0;
	playMode=MOTION_PLAY_MODE_ONCE;
	mixMode=MOTION_MIX_TRACK_ONLY;

	angles.reset();
	angleFlag=MOTION_ANGLE_READY;
	active=false;

	isInit=false;

}
