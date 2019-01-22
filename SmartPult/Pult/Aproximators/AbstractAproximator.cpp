/*
 * AbstractAproximator.cpp
 *
 *  Created on: 24 но€б. 2015 г.
 *      Author: AhrameshinGN
 */
#include "AproximatorIF.hpp"

AbstractAproximator::AbstractAproximator( List<Point*> staticList):
		referencePoints(staticList)
		{
			isComputedValue=false;
			isFaildValue=true;
			min=-1;
			max=1;
			K=1;
		}

AbstractAproximator::AbstractAproximator( List<Point*> staticList,float k, float minValue, float maxValue):
		referencePoints(staticList)
		{
			isComputedValue=false;
			isFaildValue=true;
			min=minValue;
			max=maxValue;
			K=k;
		}



bool AbstractAproximator::isComputed_()
{
	return isComputedValue;
}
bool AbstractAproximator::isFaild()
{
	return isFaildValue;
}
void AbstractAproximator::setRange(float K_, float min_, float max_)
{
	min=min_;
	max=max_;
	K=K_;
}

