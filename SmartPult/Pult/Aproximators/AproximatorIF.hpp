/*
 * AproximatorIF.hpp
 *
 *  Created on: 23 но€б. 2015 г.
 *      Author: Grigoriy
 */

#ifndef PULT_APROXIMATORS_APROXIMATORIF_HPP_
#define PULT_APROXIMATORS_APROXIMATORIF_HPP_
#include <../GyConCommon/Utils/Collections.h>
#include "../PultClasses.h"

class Point
{
	public:
		volatile float	 x;
		volatile float	 y;
		Point(){x=0;y=0;}
};


class AbstractAproximator:public JoyStickFunction
{
	protected:
	    List<Point*> referencePoints;
		bool isComputedValue;
		bool isFaildValue;
		float min;
		float max;
		float K;
	public:
		AbstractAproximator(  List<Point*> staticList);
		AbstractAproximator( List<Point*> staticList,float k, float minValue, float maxValue);
		virtual void updateReferencePoints()=0;
		bool isComputed_();
		inline  bool isFaild();
		inline  void setRange(float K_,float min_, float max_);



};

class LineralAproximator:public AbstractAproximator
{
	public:
		LineralAproximator(List<Point*> staticList):
			AbstractAproximator(staticList){}
		LineralAproximator( List<Point*> staticList,float k, float minValue, float maxValue):
			AbstractAproximator(staticList,k,minValue, maxValue){}
		virtual float calculate(float x);
		virtual void updateReferencePoints();
	private:
		float  getRezultByInterval(float xBase1,float yBase1,float xBase2,float yBase2,float x);
		UInt32 serchInterval(float x);


};

#endif /* PULT_APROXIMATORS_APROXIMATORIF_HPP_ */
