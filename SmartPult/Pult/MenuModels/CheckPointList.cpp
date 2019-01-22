/*
 * CheckPointList.cpp
 *
 *  Created on: 10 февр. 2016 г.
 *      Author: AhrameshinGN
 */

#include "CheckPointList.hpp"

 CheckPoint bit0("PdusF");
 CheckPoint bit1("DdusF");
 CheckPoint bit2("TdusF");
 CheckPoint bit3("GvF");
 CheckPoint bit4("EncF");
 CheckPoint bit5("PultF");
 CheckPoint bit6("Bit6");
 CheckPoint bit7("Bit7");
 CheckPoint bit8("Bit8");
 CheckPoint bit9("Bit9");
 CheckPoint bit10("Bit10");
 CheckPoint bit11("Bit11");
 CheckPoint bit12("Bit12");
 CheckPoint bit13("Bit13");
 CheckPoint bit14("Bit14");
 CheckPoint bit15("Bit15");

 CheckPoint::CheckPoint(const char* n): name(n)
 {
	 flagDyn=false;
	 flagStat=false;
 }

 CheckPointList::CheckPointList()
 {
	 stList.add(&bit0);
	 stList.add(&bit1);
	 stList.add(&bit2);
	 stList.add(&bit3);
	 stList.add(&bit4);
	 stList.add(&bit5);
	 stList.add(&bit6);
	 stList.add(&bit7);
	 stList.add(&bit8);
	 stList.add(&bit9);
	 stList.add(&bit10);
	 stList.add(&bit11);
	 stList.add(&bit12);
	 stList.add(&bit13);
	 stList.add(&bit14);
	 stList.add(&bit15);

 }

 List<CheckPoint*>* CheckPointList::getList(){return &stList;}

 void CheckPointList::parseBytes(UInt16 dyn, UInt16 stat)
 {
	 UInt16 tmp=1;
	 for(UInt8 i=0;i!=16;i++)
	 {


		if(	((tmp<<i)&(dyn))!=0)
		{
			stList.get(i)->flagDyn=true;
		}
		else
		{
			stList.get(i)->flagDyn=false;
		}

		if(((tmp<<i)&(stat))!=0)
		{
			stList.get(i)->flagStat=true;
		}
		else
		{
			stList.get(i)->flagStat=false;
		}

	 }
 }






