/*
 * TableForm.hpp
 *
 *  Created on: 22 мар. 2019 г.
 *      Author: Cheverev Kirill
 */

#ifndef PULT_INDICATOR_TABLE_TABLEFORM_HPP_
#define PULT_INDICATOR_TABLE_TABLEFORM_HPP_

#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "LCD Classes.h"
#include "Libs/Utils/Debug.hpp"
#include "TableText.hpp"
#include "TableData.hpp"
#include "NameColumn.hpp"
#include "LineNumber.hpp"
#include "Table.hpp"

namespace Table {

enum {xStartPositionNumber=0};
enum {yStartPositionNumber=0};
enum {ySyzeAllEditForms=9};
enum {xSizeStringNumber=19};
enum {ySizeColumNumber=9};
enum {ySizeCellNumber=19};
enum {maxXSize=319};
enum {xSizeDisplay=319};
enum {ySizeDisplay=239};

t_Pos_Size_XY posHead = {
              .X= xStartPositionNumber,
              .Xsize=maxXSize,
              .Y=yStartPositionNumber,
              .Ysize=ySyzeAllEditForms
};

struct Orientation {
    int32_t string;
    int32_t column;
};

struct OrientationTable {
    Orientation positionTable;
    Orientation minPositionTableData;
    Orientation maxPositionTableData;
    Orientation positionTableData;  };



template <typename TData,size_t maxString,size_t maxColumn,
size_t sizeStringData, size_t sizeColumnData, size_t longStr>
class TableForm  {
private:
    LCD_Cell headLine;
    OrientationTable position;
    LineNumber <maxString,maxColumn,sizeStringData,sizeColumnData,longStr> lineNumber;
    NameColumn<maxColumn,sizeColumnData,longStr> _nameColumn;
    Table<TData,maxString,maxColumn,sizeStringData, sizeColumnData, longStr> table;
    bool focus, visibleHeadLine;
    t_Pos_Size_XY sizeDisp;

public:
    TableForm(char * textHeadLine,
              bool focus=false,
              bool visibleLineNumber=true,
              bool visibleNameColumn=true,
              bool visibleHeadline=true,
              tMenu_Link * nameColumn ):
        headLine(textHeadLine,posHead),
        focus(true),
        visibleHeadLine(visibleHeadline),
        lineNumber(this->visibleLineNumber),
        _nameColumn(nameColumn,this->visibleNameColumn)        {
                }

    inline void creatForm() {
        t_Pos_Size_XY coord= {
            .X=xStartPositionNumber,
            .Y=yStartPositionNumber,
            .Xsize=0,
            .Ysize=0
        };
        if (visibleHeadLine)        {
            coord.Y+=ySyzeAllEditForms; //если заголовок видим то сдвигаем стартовую пазицию
            }
        if (_nameColumn.getVisible()) {
            if (lineNumber.getVisible())  {
                coord.Xsize=xSizeStringNumber;
                coord.Ysize=ySizeCellNumber;
                _nameColumn.setAllCoord(coord);
                coord.X=xStartPositionNumber;
                coord.Xsize=xSizeStringNumber;
                coord.Y=29;
                lineNumber.setPositionString(coord);
                coord.X=19;
                coord.Y=19;
                coord.Ysize=ySizeColumNumber;
                lineNumber.setPositionColumn(coord);
                coord.X=19;
                coord.Y=29;
                }
            else {
                coord.Ysize=0;
                coord.Xsize=0;
                _nameColumn.setAllCoord(coord);
                coord.X=0;
                coord.Y=19;
                }
            }
        else {
            if (lineNumber.getVisible())  {
                coord.Ysize=ySizeColumNumber;
                lineNumber.setPositionColumn(coord);
                coord.X=0;
                coord.Y=19;
                }
            }
        table.setAllCoord(coord);
        }

private:

    };


}



#endif /* PULT_INDICATOR_TABLE_TABLEFORM_HPP_ */
