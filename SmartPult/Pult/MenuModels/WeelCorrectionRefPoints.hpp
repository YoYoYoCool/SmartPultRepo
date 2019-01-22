/*
 * WeelCorrectionRefPoints.hpp
 *
 *  Created on: 04 февр. 2016 г.
 *      Author: AhrameshinGN
 */

#ifndef PULT_MENUMODELS_WEELCORRECTIONREFPOINTS_HPP_
#define PULT_MENUMODELS_WEELCORRECTIONREFPOINTS_HPP_
#include "../Aproximators/AproximatorIF.hpp"

class WeelCorrectionPoints
{
public:
	StaticList<Point*, 9> referencePointList;
	WeelCorrectionPoints();
};





#endif /* PULT_MENUMODELS_WEELCORRECTIONREFPOINTS_HPP_ */
