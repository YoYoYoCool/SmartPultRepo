/*
 * MotionSyncroModule.cpp
 *
 *  Created on: 18 июня 2016 г.
 *      Author: Grigoriy
 */
#include "MotionSynchroModule.hpp"
#include <ti/sysbios/knl/Task.h>

#include "MotionControl/LogicController/MotionLogicController.hpp"
extern MotionButtonControl motionLogicController;
//todo Темное  программирование. Только ради костыля

void MotionSynchroTransmiter::transmit()
{
    Task_sleep(5);
    if(isActive)
    {
        motionLogicController.slaveSynchro();
        writeBuffer.genCRC();
        driver->write(writeBuffer.getBuffer(),writeBuffer.getBufferLen());
        UInt8 i=driver->read(readBuffer.getBuffer(),readBuffer.getBufferLen());

        writeBuffer.resetErrors();

        if(i!=readBuffer.getBufferLen())    {	readBuffer.resetBuffer();isConnected=false;	return;}
        if(!readBuffer.compareCRC())        {	readBuffer.resetBuffer();isConnected=false; return;}
        isConnected=true;
    }

}

MotionSynchroErrorBits MotionSynchroTransmitBuffer::getError()
{
	MotionSynchroErrorBits ret;
	ret.all=buf[1];
	return ret;
}

bool MotionSynchroTransmitBuffer::compareCRC()
{
	UInt8 tb_1=buf[MOTION_SYNCHRO_BUFFER_LEN-1];
	UInt8 tb_2=buf[MOTION_SYNCHRO_BUFFER_LEN-2];
	if(!genCRC()){return false;}
	if(buf[MOTION_SYNCHRO_BUFFER_LEN-1]!=tb_1){return false;}
	if(buf[MOTION_SYNCHRO_BUFFER_LEN-2]!=tb_2){return false;}
	return true;
}

bool MotionSynchroTransmitBuffer::genCRC()
	{
		UInt8 data;
		UInt8 crcl;
		UInt8 crch;
		UInt8 tmp;
		UInt32 i;
		tmp=0;
		crcl=0xFF;
		crch=0xFF;

		for(i=0;i!=MOTION_SYNCHRO_BUFFER_LEN-2;i++)
		{
			data=buf[i];

			data=data^crcl;
			tmp=data<<4;
			data=tmp^data;
			tmp=data>>5;
			crcl=crch;
			crch=data^tmp;
			tmp=data<<3;
			crcl=crcl^tmp;
			tmp=data>>4;
			crcl=crcl^tmp;

		}

		i=crch;
		i=i<<8;
		i|=crcl;
		i+=0x2d5a;

		buf[MOTION_SYNCHRO_BUFFER_LEN-1]=(i&0x0000FF00)>>8;
		buf[MOTION_SYNCHRO_BUFFER_LEN-2]=(i&0x000000FF);
		return true;

	}
