/*
 * Table.hpp
 *
 *  Created on: 22 мар. 2019 г.
 *      Author: User9
 */

#ifndef PULT_INDICATOR_TABLE_TABLE_HPP_
#define PULT_INDICATOR_TABLE_TABLE_HPP_

#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "LCD Classes.h"
#include "Libs/Utils/Debug.hpp"
#include "TableText.hpp"
#include "TableData.hpp"

namespace Table {

static enum {
    sizeDispX=319,
    sizeDispY=239
};


template <typename TData,size_t maxString,size_t maxColumn,size_t sizeStringData, size_t sizeColumnData,size_t longStr>
class Table {
private:
    LCD_Cell edit[maxString][maxColumn];
    TableData <TData,sizeStringData,sizeColumnData> editData;
    TableText <sizeStringData,sizeColumnData,longStr> editText;

public:

    Table() {
        for (size_t idStr=0; idStr<maxString-1; idStr++) {
            for (size_t idColumn=0; idColumn<maxColumn-1; idColumn++) {
                edit[idStr][idColumn].p_text=&editText.string(idStr, idColumn)[0];
                }
            }
        }

    inline void scrollLeft(const OrientationTable &position) {
        for (size_t idStr=maxString-1; idStr>0; idStr--) {
            for (size_t idColumn=maxColumn-1;idColumn>0;idColumn--) {
                edit[idStr][idColumn].p_text=edit[idStr][idColumn-1].p_text;
                }
            }
        Orientation positionTableText=position.minPositionTableData;
        for (size_t idStr=0; idStr<maxString; idStr++) {
            edit[idStr][0].p_text=&editText.string(positionTableText)[0];
            positionTableText.string++;
            }
        }

    inline void scrollRight (const OrientationTable &position) {
        for (size_t idStr=0; idStr<maxString-1; idStr++) {
            for (size_t idColumn=0;idColumn<maxColumn-1;_position.column++) {
                edit[idStr][idColumn].p_text=edit[idStr][idColumn].p_text;
                }
            }
        Orientation positionTableText=position.maxPositionTableData;
        for (int32_t idStr=maxString-1;idStr>-1; idStr--) {
            edit[_idStr][maxColumn-1].p_text=&editText.string(positionTableText)[0];
            positionTableText.string--;
            }
        }

    inline void scrollUp (const OrientationTable &position) {
        for (int32_t idStr=maxString-1; idStr>0; idStr--) {
            for (size_t idColumn=0; idColumn<maxColumn-1; idColumn++) {
                edit[idStr][idColumn].p_text=edit[idStr][idColumn].p_text;
                }
            }
        Orientation positionTableText= position.minPositionTableData;
        for (size_t idColumn=0; idColumn<maxColumn; idColumn++) {
            edit[0][idColumn].p_text=&editText.string(positionTableText)[0];
            positionTableText.column++;
        }
    }

    inline void scrollDown (const OrientationTable &position) {
        for (int32_t idStr=0; idStr<maxString-1; idStr++) {
            for (int32_t idColumn=0; idColumn<maxColumn; idColumn++) {
                edit[idStr][idColumn].p_text=edit[idStr+1][idColumn].p_text;
                }
            }
        Orientation positionTableText= position.maxPositionTableData;
        for (int32_t idColumn=maxColumn-1; idColumn>-1; idColumn--)
            {
            edit[maxString-1][idColumn].p_text=&editText.string(positionTableText)[0];
            positionTableText.column--;
            }
        }

    inline void highLight(const OrientationTable &position) {
        edit[position.positionTable.string][position.positionTable.column].UnActive_Style.Cell_Color=ClrYellow;
        }

    inline void downLight (const OrientationTable &position) {
        edit[position.positionTable.string][position.positionTable.column].UnActive_Style.Cell_Color=ClrMediumOrchid;
        }

    inline void selectEdit (const OrientationTable &position) {
        edit[position.positionTable.string][position.positionTable.column].UnActive_Style.Border_Color=ClrLime;
        }

    inline void deSelectEdit (const OrientationTable &position) {
        edit[position.positionTable.string][position.positionTable.column].UnActive_Style.Border_Color=ClrWhite;
        }

    inline void setData(const OrientationTable &position, TData data) {
        editData.setTableData(position.positionTableData, data);
        printData(position,editData.getTableData(position.positionTableData));
        edit[position.positionTable.string][position.positionTable.column].p_text=&editText.string(position.positionTableData)[0];
        edit[position.positionTable.string][position.positionTable.column].ReDraw();
        }

    inline void setAllCoord (t_Pos_Size_XY & coord) {
        uint32_t startPositionDispX=coord.Xsize;
        coord.Xsize=(sizeDispX-coord.X)/maxColumn;
        coord.Ysize=(sizeDispY-coord.Y)/maxString;
        for (uint32_t idStr=0; idStr<maxString;idStr++) {
            for (uint32_t idColumn=0; idColumn<maxColumn; idColumn++) {
                edit[idStr][idColumn].Set_Coord(coord);
                coord.X+=coord.Xsize;
                }
            coord.X=startPositionDispX;
            coord.Y+=coord.Ysize;
            }
        }

    inline TData getData(const OrientationTable &position) {
        return editData.getTableData(position.positionTableData);        }

private:

    inline void printData(float data,const OrientationTable &position) {
        sprintf(&editText.string(position.positionTableData)[0],"%6.1f",data);    }

    inline void printData(uint32_t data, const OrientationTable &position) {
        sprintf(&editText.string(position.positionTableData)[0],"%u",data);    }

    inline void printData(int32_t data, const OrientationTable &position) {
        sprintf(&editText.string(position.positionTableData)[0],"%d",data);     }

    };


}



#endif /* PULT_INDICATOR_TABLE_TABLE_HPP_ */
