/*
 * CheckPointList.hpp
 *
 *  Created on: 10 февр. 2016 г.
 *      Author: AhrameshinGN
 */

#ifndef PULT_CHECKPOINTLIST_HPP_
#define PULT_CHECKPOINTLIST_HPP_
#include <../GyConCommon/Utils/Collections.h>
class CheckPoint
{
public:
	const char* name;
	volatile bool flagDyn;
	volatile bool flagStat;
	CheckPoint(const char* n);
};

class CheckPointList {
private:
	StaticList<CheckPoint*, 16> stList;
public:
	CheckPointList();
	List<CheckPoint*>* getList();
	void parseBytes(UInt16 dyn, UInt16 stat);
};



#endif /* PULT_CHECKPOINTLIST_HPP_ */
