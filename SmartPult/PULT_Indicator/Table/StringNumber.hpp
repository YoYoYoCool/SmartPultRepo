/*
 * StringNumber.hpp
 *
 *  Created on: 22 мар. 2019 г.
 *      Author: User9
 */

#ifndef PULT_INDICATOR_TABLE_STRINGNUMBER_HPP_
#define PULT_INDICATOR_TABLE_STRINGNUMBER_HPP_

#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "LCD Classes.h"
#include "Libs/Utils/Debug.hpp"
#include "TableForm.hpp"
#include "TableText.hpp"

namespace Table {
template <size_t maxString, size_t sizeStringData,size_t longStr>
class StringNumber {
private:
    LCD_Cell edit[maxString];
    TableText <sizeStringData,1, longStr> text;
    bool _visible;
public:

    StringNumber(bool visible=true): _visible(visible) {
        Orientation position;
        position.string=0;
        position.column=0;
        for (position.string=0;position.string<sizeStringData; position.string++) {
            sprintf(&text.string(position)[0],"%u",position.column+1);
            edit[position.string].p_text=&text.string(position)[0];
            }
        for (position.string=0; position.string<maxString; position.column++ ) {
            edit[position.string].p_text=&text.string(position)[0];
            }
        }

    StringNumber(bool visible=true,t_Pos_Size_XY startParamString) {

        }

    inline void scrollUp (const Orientation &position)  {
        for (size_t id=maxString-1; id>0; id--)
            edit[id].p_text=edit[id-1].p_text;
        edit[0].p_text=&text.string(position.string,0)[0];
        }

    inline void scrollDown(const Orientation &position) {
        for (size_t id=0; id<maxString-1; id++)
            edit[id].p_text=edit[id+1].p_text;
        edit[maxString-1].p_text=&text.string(position.string,0)[0];
        }

    inline void setVisible (bool visible) { this->_visible=visible;    }

    inline const bool& visible () {return _visible;}

    inline void setCoord (const Orientation &position,const t_Pos_Size_XY & coord ) {
            edit[position.string].Set_Coord(coord);  }

    inline void setAllCoord (t_Pos_Size_XY & coord) {
        for (size_t id=0; id<maxString; id++) {
            edit[id].Set_Coord(coord);
            coord.Y+=coord.Ysize;
            }
        }

    inline void drawStringNumber () {
        for (size_t id=0; id<maxString; id++) {
            edit[id].ReDraw();
            }
        }

    inline void dinglDrawStringNumber( const Orientation &position) {
        edit[position.string].ReDraw();
        }

    };
}



#endif /* PULT_INDICATOR_TABLE_STRINGNUMBER_HPP_ */
