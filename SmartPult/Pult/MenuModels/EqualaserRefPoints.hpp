/*
 * EqualaserRefPoints.hpp
 *
 *  Created on: 26 но€б. 2015 г.
 *      Author: AhrameshinGN
 */

#ifndef PULT_MENUMODELS_EQUALASERREFPOINTS_HPP_
#define PULT_MENUMODELS_EQUALASERREFPOINTS_HPP_
#include "../Aproximators/AproximatorIF.hpp"
#define EQUALASER_LINES_COUNT 14
class EqualaserRefPoints
{
public:
	StaticList<Point*, EQUALASER_LINES_COUNT> panReferencePointList;
	StaticList<Point*, EQUALASER_LINES_COUNT> tiltReferencePointList;
	StaticList<Point*, EQUALASER_LINES_COUNT> dutchReferencePointList;
	StaticList<Point*, EQUALASER_LINES_COUNT> zoomReferencePointList;
	EqualaserRefPoints();
};



#endif /* PULT_MENUMODELS_EQUALASERREFPOINTS_HPP_ */
