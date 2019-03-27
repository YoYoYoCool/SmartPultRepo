/*
 * LineNumber.hpp
 *
 *  Created on: 26 мар. 2019 г.
 *      Author: User9
 */

#ifndef PULT_INDICATOR_TABLE_LINENUMBER_HPP_
#define PULT_INDICATOR_TABLE_LINENUMBER_HPP_

#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "LCD Classes.h"
#include "Libs/Utils/Debug.hpp"
#include "TableText.hpp"
#include "TableForm.hpp"
#include "StringNumber.hpp"
#include "ColumnNumber.hpp"

namespace Table {

static enum {
    sizeDispX=319,
    sizeDispY=239
};



template <size_t maxString, size_t maxColumn,
size_t sizeStringData, size_t sizeColumnData, size_t longStr>
class LineNumber {

private:
StringNumber <maxString,sizeStringData,longStr> string;
ColumnNumber <maxColumn,sizeColumnData,longStr> column;
bool visible;

public:

    LineNumber (bool visible=true): visible(visible),
    string(visible), column(visible){
        }

    void setPositionString(t_Pos_Size_XY& position) {
        position.Ysize=(sizeDispY-position.Y)/maxString;
        string.setAllCoord(position);
        }

    void setPositionColumn(t_Pos_Size_XY& position) {
        position.Xsize=(sizeDispX-position.X)/maxColumn;
        column.setAllCoord(position);
        }

    inline void setVisible (bool visible) {
        this->visible=visible;
        }

    inline bool getVisible () {
        return this->visible;
        }

    inline void scrollHorizontal (OrientationTable &position) {
        if (position.positionTable.column>position.maxPositionTableData.column){
            column.scrollRight(position.positionTableData);
            position.positionTable.column=position.maxPositionTableData.column;}
        if(position.positionTable.column<position.minPositionTableData) {
            column.scrollLeft(position.positionTableData);
            position.positionTable.column=position.minPositionTableData.column;}
            }

    inline void scrollVertical (OrientationTable &position) {
        if (position.positionTable.string>position.maxPositionTableData.column) {
            string.scrollDown(position.positionTableData);
            position.positionTable.string=position.maxPositionTableData.string;}
        if (position.positionTable.column<position.minPositionTableData.column) {
            string.scrollUp(position.minPositionTableData);
            position.positionTable.string=position.minPositionTableData.string;}
        }

    inline void draw () {
        string.drawStringNumber();
        column.drawColumnNumber();
        }

};


}



#endif /* PULT_INDICATOR_TABLE_LINENUMBER_HPP_ */
