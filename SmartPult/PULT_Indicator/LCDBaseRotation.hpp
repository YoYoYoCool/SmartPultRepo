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
#include "Libs/StandartElement/encoder.hpp"
#include "LCDBase.hpp"
#include "Libs/Containers/RingBufferNew.h"

namespace LCD {



class LCDCell {
private:

    Containers::RingBufferN<char*> & buffer;
    bool setup;

public:
    LCD_Cell cell;

    LCDCell(Containers::RingBufferN<char*> & buffer) :setup(false),
                    buffer(buffer){
            }

    void setup () {
        if (setup)
            return;
        text=setting->text;
        cell.p_text=text[encoder.getActualPosition()];
        encoder.setMaxRisk(setting->maxRisk);
        setup=true;
        }

    void stepLeft(Containers::RingBufferN<char*> & buffer) {

        }

    void stepRight(Containers::RingBufferN<char*> & buffer) {

        }

     void updateText () {
         cell.p_text=text[encoder.getActualPosition()];
        }
};

template <uint8_t numberOfCell, uint8_t numberText>
class LCDBase {

private:
    Containers::RingBufferStatic<LCDWorkcEditSettings*,numberText> bufferText[];
    Containers::RingBufferStatic<LCDWorkcEditSettings*,numberText> buffer;
    uint8_t maxSetting;
    StandartElement::Encoder encoderVertical;
    StandartElement::Encoder encoderVerticalText;
    LCDHeader header;
    LCDWorkcEdit    cell[numberOfCell];

public:

    LCDBase(LCDHeaderSettings & settingsHeader,
            LCDWorkcEditSettings** setting,
            uint8_t numberOfCellText):
                header(settingsHeader),
                encoderVertical(numberOfCell-1),
                encoderVerticalText(numberText-1),
                setting(setting){
                for (uint8_t i=0; i<numberOfCell; i++) {
                    cell[i].setup(setting[i]);
                    buffer.write(setting[i]);
                    }
                }

    virtual void stepDown() {
        if (encoderVerticalText.getActualPosition()>encoderVertical.getActualPosition()) {
            for (uint32_t i=0; i<numberOfCell-1; i++) {
                cell[i].cell.p_text=cell[i+1].cell.p_text;
                }
            cell[encoderVertical.getActualPosition()].cell.p_text=buffer.readRight()->text[0];
            drawCell();
            cell[encoderVertical.getActualPosition()].cell.ReDraw();
            return;
            }

        }

    virtual void stepUp() {
        if (this->encoderVertical.getActualPosition()==this->encoderVerticalText.getActualPosition()) {

            }
        }

private:
    void drawCell () {
        for (uint8_t i=0; i<numberOfCell; i++) {
            cell[i].cell.ReHide();   }
        }



    };

}







#endif /* PULT_INDICATOR_LCDBASE_HPP_ */
