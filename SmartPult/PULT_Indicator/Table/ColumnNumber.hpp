/*
 * ColumnNumber.hpp
 *
 *  Created on: 22 мар. 2019 г.
 *      Author: User9
 */

#ifndef PULT_INDICATOR_TABLE_COLUMNNUMBER_HPP_
#define PULT_INDICATOR_TABLE_COLUMNNUMBER_HPP_

#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "LCD Classes.h"
#include "LensParam/LensData.hpp"
#include "Libs/Utils/Debug.hpp"
#include "TableText.hpp"
#include "TableForm.hpp"

namespace Table {


template <size_t maxColumn, size_t sizeColumnData,size_t longStr>
class ColumnNumber {

private:
    LCD_Cell edit[maxColumn];
    TableText <1,sizeColumnData,longStr> text;
    bool _visible;
public:
    ColumnNumber (bool visible=true): _visible(visible) {
        Orientation position;
        position.string=0;
        position.column=0;
        for (position.column=0;position.column<sizeColumnData; position.column++) {
            sprintf(&text.string(position)[0],"%u",position.column+1);
            edit[position.column].p_text=&text.string(position)[0];
            }
        for (position.column=0; position.column<maxColumn; position.column++ ) {
            edit[position.column].p_text=&text.string(position)[0];
            }
        }
    ColumnNumber (bool visible=true,t_Pos_Size_XY startParamColumn) {

        }

    inline void setVisible (bool visible) { this->_visible=visible;    }

    inline const bool& visible () {return _visible;}

    inline void setCoord (Orientation &position, t_Pos_Size_XY & coord ) {
        edit[position.column].Set_Coord(coord);  }

    inline void setAllCoord (t_Pos_Size_XY & coord) {
        for (size_t id=0; id<maxColumn; id++) {
            edit[id].Set_Coord(coord);
            coord.X+=coord.Xsize;
            }
        }

    inline void scrollLeft(Orientation &position) {
        for (size_t id=maxColumn-1;id>0; id--)
            edit[id].p_text=edit[id-1].p_text;
        edit[0].p_text=&text.string(0, position.column)[0];
        }

    inline void scrollRight (Orientation &position) {
        for (size_t id=0; id<maxColumn-1; id++)
            edit[id].p_text=edit[id-1].p_text;
        edit[maxColumn-1].p_text=&text.string(0, position.column)[0];
        }

    inline void drawColumnNumber() {
        for (size_t id=0; id<maxColumn; id++) {
            edit[id].Draw();
            }
        }

    inline void drawSinglColumnNumber(Orientation &position) {
        edit[position.column].ReDraw(); }
    };

}

#endif /* PULT_INDICATOR_TABLE_COLUMNNUMBER_HPP_ */
