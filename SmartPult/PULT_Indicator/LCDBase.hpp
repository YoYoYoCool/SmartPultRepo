/*
 * LCDBase.hpp
 *
 *  Created on: 11 θών. 2019 γ.
 *      Author: User9
 */

#ifndef PULT_INDICATOR_LCDBASE_HPP_
#define PULT_INDICATOR_LCDBASE_HPP_




#include "LCD Classes.h"
#include "../Pult/PultClasses.h"
#include "Libs/StandartElement/galetniy Perecluxhatel.hpp"
#include "Libs/StandartElement/encoder.hpp"

namespace LCD {

const char * pointerRight = "->";
const char * pointerLeft = "<-";


struct LCDHeaderSettings  {

    bool visibleHeader;
    const char* textHeader;
    bool visiblePointerLeft;
    bool visiblePointerRight;
    bool visiblePointerUp;
    bool visiblePointerDown;
    };


class LCDHeader {
private:

        struct CellHeader {
            bool visible;
            LCD_Cell cell;
            };

        CellHeader cellHeader;
        CellHeader cellPointerLeft;
        CellHeader cellPointerRight;
        CellHeader cellPointerUp;
        CellHeader cellPointerDown;

public:
    LCDHeader(LCDHeaderSettings settingsHeader ) {
        t_Pos_Size_XY positionHeader = {    .X=0,  .Y=0, .Xsize=319, .Ysize=30  };

        if (settingsHeader.visiblePointerLeft) {
            cellPointerLeft.visible=settingsHeader.visiblePointerLeft;
            cellPointerLeft.cell.Active_Style = Style_MenuHeader;
            cellPointerLeft.cell.UnActive_Style = Style_MenuHeader;
            t_Pos_Size_XY positionPointerLeft = {    .X=0,  .Y=0, .Xsize=30, .Ysize=30  };
            cellPointerLeft.cell.p_text=(char*)pointerLeft;
            cellPointerLeft.cell.Set_Coord(positionPointerLeft);
            positionHeader.X=positionPointerLeft.Xsize+2;
            positionHeader.Xsize-=positionPointerLeft.Xsize;
        }
        else{
            cellPointerLeft.visible=false;
        }

        if (settingsHeader.visiblePointerRight) {
            cellPointerRight.visible=settingsHeader.visiblePointerRight;
            cellPointerRight.cell.Active_Style = Style_MenuHeader;
            cellPointerRight.cell.UnActive_Style = Style_MenuHeader;
            t_Pos_Size_XY positionPointerRight = {    .X=287,  .Y=0, .Xsize=30, .Ysize=30  };
            cellPointerRight.cell.p_text=(char*)pointerRight;
            cellPointerRight.cell.Set_Coord(positionPointerRight);
            positionHeader.Xsize-=(positionPointerRight.Xsize+2);
        }
        else{
            cellPointerRight.visible=false;
        }

        if (settingsHeader.visibleHeader){
            cellHeader.visible=settingsHeader.visibleHeader;
            cellHeader.cell.Active_Style = Style_MenuHeader;
            cellHeader.cell.UnActive_Style = Style_MenuHeader;
            cellHeader.cell.p_text=(char*)settingsHeader.textHeader;
            cellHeader.cell.Set_Coord(positionHeader);
        }
        else {
            cellHeader.visible=false;
        }

        t_Pos_Size_XY freeDisplayField = {    .X=10,  .Y=30, .Xsize=309, .Ysize=209  };

        if (settingsHeader.visiblePointerUp) {
            cellPointerUp.visible=settingsHeader.visiblePointerUp;
            cellPointerUp.cell.Active_Style = Style_MenuHeader;
                    cellPointerUp.cell.UnActive_Style = Style_MenuHeader;
        }
        else{
            cellPointerUp.visible=false;
        }

        if (settingsHeader.visiblePointerDown) {
            cellPointerDown.visible=settingsHeader.visiblePointerDown;
            cellPointerDown.cell.Active_Style = Style_MenuHeader;
            cellPointerDown.cell.UnActive_Style = Style_MenuHeader;
        }
        else{
            cellPointerDown.visible=false;
        }
    }

    inline void draw () {
        if (cellHeader.visible)
            cellHeader.cell.ReDraw();
        if (cellPointerLeft.visible)
            cellPointerLeft.cell.ReDraw();
        if (cellPointerRight.visible)
            cellPointerRight.cell.ReDraw();
        if (cellPointerUp.visible)
            cellPointerUp.cell.ReDraw();
        if (cellPointerDown.visible)
            cellPointerDown.cell.ReDraw();
        }
};

struct LCDWorkcEditSettings {
    char **text;
    int16_t maxRisk;
    };


class LCDWorkcEdit {
private:

    char **text;
    bool worcShabloConstr;

public:

    StandartElement::Encoder encoder;
    LCD_Cell cell;

    LCDWorkcEdit(LCDWorkcEditSettings * setting) :encoder(setting->maxRisk), text(setting->text),worcShabloConstr(false) {

        }

    LCDWorkcEdit() :worcShabloConstr(true) {

        }

    void setup (LCDWorkcEditSettings * setting) {
        if (!worcShabloConstr)
            return;
        text=setting->text;
        cell.p_text=text[encoder.getActualPosition()];
        encoder.setMaxRisk(setting->maxRisk);
        }

    void stepLeft() {
        encoder.decrement();
        cell.p_text=text[encoder.getActualPosition()];
        cell.ReDraw();
        }

    void stepRight() {
        encoder.increment();
        cell.p_text=text[encoder.getActualPosition()];
        cell.ReDraw();
        }

     void updateText () {
         cell.p_text=text[encoder.getActualPosition()];
        }

};


template <uint8_t numberOfCell>
class LCDBaseNoRotation {
private:

    void calculateInformEdit(t_Pos_Size_XY freeDisplayField) {
        freeDisplayField.Ysize=((freeDisplayField.Ysize-5)-(5*(numberOfCell)))/numberOfCell;
        freeDisplayField.Y+=5;
        for (uint32_t i=0; i<numberOfCell; i++) {
            cell[i].cell.Set_Coord(freeDisplayField);
            freeDisplayField.Y+=freeDisplayField.Ysize;
            freeDisplayField.Y+=5;
            }
        }
    LCDHeader header;
    LCDWorkcEdit cell[numberOfCell];


public:

    StandartElement::Encoder encoderVertical;


    LCDBaseNoRotation(LCDHeaderSettings settingsHeader,LCDWorkcEditSettings** setting): header(settingsHeader), encoderVertical((int16_t)numberOfCell-1) {
        t_Pos_Size_XY freeDisplayField = {
                                          .X=20,
                                          .Xsize=279,
                                          .Ysize=210,
                                          .Y=30
            };
        calculateInformEdit(freeDisplayField);
        for (int8_t i=0; i<numberOfCell; i++) {
            cell[i].setup(setting[i]);
            }
        }

    void drawForm() {
        header.draw();
        for (uint8_t i=0; i<numberOfCell; i++) {
            cell[i].cell.ReHide();   }
        cell[encoderVertical.getActualPosition()].cell.ReDraw();
    }

    void stepDown() {
        cell[encoderVertical.getActualPosition()].cell.ReHide();
        encoderVertical.increment();
        cell[encoderVertical.getActualPosition()].cell.ReDraw();
        }

    void stepUp() {
        cell[encoderVertical.getActualPosition()].cell.ReHide();
        encoderVertical.decrement();
        cell[encoderVertical.getActualPosition()].cell.ReDraw();
        }

    void stepLeft() {
        cell[encoderVertical.getActualPosition()].stepLeft();
        }

    void stepRight() {
        cell[encoderVertical.getActualPosition()].stepRight();
        }

    void setGorizontalEncoder(int8_t id, int16_t count) {
        cell[id].encoder.setActualPosition(count);
        cell[id].updateText();
        }

    int8_t getCellEncoderPosition() {
        return cell[encoderVertical.getActualPosition()].encoder.getActualPosition();
        }

};




}







#endif /* PULT_INDICATOR_LCDBASE_HPP_ */
