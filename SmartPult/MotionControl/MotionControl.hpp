/*
 * MotionControl.hpp
 *
 *  Created on: 08 дек. 2015 г.
 *      Author: AhrameshinGN
 */

#ifndef MOTIONCONTROL_MOTIONCONTROL_HPP_
#define MOTIONCONTROL_MOTIONCONTROL_HPP_
#include <../GyConCommon/dataTypes.h>
#include <../GyConCommon/Utils/Collections.h>
#include <stdio.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/knl/Task.h>
#define MOTION_AXIS_COUNT 9



enum MotionControlState
{
	MOTION_CONTROL_READY=0,
	MOTION_CONTROL_PLAY,
	MOTION_CONTROL_REC,
	MOTION_CONTROL_PAUSE
};

class MotionPoint
{
public:
	volatile float panRef;
	volatile float dutchRef;
	volatile float tiltRef;

	volatile float zoomRef;
	volatile float irisRef;
	volatile float focusRef;

	volatile float poleRef;

	volatile float levelCorrectionSpeed;
	volatile UInt32 controlBits;

	void renderToArray(UInt32* buffer)
	{
		buffer[0]=*((UInt32*)(&panRef));
		buffer[1]=*((UInt32*)(&dutchRef));
		buffer[2]=*((UInt32*)(&tiltRef));
		buffer[3]=*((UInt32*)(&zoomRef));
		buffer[4]=*((UInt32*)(&irisRef));
		buffer[5]=*((UInt32*)(&focusRef));

		buffer[6]=*((UInt32*)(&levelCorrectionSpeed));
		buffer[7]=controlBits;

		buffer[8]=*((UInt32*)(&poleRef));;
	}

	void parseArray(UInt32* buffer)
	{
		panRef=		*((float*)(&(buffer[0])));
		dutchRef=	*((float*)(&(buffer[1])));
		tiltRef=	*((float*)(&(buffer[2])));
		zoomRef=	*((float*)(&(buffer[3])));
		irisRef=	*((float*)(&(buffer[4])));
		focusRef=	*((float*)(&(buffer[5])));

		levelCorrectionSpeed=*((float*)(&(buffer[6])));
		controlBits=	buffer[7];

		poleRef=	*((float*)(&(buffer[8])));

	}

	void reset()
	{
		panRef=0.0;
		dutchRef=0.0;
		tiltRef=0.0;
		zoomRef=0.0;
		focusRef=0.0;
		irisRef=0.0;

		levelCorrectionSpeed=0.0;
		controlBits=0.0;

		poleRef=0.0;
	}

	void reversSpeedValue()
	{
		panRef=-panRef;
		dutchRef=-dutchRef;
		tiltRef=-tiltRef;
		zoomRef=-zoomRef;
		poleRef=-poleRef;
	}

	inline UInt8 getBufferCount(){return MOTION_AXIS_COUNT;}
};


enum MotionTrackMetaTypes
{
	MOTION_META_POINT_COUNT=0,
	MOTION_META_ZOOM_START,
	MOTION_META_PAN_START,
	MOTION_META_DUTCH_START,
	MOTION_META_TILT_START,
	MOTION_META_ZOOM_END,
	MOTION_META_PAN_END,
	MOTION_META_DUTCH_END,
	MOTION_META_TILT_END,
	MOTION_META_TRANSFER_DELAY,
	MOTION_META_COUNT

};
class MotionTrackMeta
{

public:
	MotionTrackMeta(){	 resetData();	}
	volatile UInt32 buffer[MOTION_META_COUNT];
	inline void setPointCount(UInt32 count){				buffer[0]=count;}
	inline UInt32 getPointCount(){				return buffer[0];}
	float getDate(MotionTrackMetaTypes type){				return *((float*)(&buffer[type]));	}
	void setDate(MotionTrackMetaTypes type, float date)	{	buffer[type]=*((UInt32*)(&date));	}

	 inline UInt16  getBufferLen(){return MOTION_META_COUNT;}
	 void resetData()
	 {
		 for(UInt8 i=0;i!=MOTION_META_COUNT;i++) {	 buffer[i]=0;	 }
	 }
};
class MotionTrack
{
protected:
	UInt32 maxCount;
public:
	MotionTrackMeta meta;
	UInt32 iterator;
	MotionTrack(UInt32 maxCount_):
	maxCount(maxCount_){iterator=0;}

	virtual MotionPoint* getCurrentPoint()=0;
	virtual bool openTrack()				{return true;}
	virtual bool closeTrack()				{return true;}
	virtual bool isOpen()					{return true;}
	virtual bool synchronizeWithStorage()	{return true;}
	void setMaxCount(UInt32 count){maxCount=count;};
	UInt32 getMaxCount(){return maxCount;}
	virtual bool appEndPoint(){	return nextPoint();	}
	virtual bool nextPoint()
	{
		if(iterator>=maxCount-1){return false;}
		else{iterator++;return true;}
	}
	virtual bool seekStart(){	iterator=0;	return true;}
	virtual bool seekEnd()	{	iterator=maxCount-1;return true;}
	virtual bool prevPoint()
	{
		if(iterator<=0)	{return false;}
		else{iterator--;return true;}
	}

	virtual bool readMeta()=0;
	virtual bool saveMeta()=0;
	virtual bool clearTrack(){maxCount=0;iterator=0;return true;}

};

class SimpleMotionTrack: public MotionTrack
{

	MotionPoint points[1024];
public:
	SimpleMotionTrack():
		MotionTrack(1024){}
	virtual MotionPoint* getCurrentPoint()
	{
		return &points[iterator];
	}
};

#define TRACK_META_SIZE 512


class ExtendedMotionTrack:public MotionTrack
{
	MotionPoint curPoint;
	FILE* trackFile;
	const char* fileName;
	bool isOpened;
	UInt32 buf[MOTION_AXIS_COUNT];
	public:
		ExtendedMotionTrack(const char* fileName_):
		MotionTrack(0),
		fileName(fileName_)
		{
			curPoint.reset();
			isOpened=false;
		}
		virtual MotionPoint* getCurrentPoint()
		{
			return &curPoint;
		}
		virtual bool nextPoint()
		{
			if(!MotionTrack::nextPoint()){return false;}
			if(fread(((void*)(buf)), 4,curPoint.getBufferCount(),trackFile)!=curPoint.getBufferCount())
			{
				curPoint.reset();
				return false;
			}
			curPoint.parseArray(buf);
			return true;
		}
		virtual bool seekStart()
		{
			MotionTrack::seekStart();
			if(!trackFile){return false;}
			if(fseek(trackFile,TRACK_META_SIZE,SEEK_SET)!=0){return false;}
			return true;
		}
		virtual bool seekEnd()
		{
			MotionTrack::seekEnd();
			if(!trackFile){return false;}
			if(fseek(trackFile,0,SEEK_END)!=0){return false;}
			return true;
		}
		virtual bool prevPoint()
		{
			if(!MotionTrack::prevPoint()){return false;}
			if(fseek(trackFile,-(curPoint.getBufferCount()*4),SEEK_CUR)!=0)
			{
				curPoint.reset();
				return false;
			}
			if(ftell(trackFile)<512)
			{
				curPoint.reset();
				return false;
			}
			if(fread(((void*)(buf)), 4,curPoint.getBufferCount(),trackFile)!=curPoint.getBufferCount())
			{
				curPoint.reset();
				return false;
			}
			if(fseek(trackFile,-(curPoint.getBufferCount()*4),SEEK_CUR)!=0)
			{
				curPoint.reset();
				return false;
			}
			curPoint.parseArray(buf);
			curPoint.reversSpeedValue();
			return true;


		}
		virtual bool appEndPoint()
		{
			if(!MotionTrack::nextPoint()){return false;}
			curPoint.renderToArray(buf);
			if(fwrite((void*)buf,4,curPoint.getBufferCount(),trackFile)!=curPoint.getBufferCount())
			{
				curPoint.reset();
				return false;
			}
			return true;
		}
		virtual bool openTrack()
		{
			trackFile=fopen(fileName, "r+b");
			if(!trackFile)
			{
				trackFile=fopen(fileName, "w+b");
				if(!trackFile){return false;}
				buf[0]=0;
				for(UInt16 i=0;i!=TRACK_META_SIZE;i++)
				{
					fwrite((void*)buf,1,1,trackFile);
				}
			}
			this->isOpened=true;
			return seekStart();
		};
		virtual bool closeTrack()
		{
			if(trackFile!=0)
			{
				if(fclose(trackFile)!=0){return false;}
			}
			this->isOpened=false;
			trackFile=0;
			return true;
		}
		virtual bool isOpen(){	return isOpened||(trackFile!=0);}

		virtual bool synchronizeWithStorage()
		{
			if(fclose(trackFile)!=0)
			{
			    return false;
			}
			this->isOpened=false;
			trackFile=fopen(fileName, "r+b");
			if(!trackFile)
			{
			    return false;
			}
			this->isOpened=true;
			return seekEnd();
		}

		virtual bool readMeta()
		{
			UInt32 tmp=0;
			if(isOpened&&(trackFile!=0))
			{
				tmp=ftell(trackFile);
				if(fseek(trackFile,0,SEEK_SET)!=0){return false;}
				if(fread(((void*)(meta.buffer)), 4,MOTION_META_COUNT,trackFile)!=MOTION_META_COUNT){meta.resetData();return false;}
				if(fseek(trackFile,tmp,SEEK_SET)!=0){return false;}
				return true;
			}
			else
			{
				if(!openTrack()){return false;}

				if(fseek(trackFile,0,SEEK_SET)!=0){return false;}
				if(fread(((void*)(meta.buffer)), 4,MOTION_META_COUNT,trackFile)!=MOTION_META_COUNT){meta.resetData();return false;}
				return closeTrack();
			}
		}
		virtual bool saveMeta()
		{
			UInt32 tmp=0;
			if(isOpened&&(trackFile!=0))
			{
				tmp=ftell(trackFile);
				if(fseek(trackFile,0,SEEK_SET)!=0){return false;}
				if(fwrite(((void*)(meta.buffer)), 4,MOTION_META_COUNT,trackFile)!=MOTION_META_COUNT){meta.resetData();return false;}
				if(fseek(trackFile,tmp,SEEK_SET)!=0){return false;}
				return true;
			}
			else
			{
				if(!openTrack()){return false;}
				if(fseek(trackFile,0,SEEK_SET)!=0){return false;}
				if(fwrite(((void*)(meta.buffer)), 4,MOTION_META_COUNT,trackFile)!=MOTION_META_COUNT){meta.resetData();return false;}
				return closeTrack();
			}
		}

		virtual bool clearTrack()
		{
			MotionTrack::clearTrack();
			if(trackFile!=0){fclose(trackFile);}
			trackFile=fopen(fileName, "w+b");
			if(!trackFile){return false;}
			buf[0]=0;
			for(UInt16 i=0;i!=TRACK_META_SIZE;i++)		{	fwrite((void*)buf,1,1,trackFile);	}
			meta.resetData();
			return closeTrack();

		}
};

enum MotionPlayMode
{
	MOTION_PLAY_MODE_ONCE=0,
	MOTION_PLAY_MODE_CIRCLE,
	MOTION_PLAY_MODE_PP,
	MOTION_PLAY_MODE_COUNT
};
enum MotionMixMode
{
	MOTION_MIX_TRACK_ONLY=0,
	MOTION_MIX_PDTZ,
	MOTION_MIX_ALL,
	MOTION_MIX_COUNT
};
enum MotionAnglsFlag
{
		MOTION_ANGLE_READY=0,
		MOTION_GET_ANGLE,
		MOTION_GET_ANGLE_ZIF,
		MOTION_SET_ANGLE,
		MOTION_APPLY_SET_ANGLE,
		MOTION_APPLY_SET_WAIT,
		MOTION_ANGLE_WAIT,
		MOTION_ANGLE_RESET
};


class MotionControlListener
{
public:
	virtual void motionOnOff(bool onOff)
	{

	}
	virtual void motionSetTransferDelay(UInt32 delay)
	{

	}
	virtual void useAutonegatation()
	{

	}
	virtual bool isAutonegComplete()
	{
		return false;
	}
	virtual bool motionCompareEndSetTransferDelay(UInt32 delay)
	{
		return false;
	}
	virtual UInt32 getMiCurrentTransferDelay()
	{
		return 20;
	}

};

#define SD_FLASH_SIZE 0x2CB4178 //625x10^5 points (1.5Gb)
class MotionControl
{
	volatile MotionControlState moduleState;
	volatile MotionControlState pauseReturnModuleState;
	List<MotionTrack*>* tracks;
	volatile UInt8 trackID;
	volatile bool reversePlay;
	volatile UInt32 synchroPrescaler;
	const UInt32 synchroPrescalerReload;

	volatile UInt32 freeSpace;
	volatile MotionPlayMode playMode;
	volatile MotionMixMode mixMode;

	MotionPoint angles;
	MotionAnglsFlag angleFlag;

	volatile bool active;

	volatile bool isInit;

	void closeAll()
	{
		for(UInt8 i=0;i!=tracks->getSize();i++)
		{
			if(!tracks->get(i)->isOpen()){continue;}
			tracks->get(i)->closeTrack();
		}
	}

	inline void motionOnOff(bool dir)
	{
		if(pult!=NULL)
		{
			pult->motionOnOff(dir);
		}
	}

public:
	MotionControlListener* pult;
	MotionControl( MotionControlListener* pl);

	UInt32 getFreeSpace(){return this->freeSpace;}
	void swicthPlayMode()
	{
		switch(playMode)
		{
			case MOTION_PLAY_MODE_ONCE:		playMode=MOTION_PLAY_MODE_CIRCLE;return;
			case MOTION_PLAY_MODE_CIRCLE:	playMode=MOTION_PLAY_MODE_PP;return;
			case MOTION_PLAY_MODE_PP:		playMode=MOTION_PLAY_MODE_ONCE;return;
		}
		playMode=MOTION_PLAY_MODE_ONCE;
		return;
	}
	void switchMixMode()
	{
		switch(mixMode)
		{
			case MOTION_MIX_TRACK_ONLY:		mixMode=MOTION_MIX_PDTZ;return;
			case MOTION_MIX_PDTZ:			mixMode=MOTION_MIX_ALL;return;
			case MOTION_MIX_ALL:			mixMode=MOTION_MIX_TRACK_ONLY;return;
		}
		mixMode=MOTION_MIX_TRACK_ONLY;
		return;
	}
	MotionPlayMode	getPlayMode(){return playMode;}
	MotionMixMode	getMixMode(){return mixMode;}

	void resetModule()
	{
//		moduleState=MOTION_CONTROL_READY;
		switch(moduleState)
		{
			case MOTION_CONTROL_REC:
				stopRec();
				break;
			case MOTION_CONTROL_PLAY:
			case MOTION_CONTROL_PAUSE:
				stopPlay();
				break;
		}
		moduleState=MOTION_CONTROL_READY;
		angleFlag=MOTION_ANGLE_RESET;
		closeAll();

//Todo реализовать переход в стартовое состояние
	}
	bool pause()
	{
		if(moduleState!=MOTION_CONTROL_PAUSE)
		{
			pauseReturnModuleState=moduleState;
			moduleState=MOTION_CONTROL_PAUSE;
			return true;
		}

		else
		{
			moduleState=pauseReturnModuleState;
			pauseReturnModuleState=MOTION_CONTROL_PAUSE;
			return true;
		}
	}

	bool getTrackTransferDelay(UInt32* r)
	{
		float ret=20;
		if(!tracks->get(trackID)->readMeta()) {return false;}
		ret=tracks->get(trackID)->meta.getDate(MOTION_META_TRANSFER_DELAY);
		*r=(*((UInt32*)(&ret)));
		return true;
	}

	bool play()
	{
		float t=0;
		if(moduleState==MOTION_CONTROL_READY)
		{
			if(!tracks->get(trackID)->openTrack()){return false;}
			if(!tracks->get(trackID)->readMeta()) {return false;}
			tracks->get(trackID)->setMaxCount(tracks->get(trackID)->meta.getPointCount());

			angles.panRef=tracks->get(trackID)->meta.getDate(MOTION_META_PAN_START);
			angles.dutchRef=tracks->get(trackID)->meta.getDate(MOTION_META_DUTCH_START);
			angles.tiltRef=tracks->get(trackID)->meta.getDate(MOTION_META_TILT_START);
			angles.zoomRef=tracks->get(trackID)->meta.getDate(MOTION_META_ZOOM_START);

			t=tracks->get(trackID)->meta.getDate(MOTION_META_TRANSFER_DELAY);
			if(!pult->motionCompareEndSetTransferDelay(*((UInt32*)(&t))))
			{
				tracks->get(trackID)->closeTrack();
				return false;
			}

			if(!setAngle()){return false;}
			goToAngle();

			if(!tracks->get(trackID)->seekStart()){return false;}
			reversePlay=false;
			return true;
		}
		return false;
	}
	bool reversPlay()
	{
		float t=0;
		if(moduleState==MOTION_CONTROL_READY)
		{
			if(!tracks->get(trackID)->openTrack())	{return false;}
			if(!tracks->get(trackID)->readMeta()) 	{return false;}

			tracks->get(trackID)->setMaxCount(tracks->get(trackID)->meta.getPointCount());

			angles.panRef=tracks->get(trackID)->meta.getDate(MOTION_META_PAN_END);
			angles.dutchRef=tracks->get(trackID)->meta.getDate(MOTION_META_DUTCH_END);
			angles.tiltRef=tracks->get(trackID)->meta.getDate(MOTION_META_TILT_END);
			angles.zoomRef=tracks->get(trackID)->meta.getDate(MOTION_META_ZOOM_END);

			t=tracks->get(trackID)->meta.getDate(MOTION_META_TRANSFER_DELAY);
			if(!pult->motionCompareEndSetTransferDelay(*((UInt32*)(&t))))
			{
				tracks->get(trackID)->closeTrack();
				return false;
			}

			if(!setAngle()){return false;}
			goToAngle();

			if(!tracks->get(trackID)->seekEnd())	{return false;}
			reversePlay=true;
			return true;
		}
		return false;
	}
	void playActivation()
	{
		moduleState=MOTION_CONTROL_PLAY;
	}
	bool stopPlay()
	{
		if(moduleState==MOTION_CONTROL_PLAY||moduleState==MOTION_CONTROL_PLAY)
		{
			moduleState=MOTION_CONTROL_READY;
			tracks->get(trackID)->closeTrack();
			return true;
		}
		return false;
	}
	bool rec()
	{
		if(moduleState==MOTION_CONTROL_READY)
		{
			if(!getAngle()){return false;}
			if(!clearCurrentTrack()){return false;}
			if(!tracks->get(trackID)->openTrack()){return false;}
			if(!tracks->get(trackID)->seekStart()){return false;}

			tracks->get(trackID)->meta.setDate(MOTION_META_PAN_START,angles.panRef);
			tracks->get(trackID)->meta.setDate(MOTION_META_DUTCH_START,angles.dutchRef);
			tracks->get(trackID)->meta.setDate(MOTION_META_TILT_START,angles.tiltRef);
			tracks->get(trackID)->meta.setDate(MOTION_META_ZOOM_START,angles.zoomRef);

//			freeSpace+=tracks->get(trackID)->meta.getPointCount();
			if(freeSpace<100){return false;}
			tracks->get(trackID)->setMaxCount(freeSpace);
			moduleState=MOTION_CONTROL_REC;
			return true;
		}
		return false;
	}
	bool stopRec()
	{
		UInt32 t=20;
		if(moduleState==MOTION_CONTROL_REC)
		{

			moduleState=MOTION_CONTROL_READY;
			tracks->get(trackID)->meta.setPointCount(tracks->get(trackID)->iterator);
			tracks->get(trackID)->setMaxCount(tracks->get(trackID)->iterator);


			t=pult->getMiCurrentTransferDelay();
			tracks->get(trackID)->meta.setDate(MOTION_META_TRANSFER_DELAY,*((float*)(&t)));

			if(!getAngle())
			{
				tracks->get(trackID)->meta.setDate(MOTION_META_PAN_END,0.0);
				tracks->get(trackID)->meta.setDate(MOTION_META_DUTCH_END,0.0);
				tracks->get(trackID)->meta.setDate(MOTION_META_TILT_END,0.0);
				tracks->get(trackID)->meta.setDate(MOTION_META_ZOOM_END,0.0);
			}
			else
			{
				tracks->get(trackID)->meta.setDate(MOTION_META_PAN_END,angles.panRef);
				tracks->get(trackID)->meta.setDate(MOTION_META_DUTCH_END,angles.dutchRef);
				tracks->get(trackID)->meta.setDate(MOTION_META_TILT_END,angles.tiltRef);
				tracks->get(trackID)->meta.setDate(MOTION_META_ZOOM_END,angles.zoomRef);
			}

			if(!tracks->get(trackID)->saveMeta())  {return false;}
			if(!tracks->get(trackID)->closeTrack()){return false;}
			return true;
		}
		return false;
	}

	bool setAngle()
	{
		angleFlag=MOTION_SET_ANGLE;

		for(UInt8 i=0;i!=10;i++)
		{
			if(angleFlag!=MOTION_ANGLE_READY)
			{
				Task_sleep(30);
				continue;
			}
		}

		if(angleFlag!=MOTION_ANGLE_READY)
		{
			angleFlag=MOTION_ANGLE_RESET;
			return false;
		}
		return true;

	}

	bool readyToPlay()
	{
		return (angleFlag==MOTION_ANGLE_READY);
	}
	bool getAngle()
	{
	    //-----------------------------------------
	    //Get HeadAngles
		angleFlag=MOTION_GET_ANGLE;
		//WaitResponse
		for(UInt8 i=0;i!=10;i++)
		{
			if(angleFlag!=MOTION_ANGLE_READY)
			{
				Task_sleep(30);
				continue;
			}
		}
		//Reset ifError
		if(angleFlag!=MOTION_ANGLE_READY)
		{
			angleFlag=MOTION_ANGLE_RESET;
			return false;
		}

	    //-----------------------------------------
//	    //Get ZIF Angle
        angleFlag=MOTION_GET_ANGLE_ZIF;
        //WaitResponse
        for(UInt8 i=0;i!=10;i++)
        {
            if(angleFlag!=MOTION_ANGLE_READY)
            {
                Task_sleep(30);
                continue;
            }
        }
		//Continue if Error
        if(angleFlag!=MOTION_ANGLE_READY)
        {
            angleFlag=MOTION_ANGLE_RESET;
            return true;
        }
		return true;
	}

	void goToAngle()
	{
		angleFlag=MOTION_APPLY_SET_ANGLE;
	}
	MotionAnglsFlag* getAnglesFlag(){return &angleFlag;}
	MotionPoint* getAnglesData(){return &angles;}

	bool clearCurrentTrack()
	{
		freeSpace+=tracks->get(trackID)->meta.getPointCount();
		return tracks->get(trackID)->clearTrack();
	}

	bool init()
	{
		UInt32 totalSize=0;
		for(UInt8 i=0;i!=tracks->getSize();i++)
		{
			if(!tracks->get(i)->readMeta())
			{
				tracks->get(i)->meta.resetData();
			}
			else
			{
				totalSize+=tracks->get(i)->meta.getPointCount();
			}
		}

		if(totalSize>=SD_FLASH_SIZE){	freeSpace=0;}
		else{							freeSpace=SD_FLASH_SIZE-totalSize;	}

		isInit=true;
		return true;
	}

	bool isInit_(){return isInit;}

//	bool wdtCallback();
	void activate()
	{
		motionOnOff(true);
		pult->useAutonegatation();
		active=true;
	}
	void deactivated()
	{
		motionOnOff(false);
		active=false;
	}

	bool isReadyToRec()
	{
		if(pult->isAutonegComplete()){return true;}
		pult->useAutonegatation();
		if(pult->isAutonegComplete()){return true;}
		return false;
	}
	bool isReady(){	return 	(moduleState==MOTION_CONTROL_READY);}
	bool isPlay() {	return 	(moduleState==MOTION_CONTROL_PLAY );}
	bool isRec()  {	return 	(moduleState==MOTION_CONTROL_REC  );}
	bool isPaused(){return 	(moduleState==MOTION_CONTROL_PAUSE);}
	bool isActive(){return active;}
	bool getReversPlayFlag(){return reversePlay;}

	bool isEmptyTrack(UInt8 trackID_)
	{
		if(trackID_>=tracks->getSize()){return true;}
		if(tracks->get(trackID_)->meta.getPointCount()>0){return false;}
		return true;
	}
	inline bool isEmptyTrack()
	{
		return isEmptyTrack(trackID);
	}

	void selectCurrentTrackID(UInt8 trackID_)
	{
		trackID=trackID_;
	}
	UInt8 getCurrentTrackID(){return trackID;}

	MotionPoint* getCurrentPoint(){	return tracks->get(trackID)->getCurrentPoint();}
	MotionTrack* getCurrentTrack(){	return tracks->get(trackID);}


	void strobe()
	{
		switch(moduleState)
		{
			case MOTION_CONTROL_PLAY:
				if(!reversePlay)
				{
					if(!tracks->get(trackID)->nextPoint())
					{
						moduleState=MOTION_CONTROL_READY;
						tracks->get(trackID)->closeTrack();
					//	if(playMode==MOTION_PLAY_MODE_CIRCLE){	play();	}
					//	if(playMode==MOTION_PLAY_MODE_PP){	reversPlay();	}
					}
				}
				else
				{
					if(!tracks->get(trackID)->prevPoint())
					{
						moduleState=MOTION_CONTROL_READY;
						tracks->get(trackID)->closeTrack();
					//	if(playMode==MOTION_PLAY_MODE_CIRCLE){reversPlay();	}
					//	if(playMode==MOTION_PLAY_MODE_PP){	play();	}
					}
				}
				break;
			case MOTION_CONTROL_REC:
				if(!tracks->get(trackID)->appEndPoint())
				{
					moduleState=MOTION_CONTROL_READY;
					tracks->get(trackID)->closeTrack();
				}
				freeSpace--;

				//todo Разобраться детально с этой хероборой
				synchroPrescaler++;
				if(synchroPrescaler>synchroPrescalerReload)
				{
//					if(!tracks->get(trackID)->synchronizeWithStorage())
//					{
//						tracks->get(trackID)->closeTrack();
//						moduleState=MOTION_CONTROL_READY;
//
//					}
					synchroPrescaler=0;
				}
				break;
			default:
				break;
		}

	}

};
#endif /* MOTIONCONTROL_MOTIONCONTROL_HPP_ */
