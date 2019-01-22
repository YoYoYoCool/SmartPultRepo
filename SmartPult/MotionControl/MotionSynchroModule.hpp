/*
 * MotionSynchroModule.hpp
 *
 *  Created on: 18 θώνÿ 2016 γ.
 *      Author: Grigoriy
 */

#ifndef MOTIONCONTROL_MOTIONSYNCHROMODULE_HPP_
#define MOTIONCONTROL_MOTIONSYNCHROMODULE_HPP_
#include <../GyConCommon/dataTypes.h>
#include <../GyConCommon/ExchangeDrivers/UartDriver.h>
#include "Board.h"
#define MOTION_SYNCHRO_BUFFER_LEN 4

#define MOTION_SYNCHRO_USE_EXTRENAL_COMMAND

enum MotionSynchroCommandSet
{
	MOTION_SYNCHRO_SUCCESS=0,
	MOTION_SYNCHRO_GO_TO_ANGLES,
	MOTION_SYNCHRO_SLAVE2_PLAY,
	MOTION_SYNCHRO_REV_PLAY,
	MOTION_SYNCHRO_STOP,
	MOTION_SYNCHRO_PAUSE,
	MOTION_SYNCHRO_REC,
	MOTION_SYNCHRO_READY_TO_PLAY,
	MOTION_SYNCHRO_MASTER_PLAY,
	MOTION_SYNCHRO_SLAVE1_PLAY,
	MOTION_SYNCHRO_SLAVE3_PLAY
};

union MotionSynchroErrorBits
{
	volatile UInt8 all;
	struct
	{
		volatile UInt8 overError;
		volatile UInt8 NU1;
		volatile UInt8 NU2;
		volatile UInt8 NU3;
		volatile UInt8 NU4;
		volatile UInt8 NU5;
		volatile UInt8 NU6;
		volatile UInt8 NU7;
	}bits;
};

class MotionSynchroTransmitBuffer
{
	public:
		MotionSynchroTransmitBuffer()
		{
			for(UInt8 i=0;i!=MOTION_SYNCHRO_BUFFER_LEN;i++)
			{
				buf[i]=0;
			}
		}

		bool compareCRC();
		bool genCRC();
		UInt8 getBufferLen(){return MOTION_SYNCHRO_BUFFER_LEN;}
		UInt8* getBuffer(){return buf;}
		MotionSynchroCommandSet getCommand(){return (MotionSynchroCommandSet)buf[0];}
		MotionSynchroErrorBits getError();
		void setCommand(MotionSynchroCommandSet command){buf[0]=command;}
		void setError(MotionSynchroErrorBits bits){buf[1]=bits.all;}

		void resetBuffer()
		{
			for(UInt8 i=0;i!=MOTION_SYNCHRO_BUFFER_LEN;i++)
			{
				buf[i]=0;
			}
		}
		void resetErrors(){buf[1]=0;}
	private:
		UInt8 buf[MOTION_SYNCHRO_BUFFER_LEN];

};


class MotionSynchroTransmiter
{
	public:
		MotionSynchroTransmitBuffer writeBuffer;
		MotionSynchroTransmitBuffer readBuffer;
		volatile bool isConnected;
		volatile bool isActive;
		MotionSynchroTransmiter():
		    isConnected(false),
		    isActive(false)
		{

		}

		 void init(UartDriver& driver_)
		 {
		     driver=&driver_;
		 }

		void transmit();
		inline void activateModule()            {isActive=true; }
		inline void deactivateModule()          {isActive=false;}

		inline void setSuccessCommand()			{writeBuffer.setCommand(MOTION_SYNCHRO_SUCCESS);		}
		inline void setGTACommand()				{writeBuffer.setCommand(MOTION_SYNCHRO_GO_TO_ANGLES);	}
		inline void setMasterPlayCommand()		{writeBuffer.setCommand(MOTION_SYNCHRO_MASTER_PLAY);	}
		inline void setRevPlayCommand()			{writeBuffer.setCommand(MOTION_SYNCHRO_REV_PLAY);		}
		inline void setStopCommand()			{writeBuffer.setCommand(MOTION_SYNCHRO_STOP);			}
		inline void setPauseCommand()			{writeBuffer.setCommand(MOTION_SYNCHRO_PAUSE);			}
		inline void setRecCommand()				{writeBuffer.setCommand(MOTION_SYNCHRO_REC);			}
		inline void setReadyToPlayCommand()		{writeBuffer.setCommand(MOTION_SYNCHRO_READY_TO_PLAY);	}
		inline void setSlave1PlayCommand()	    {writeBuffer.setCommand(MOTION_SYNCHRO_SLAVE1_PLAY);	}
		inline void setSlave2PlayCommand()	    {writeBuffer.setCommand(MOTION_SYNCHRO_SLAVE2_PLAY);	}
		inline void setSlave3PlayCommand()		{writeBuffer.setCommand(MOTION_SYNCHRO_SLAVE3_PLAY);	}

#ifdef MOTION_SYNCHRO_USE_EXTRENAL_COMMAND

		inline bool isSuccessComand()		{return ((readBuffer.getCommand()==MOTION_SYNCHRO_SUCCESS)&&isConnected);       }
		inline bool isGTAComand()			{return ((readBuffer.getCommand()==MOTION_SYNCHRO_GO_TO_ANGLES)&&isConnected);  }
		inline bool isMasterPlayComand()	{return ((readBuffer.getCommand()==MOTION_SYNCHRO_MASTER_PLAY)&&isConnected);   }
		inline bool isRevPlayComand()		{return ((readBuffer.getCommand()==MOTION_SYNCHRO_REV_PLAY)&&isConnected);      }
		inline bool isStopComand()			{return ((readBuffer.getCommand()==MOTION_SYNCHRO_STOP)&&isConnected);          }
		inline bool isPauseComand()			{return ((readBuffer.getCommand()==MOTION_SYNCHRO_PAUSE)&&isConnected);         }
		inline bool isRecComand()			{return ((readBuffer.getCommand()==MOTION_SYNCHRO_REC)&&isConnected);           }
		inline bool isReadyToPlayComand()	{return ((readBuffer.getCommand()==MOTION_SYNCHRO_READY_TO_PLAY)&&isConnected); }
		inline bool isSlave1PlayComand()	{return ((readBuffer.getCommand()==MOTION_SYNCHRO_SLAVE1_PLAY)&&isConnected);   }
		inline bool isSlave2PlayComand()	{return ((readBuffer.getCommand()==MOTION_SYNCHRO_SLAVE2_PLAY)&&isConnected);   }
		inline bool isSlave3PlayComand()	{return ((readBuffer.getCommand()==MOTION_SYNCHRO_SLAVE3_PLAY)&&isConnected);   }

#else
		inline bool isSuccessComand()       {   return false;   }
		inline bool isGTAComand()           {   return false;   }
		inline bool isMasterPlayComand()    {   return false;   }
		inline bool isRevPlayComand()       {   return false;   }
		inline bool isStopComand()          {   return false;   }
		inline bool isPauseComand()         {   return false;   }
		inline bool isRecComand()           {   return false;   }
		inline bool isReadyToPlayComand()   {   return false;   }
		inline bool isSlave1PlayComand()    {   return false;   }
		inline bool isSlave2PlayComand()    {   return false;   }
		inline bool isSlave3PlayComand()    {   return false;   }
#endif

		inline void resetModule()
		{
			this->writeBuffer.resetBuffer();
			this->readBuffer.resetBuffer();
		}

	private:
		UartDriver* driver;
};




#endif /* MOTIONCONTROL_MOTIONSYNCHROMODULE_HPP_ */
