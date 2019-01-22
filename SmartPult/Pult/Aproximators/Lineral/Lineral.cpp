/*
 * Lineral.cpp
 *
 *  Created on: 24 но€б. 2015 г.
 *      Author: AhrameshinGN
 */
#include "../AproximatorIF.hpp"


void LineralAproximator::updateReferencePoints()
{
	isComputedValue=true;
}

float LineralAproximator::getRezultByInterval(float xBase1,float yBase1,float xBase2,float yBase2,float x)
{
	if((xBase2-xBase1)==0){isFaildValue=true;return 0;}
//	if((yBase2-yBase1)==0){isFaildValue=true;return 0;}
	float ret=(((x-xBase1)*(yBase2-yBase1))/(xBase2-xBase1))+yBase1;
	return ret;
}
UInt32 LineralAproximator::serchInterval(float x)
{
	for(UInt32 i=0;i!=referencePoints.getSize();i++)
	{
		if(x<referencePoints.get(i)->x)
		{
			if(i==0){return i+1;}
			return i;
		}
	}
	return referencePoints.getSize()-1;
}
float LineralAproximator::calculate(float x)
{
	isFaildValue=true;
	if(referencePoints.getSize()<=1){return 0.0;}
	UInt32 tmp=serchInterval(x);
	if(tmp<=0||tmp>=referencePoints.getSize()){return 0;}
	isFaildValue=false;
	float ret=getRezultByInterval(referencePoints.get(tmp-1)->x,referencePoints.get(tmp-1)->y,referencePoints.get(tmp)->x,referencePoints.get(tmp)->y,x);
	ret*=K;
	if(ret>=max){ret=max;}
	if(ret<=min){ret=min;}
	return ret;

}



