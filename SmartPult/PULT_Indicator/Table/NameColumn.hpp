/*
 * NameColumn.hpp
 *
 *  Created on: 22 мар. 2019 г.
 *      Author: User9
 */

#ifndef PULT_INDICATOR_TABLE_NAMECOLUMN_HPP_
#define PULT_INDICATOR_TABLE_NAMECOLUMN_HPP_

#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "LCD Classes.h"
#include "Libs/Utils/Debug.hpp"
#include "TableText.hpp"

namespace Table {

static enum {
    sizeDispX=319,
    sizeDispY=239
};

template <size_t maxColumn,size_t sizeColumnData,size_t longStr>
class NameColumn {
private:
    LCD_Cell edit[maxColumn];
    TableText <1,sizeColumnData,longStr> text;
    bool _visible;
public:
    NameColumn(tMenu_Link* nameColumn, bool visible=true): _visible(visible) {
        Orientation position;
        position.string=0;
        for (position.column=0; position.column<sizeColumnData;position.column++)
            sprintf(text.string(position.string, position.column),"%s",nameColumn[position.column]->Name);
        for (position.column=0; position.column<maxColumn;position.column++)
            edit[position.column].p_text=&text.string(position)[0];
        }
    NameColumn() {}
    inline void scrollLeft(const Orientation &position) {
        for (size_t id=maxColumn; id>1;is--)
            edit[id].p_text=edit[id-1].p_text;
        edit[1].p_text=&text.string(0, position.column)[0];
        }

    inline void setCoord (const Orientation &position, t_Pos_Size_XY & coord ) {
        edit[position.column].Set_Coord(coord);  }

    inline void setAllCoord (t_Pos_Size_XY & coord) {
        edit[0].Set_Coord(coord);
        coord.Ysize=ySyzeAllEditForms;
        coord.X+=coord.Xsize;
        coord.Xsize=(sizeDispX-coord.X)/(maxColumn-1);
        for (size_t idColumn=1; idColumn<maxColumn; idColumn++) {
            edit[idColumn].Set_Coord(coord);
            coord.X+=coord.Xsize;
            }
        }

    inline void setCoordNameCell(t_Pos_Size_XY & coord) {

        }

    inline void scrollRight(const Orientation &position) {
        for (size_t id=1; id<maxColumn-1;is--)
            edit[id].p_text=edit[id+1].p_text;
        edit[maxColumn-1].p_text=&text.string(0, position.column)[0];
        }

    inline void setVisible (bool visible) { this->_visible=visible;    }

    inline const bool getVisible () {return _visible;}

    inline void drawStringNumber () {
        for (size_t id=0; id<maxString; id++) {
            edit[id].ReDraw();
            }
         }

     inline void DrawStringNumber(const Orientation &position) {
        edit[position.string].ReDraw();   }
        };


}



#endif /* PULT_INDICATOR_TABLE_NAMECOLUMN_HPP_ */
