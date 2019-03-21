/*
 * LCD Lens.hpp
 *
 *  Created on: 7 мар. 2019 г.
 *      Author: Cheverev Kirill
 */

#ifndef PULT_INDICATOR_LCD_LENS_HPP_
#define PULT_INDICATOR_LCD_LENS_HPP_
/*
#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "LCD Classes.h"
#include "LensParam/LensData.hpp"
#include "Libs/Containers/String.hpp"
#include "Libs/Containers/StringStatic.hpp"
#include "Libs/Utils/Debug.hpp"

enum {xStartPositionNumber=1};
enum {yStartPositionNumber=1};
enum {ySyzeAllEditForms=10};
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
    size_t string;
    size_t column;
};

struct OrientationTable {
    Orientation positionTable;
    Orientation minPositionTableData;
    Orientation maxPositionTableData;
    Orientation positionTableData;
};

template <typename TData,size_t sizeStringData, size_t sizeColumnData>
class TableData {
private:
    TData tableData[sizeStringData][sizeColumnData];
public:
    TableEdit() {
        for (size_t idStr=0; idStr<sizeStringData; idStr++) {
            for (size_t idColumn=0; idColumn<sizeColumnData; idColumn++) {
                tableData[idStr][idColumn]=0;
            }
        }
    }

    inline void setTableData (const Orientation & position, const TData data) {
        tableData[position.string][position.column]=data;
    }

    inline const TData getTableData (const Orientation & position) {
        return tableData[position.string][position.column];
    }


};


template <size_t sizeStringData, size_t sizeColumnData, size_t longStr>
class TableText {
private:
    Containers::StringStatic<longStr> _string[sizeStringData][sizeColumnData];
public:

    TableText () {
        for (size_t idStr=0; idStr<sizeStringData; idStr++) {
            for (size_t idColumn=0; idColumn<sizeColumnData; idColumn++) {
             _string[idStr][idColumn].setChar(0,'0');
             _string[idStr][idColumn].setChar(1,'.');
             _string[idStr][idColumn].setChar(2,'0');
             _string[idStr][idColumn].setChar(3,'\0');
            }
        }
    }

    inline Containers::String & string (const Orientation & position) {
        return _string[position.string][position.column];
        }
    inline Containers::String & string (size_t string, size_t column) {
        return _string[string][column];
        }
    };

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

    inline void setVisible (bool visible) { this->_visible=visible;    }

    inline const bool& visible () {return _visible;}

    inline void setCoord (Orientation &position, t_Pos_Size_XY & coord ) {
        edit[position.column].Set_Coord(coord);  }

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

    inline void drawStringNumber () {
        for (size_t id=0; id<maxString; id++) {
            edit[id].ReDraw();
            }
        }

    inline void dinglDrawStringNumber( const Orientation &position) {
        edit[position.string].ReDraw();   }
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

    inline void scrollRight(const Orientation &position) {
        for (size_t id=1; id<maxColumn-1;is--)
            edit[id].p_text=edit[id+1].p_text;
        edit[maxColumn-1].p_text=&text.string(0, position.column)[0];
        }

    inline void setVisible (bool visible) { this->_visible=visible;    }

    inline const bool& visible () {return _visible;}

    inline void drawStringNumber () {
        for (size_t id=0; id<maxString; id++) {
            edit[id].ReDraw();
            }
         }

     inline void dinglDrawStringNumber(const Orientation &position) {
        edit[position.string].ReDraw();   }
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

const Orientation sizeDisplay= {
     .column=239,
     .string=319
    };


template <typename TData,size_t maxString,size_t maxColumn,
size_t sizeStringData, size_t sizeColumnData, size_t longStr>
class TableForm: public LCD_Listener  {
private:
    LCD_Cell headLine;
    OrientationTable position;
    ColumnNumber<maxColumn, sizeColumnData, longStr> columnNumber;
    StringNumber<maxString, sizeStringData, longStr> stringNumber;
    NameColumn<maxColumn,sizeColumnData,longStr> _nameColumn;
    Table<TData,maxString,maxColumn,sizeStringData, sizeColumnData, longStr> table;
    bool focus, visibleColumnNumber, visibleStringNumber, visibleNameColumn, visibleHeadLine;
    t_Pos_Size_XY sizeDisp;

public:
    TableForm(char * textHeadLine,bool focus=false, bool visibleColumnNumber=true,
              bool visibleStringNumber=true, bool visibleNameColumn=true,
              bool visibleHeadline=true,tMenu_Link * nameColumn ):
        headLine(textHeadLine,posHead),
        focus(true),
        visibleColumnNumber(visibleColumnNumber),
        visibleStringNumber(visibleStringNumber),
        visibleNameColumn(visibleNameColumn),
        visibleHeadLine(visibleHeadline),
        columnNumber(this->visibleColumnNumber),
        stringNumber(this->visibleStringNumber),
        _nameColumn(nameColumn,this->visibleNameColumn)        {
        creatForm();        }

    inline void creatForm() {
        t_Pos_Size_XY sizeDisp= {
            .X=xStartPositionNumber,
            .Y=yStartPositionNumber,
            .Xsize=xSizeDisplay,
            .Ysize=ySizeDisplay };

        }

virtual void Listener() {
    if (this->Focused) {
        ClearDisp();
        draw();
        }
    if (getButtonState(pult_Button_Select)) {
        select();
        }
    if (getButtonState(pult_Button_Up)) {
        if (focus) plus();
        else   stepUp();
        }
    if (getButtonState(pult_Button_Dn)) {
        if (focus) minus();
        else  stepDown();
        }
    if (getButtonState(pult_Button_Left)) {
        if (!focus)
            stepLeft();
        }
    if (getButtonState(pult_Button_Right)) {
        if (!focus)
            stepRight();
        }
    if (getButtonState(pult_Button_ESC)) {
        esc();
        }
}

private:

    inline void draw() {

        }

    inline void constrTable() {

        }

    inline void plus () {

        }

    inline void minus() {

        }

    inline void resetText() {

        }

    inline void activeEdit() {

        }

    inline void unActiveEdit() {

        }

    inline void stepLeft () {

        }

    inline void stepRight () {

        }

    inline void stepUp () {

        }

    inline void stepDown () {

        }

    inline void scrollUp () {

        }

    inline void scrollDown () {

        }

    inline void select() {

        }

    inline void esc () {

        }


    };



*/

#endif /* PULT_INDICATOR_LCD_LENS_HPP_ */
