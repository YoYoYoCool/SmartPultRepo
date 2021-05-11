/*
 * ShakerForm.hpp
 *
 *  Created on: 13 мая 2020 г.
 *      Author: Кирилл Чеверев
 */

#ifndef PULT_INDICATOR_SHAKERFORM_HPP_
#define PULT_INDICATOR_SHAKERFORM_HPP_




#include "LCD Classes.h"
#include "LCDBase.hpp"
#include <InternalDevice/shakerInternalChannel.hpp>
#include <InternalDevice/shakerSinInternalChannel.hpp>
#include "Libs/StandartElement/encoder.hpp"
#include "Libs/StandartElement/RNDGenerator.hpp"


namespace LCD {

    namespace UpdateValue {

        union BlockingValue {
            uint32_t all;
            struct {
                uint8_t panRNDShakerBlocking            :1;

                uint8_t tiltRNDShakerBlocking           :1;
                uint8_t rollRNDShakerBlocking           :1;
                uint8_t zoomRNDShakerBlocking           :1;
                uint8_t RNDShakerHardBlocking           :1;
                uint8_t panSinShakerBlocking            :1;
                uint8_t tiltSinShakerBlocking           :1;
                uint8_t rollSinShakerBlocking           :1;
                uint8_t zoomSinShakerBlocking           :1;
                uint8_t SinShakerHardBlocking           :1;
                uint8_t panShakerBoxBlocking            :1;
                uint8_t tiltShakerBoxBlocking           :1;
                uint8_t rollShakerBoxBlocking           :1;
                uint8_t zoomShakerBoxBlocking           :1;
                uint8_t ShakerBoxHardBlocking           :1;
                uint32_t NU                             :17;

            }BlockingBit;
        };

        static BlockingValue blockingValue;

        void readBlocking () {
            blockingValue.all=EE_Working::Read(EE_LC_SHAKER_BLOCKING);
            if (blockingValue.all==0xFFFFFFFF) {
                blockingValue.all=0x00007fff;
                EE_Working::Write(EE_LC_SHAKER_BLOCKING, blockingValue.all);
            }
            if (!blockingValue.BlockingBit.RNDShakerHardBlocking) {
                blockingValue.BlockingBit.SinShakerHardBlocking=1;
                EE_Working::Write(EE_LC_SHAKER_BLOCKING, blockingValue.all);
            }
            if (!blockingValue.BlockingBit.SinShakerHardBlocking) {
                blockingValue.BlockingBit.RNDShakerHardBlocking=1;
                EE_Working::Write(EE_LC_SHAKER_BLOCKING, blockingValue.all);
            }


        }

        class IUpdateValue {

        public:

            virtual void upCounter() =0;

            virtual void downCounter() =0;

        };

        class UpdateUpDownBtn: public IUpdateValue {

        private:

            float &value;
            PultButton& upBtn;
            PultButton& downBtn;
            uint8_t stepColunter;

        public:

            UpdateUpDownBtn(PultButton& upBtn,PultButton& downBtn, float& value): value(value),
            upBtn(upBtn),downBtn(downBtn)
        {

        }

            virtual void upCounter() {
                if (upBtn.isPressed()) {
                    value+=0.01;
                }


            }

            virtual void downCounter() {
                if (downBtn.isPressed()) {
                    value-=0.01;
                }
            }

        };

    }

    namespace ElementShakerForm {

        class IAxisSetting {

        public:

            virtual void work() =0;

        };

        class AxisCell:public IAxisSetting {

        private:

            LCD_Cell cell;
            char text[10];
            UpdateValue::UpdateUpDownBtn update;
            float& value;
            float valueOldStep, minValue,maxValue;
            uint8_t adresValue;

        public:

            AxisCell(PultButton& upBtn,PultButton& downBtn, float& value, uint8_t adresValue, float minValue,float maxValue):update(upBtn,downBtn,value),
            value(value),adresValue(adresValue),valueOldStep(0.0),minValue(minValue),maxValue(maxValue) {
                cell.SetText(&text[0]);
                readEEprom();
            }

            virtual void work() {
                valueOldStep=value;
                update.upCounter();
                if (value>maxValue)
                    value=maxValue;
                update.downCounter();
                if (value<minValue)
                    value=minValue;
                if(valueOldStep!=value) {
                    clearString();
                    sprintf(&text[0],"%2.2f",value);
                    EE_Working::Write(adresValue, *((uint32_t*)&value));
                    cell.ReDraw();
                }

            }

            void readEEprom () {
                uint32_t tmp = EE_Working::Read(adresValue);
                clearString();
                if (tmp==0xffffffff){
                    tmp=*((uint32_t*)(&minValue));
                    EE_Working::Write(adresValue, tmp);
                }
                value=*((float*)(&tmp));
                if ((value<minValue)||(value>maxValue)) {
                    value=minValue;
                    EE_Working::Write(adresValue, *((uint32_t*)&value));
                }
                sprintf(&text[0],"%2.2f",value);
            }

            ILCD_Cell & _cell() { return cell;}

            void setText (char * text) {    cell.SetText(text);    }

            void setCoord (t_Pos_Size_XY coord) {   cell.Set_Coord(coord);     }

        private :

            void clearString() {
                for (uint8_t i=0;i<10;i++)
                    text[i]=0;

            }

        };
    }

    namespace Form {

#ifdef USAEdition
    enum {
        tableCellLine=0,panLine, tiltLine, rollLine, zoomLine,offsetLine
    };


#else
    enum {
        tableCellLine=0,panLine, rollLine, tiltLine, zoomLine,offsetLine
        };
#endif

    enum {axisNameCell=0,speedCell, lpfCell,hpfCell,offsetColumn};

        class ShakerMenu:public LCD_Menu {
        public:

            ShakerMenu(char* pNam, tMenu_Link* Link, byte Count, byte Orient, byte Menu_Per_Scr):
                LCD_Menu(pNam,Link,Count,Orient,Menu_Per_Scr)
                {
                readEEPROM();
                }

            virtual void readEEPROM() {
                UpdateValue::readBlocking();
                Table_Cell[0]->Active_Style.Cell_Color = ClrDarkSlateBlue;
                Table_Cell[0]->UnActive_Style.Cell_Color = ClrDarkSlateBlue;
                Table_Cell[1]->Active_Style.Cell_Color = ClrDarkSlateBlue;
                Table_Cell[1]->UnActive_Style.Cell_Color = ClrDarkSlateBlue;

                if (UpdateValue::blockingValue.BlockingBit.RNDShakerHardBlocking) {
                    Table_Cell[0]->Active_Style.Cell_Color = ClrLinen;
                    Table_Cell[0]->UnActive_Style.Cell_Color = ClrLinen;
                }

                if (LCD::UpdateValue::blockingValue.BlockingBit.SinShakerHardBlocking) {
                    Table_Cell[1]->Active_Style.Cell_Color = ClrLinen;
                    Table_Cell[1]->UnActive_Style.Cell_Color = ClrLinen;
                    }
            }
        };

        tMenu_Link link[8] = {
                              {"PAN GAIN:", NULL},
                              {"TILT GAIN:", NULL},
#ifdef USAEdition
                              {"ROLL GAIN:", NULL},
#else
                              {"DUTCH GAIN:", NULL},
#endif
                              {"ZOOM GAIN:", NULL},
                              {"PAN FRIQ HPF:", NULL},
                              {"TILT FRIQ HPF:", NULL},
#ifdef USAEdition
                              {"ROLL FRIQ HPF:", NULL},
#else
                              {"DUTCH FRIQ HPF:", NULL},
#endif
                              {"ZOOM FRIQ HPF:", NULL},
        };

        enum  { panGainAxis=0,tiltGainAxis,rollGainAxis,zoomGainAxis,panHPFAxis,tiltHPFAxis,rollHPFAxis,zoomHPFAxis};

        static char* textHeader ="RND SHAKER";

        class ShakerRNDMenu:public LCD_Menu {
        private:

            IShaker * axisShaker[offsetLine];
            Generation::IGenerator * generator[offsetLine];
            uint32_t colorCell[2];
            float gainPan,gainTilt,gainRoll,gainZoom,hpfPan,hpfTilt,hpfRoll,hpfZoom;
            float *data[8];
            float maxGainValue;
            float maxFriqValue;
            char text[8][30];
            uint8_t buffer[8];

        public:
            ShakerRNDMenu():LCD_Menu(textHeader,link,8,0,8),maxGainValue(6.0),maxFriqValue(1000.0)
            {
                Cell_Header.SetText(textHeader);
                axisShaker[panGainAxis]=p_pult->_panAxisShaker();
                axisShaker[tiltGainAxis]=p_pult->_tiltAxisShaker();
                axisShaker[rollGainAxis]=p_pult->_rollAxisShaker();
                axisShaker[zoomGainAxis]=p_pult->_zoomAxisShaker();
                Table_Cell[panGainAxis]->p_text=text[0];
                Table_Cell[tiltGainAxis]->p_text=text[1];
                Table_Cell[rollGainAxis]->p_text=text[2];
                Table_Cell[zoomGainAxis]->p_text=text[3];
                Table_Cell[panHPFAxis]->p_text=text[4];
                Table_Cell[tiltHPFAxis]->p_text=text[5];
                Table_Cell[rollHPFAxis]->p_text=text[6];
                Table_Cell[zoomHPFAxis]->p_text=text[7];
                data[panGainAxis]=&gainPan;
                data[tiltGainAxis]=&gainTilt;
                data[rollGainAxis]=&gainRoll;
                data[zoomGainAxis]=&gainZoom;
                data[panHPFAxis]=&hpfPan;
                data[tiltHPFAxis]=&hpfTilt;
                data[rollHPFAxis]=&hpfRoll;
                data[zoomHPFAxis]=&hpfZoom;
                buffer[panGainAxis]=9;
                buffer[tiltGainAxis]=10;
#ifdef USAEdition
                buffer[rollGainAxis]=10;
#else
                buffer[rollGainAxis]=11;
#endif
                buffer[zoomGainAxis]=10;
                buffer[panHPFAxis]=13;
                buffer[tiltHPFAxis]=14;
#ifdef USAEdition
                buffer[rollHPFAxis]=15;
#else
                buffer[rollHPFAxis]=14;
#endif
                buffer[zoomHPFAxis]=14;
                colorCell[0] = ClrDarkSlateBlue;
                colorCell[1] = ClrLinen;
                readEEPROM();
                for (uint8_t i=0; i<4;i++) {
                    uint32_t index = UpdateValue::blockingValue.all;
                    index>>=i;
                    index&=0x1;
                    axisShaker[i]->setBlokingMode((bool)index);
                    Table_Cell[i]->Active_Style.Cell_Color=colorCell[index];
                    Table_Cell[i]->UnActive_Style.Cell_Color=colorCell[index];
                }
            }

            virtual void Listener() {
                if(Focused)
                    {
                        ClearDisp();
                        DrawHeader();
                        DrawVert();
                        Focused = false;
                    }

                if (getButtonState(pult_Button_Select) == PRESSED)
                    {
                    if (Tek_Count<5)
                        {
                        uint8_t index = this->Tek_Count;
                        index-=1;
                        UpdateValue::blockingValue.all^=1<<index;
                        uint32_t colorIndex = UpdateValue::blockingValue.all;
                        colorIndex>>=index;
                        colorIndex&=1;
                        Table_Cell[index]->Active_Style.Cell_Color=colorCell[colorIndex];
                        Table_Cell[index]->UnActive_Style.Cell_Color=colorCell[colorIndex];
                        Table_Cell[index]->ReDraw();
                        axisShaker[Tek_Count-1]->setBlokingMode((bool)colorIndex);
                        EE_Working::Write(EE_LC_SHAKER_BLOCKING, UpdateValue::blockingValue.all);
                        }
                    }

                if ((getButtonState(pult_Button_Left) == PRESSED)||(getButtonState(pult_Button_Left) == HOLD))
                {

                    if (Tek_Count<5)
                        {
                        *data[Tek_Count-1]-=0.01;
                        if (*data[Tek_Count-1]<0.0)
                            *data[Tek_Count-1]=0.0;
                        axisShaker[Tek_Count-1]->_resistor().setValue(*data[Tek_Count-1]);
                        }
                    else
                        {
                        if(*data[Tek_Count-1]<=1.0)
                            *data[Tek_Count-1]-=0.01;
                        else if(*data[Tek_Count-1]<=10.0)
                            *data[Tek_Count-1]-=0.1;
                        else
                            *data[Tek_Count-1]-=1.0;
                        if (*data[Tek_Count-1]<0.03)
                            *data[Tek_Count-1]=0.03;
                        axisShaker[Tek_Count-5]->getFilter().setFriquensyLeftFilter(1/(*data[Tek_Count-1]));
                        }
                    sprintf(&text[Tek_Count-1][buffer[Tek_Count-1]],"%2.2f",*data[Tek_Count-1]);
                    Table_Cell[Tek_Count-1]->ReDraw();
                    EE_Working::Write(EE_LC_INTERNAL_SHAKER_PAN_SPEED+Tek_Count-1, *((uint32_t*)data[Tek_Count-1]));
                }

                if ((getButtonState(pult_Button_Right) == PRESSED)||(getButtonState(pult_Button_Right) == HOLD))
                {
                    if (Tek_Count<5)
                        {
                        *data[Tek_Count-1]+=0.01;
                        if (*data[Tek_Count-1]>maxGainValue)
                            *data[Tek_Count-1]=maxGainValue;
                        axisShaker[Tek_Count-1]->_resistor().setValue(*data[Tek_Count-1]);
                        }
                    else
                        {
                        if(*data[Tek_Count-1]<=1.0)
                            *data[Tek_Count-1]+=0.01;
                        else if(*data[Tek_Count-1]<=10.0)
                            *data[Tek_Count-1]+=0.1;
                        else
                            *data[Tek_Count-1]+=1.0;
                        if (*data[Tek_Count-1]>maxFriqValue)
                            *data[Tek_Count-1]=maxFriqValue;
                        axisShaker[Tek_Count-5]->getFilter().setFriquensyLeftFilter(1/(*data[Tek_Count-1]));
                        }
                    sprintf(&text[Tek_Count-1][buffer[Tek_Count-1]],"%2.3f",*data[Tek_Count-1]);
                    Table_Cell[Tek_Count-1]->ReDraw();
                    EE_Working::Write(EE_LC_INTERNAL_SHAKER_PAN_SPEED+Tek_Count-1, *((uint32_t*)data[Tek_Count-1]));
                }

                if (getButtonState(pult_Button_Up) == PRESSED)
                {
                    Minus();
                }

                if (getButtonState(pult_Button_Dn) == PRESSED)
                {
                    Plus();
                }

                if (getButtonState(pult_Button_ESC) == PRESSED)
                    {
                        pDispl = pDispl->Parent;
                        pDispl->Focused = true;
                        return;
                    }
            }

            inline void readEEPROM() {
                uint32_t tmp = EE_Working::Read(EE_LC_INTERNAL_SHAKER_PAN_SPEED);
                if (tmp==0xffffffff)
                {
                    gainPan=0.0;
                    tmp=*((uint32_t*)(&gainPan));
                    EE_Working::Write(EE_LC_INTERNAL_SHAKER_PAN_SPEED, tmp);
                }
                gainPan=*((float*)(&tmp));
                if ((gainPan<0.0)||(gainPan>maxGainValue))
                {
                    gainPan=0.0;
                    EE_Working::Write(EE_LC_INTERNAL_SHAKER_PAN_SPEED, *((uint32_t*)&gainPan));
                }
                sprintf(text[0],"PAN GAIN:%2.2f",gainPan);
                axisShaker[0]->_resistor().setValue(gainPan);

                tmp = EE_Working::Read(EE_LC_INTERNAL_SHAKER_TILT_SPEED);
                if (tmp==0xffffffff)
                {
                    gainTilt=0.0;
                    tmp=*((uint32_t*)(&gainTilt));
                    EE_Working::Write(EE_LC_INTERNAL_SHAKER_TILT_SPEED, tmp);
                }
                gainTilt=*((float*)(&tmp));
                if ((gainTilt<0.0)||(gainTilt>maxGainValue))
                {
                    gainTilt=0.0;
                    EE_Working::Write(EE_LC_INTERNAL_SHAKER_TILT_SPEED, *((uint32_t*)&gainTilt));
                }
                sprintf(text[1],"TILT GAIN:%2.2f",gainTilt);
                axisShaker[1]->_resistor().setValue(gainTilt);

                tmp = EE_Working::Read(EE_LC_INTERNAL_SHAKER_ROLL_SPEED);
                if (tmp==0xffffffff)
                {
                    gainRoll=0.0;
                    tmp=*((uint32_t*)(&gainRoll));
                    EE_Working::Write(EE_LC_INTERNAL_SHAKER_ROLL_SPEED, tmp);
                }
                gainRoll=*((float*)(&tmp));
                if ((gainRoll<0.0)||(gainRoll>maxGainValue))
                {
                    gainRoll=0.0;
                    EE_Working::Write(EE_LC_INTERNAL_SHAKER_ROLL_SPEED, *((uint32_t*)&gainRoll));
                }
#ifdef USAEdition
                sprintf(text[2],"ROLL GAIN:%2.2f",gainRoll);
#else
                sprintf(text[2],"DUTCH GAIN:%2.2f",gainRoll);
#endif
                axisShaker[2]->_resistor().setValue(gainRoll);

                tmp = EE_Working::Read(EE_LC_INTERNAL_SHAKER_ZOOM_SPEED);
                if (tmp==0xffffffff)
                {
                    gainZoom=0.0;
                    tmp=*((uint32_t*)(&gainZoom));
                    EE_Working::Write(EE_LC_INTERNAL_SHAKER_ZOOM_SPEED, tmp);
                }
                gainZoom=*((float*)(&tmp));
                if ((gainZoom<0.0)||(gainZoom>maxGainValue))
                {
                    gainZoom=0.0;
                    EE_Working::Write(EE_LC_INTERNAL_SHAKER_ZOOM_SPEED, *((uint32_t*)&gainZoom));
                }
                sprintf(text[3],"ZOOM GAIN:%2.2f",gainZoom);
                axisShaker[3]->_resistor().setValue(gainZoom);

                tmp = EE_Working::Read(EE_LC_INTERNAL_SHAKER_PAN_HPF);
                if (tmp==0xffffffff)
                {
                    hpfPan=0.01;
                    tmp=*((uint32_t*)(&hpfPan));
                    EE_Working::Write(EE_LC_INTERNAL_SHAKER_PAN_HPF, tmp);
                }
                hpfPan=*((float*)(&tmp));
                if ((hpfPan<0.03)||(gainPan>maxFriqValue))
                {
                    hpfPan=0.03;
                    EE_Working::Write(EE_LC_INTERNAL_SHAKER_PAN_HPF, *((uint32_t*)&hpfPan));
                }
                sprintf(text[4],"PAN FRIQ HPF:%2.3f",hpfPan);
                axisShaker[0]->getFilter().setFriquensyLeftFilter(hpfPan);

                tmp = EE_Working::Read(EE_LC_INTERNAL_SHAKER_TILT_HPF);
                if (tmp==0xffffffff)
                {
                    hpfTilt=0.03;
                    tmp=*((uint32_t*)(&hpfTilt));
                    EE_Working::Write(EE_LC_INTERNAL_SHAKER_TILT_HPF, tmp);
                }
                hpfTilt=*((float*)(&tmp));
                if ((hpfTilt<0.03)||(hpfTilt>maxFriqValue))
                {
                    hpfPan=0.03;
                    EE_Working::Write(EE_LC_INTERNAL_SHAKER_TILT_HPF, *((uint32_t*)&hpfTilt));
                }
                sprintf(text[5],"TILT FRIQ HPF:%2.3f",hpfTilt);
                axisShaker[1]->getFilter().setFriquensyLeftFilter(hpfTilt);

                tmp = EE_Working::Read(EE_LC_INTERNAL_SHAKER_ROLL_HPF);
                if (tmp==0xffffffff)
                {
                    hpfRoll=0.03;
                    tmp=*((uint32_t*)(&hpfRoll));
                    EE_Working::Write(EE_LC_INTERNAL_SHAKER_ROLL_HPF, tmp);
                }
                hpfRoll=*((float*)(&tmp));
                if ((hpfRoll<0.03)||(hpfRoll>maxFriqValue))
                {
                    hpfRoll=0.03;
                    EE_Working::Write(EE_LC_INTERNAL_SHAKER_ROLL_HPF, *((uint32_t*)&hpfRoll));
                }
#ifdef USAEdition
                sprintf(text[6],"ROLL FRIQ HPF:%2.3f",hpfRoll);
#else
                sprintf(text[6],"DUTCH FRIQ HPF:%2.3f",hpfRoll);
#endif

                axisShaker[2]->getFilter().setFriquensyLeftFilter(hpfRoll);

                tmp = EE_Working::Read(EE_LC_INTERNAL_SHAKER_ZOOM_HPF);
                if (tmp==0xffffffff)
                {
                    hpfZoom=0.03;
                    tmp=*((uint32_t*)(&hpfZoom));
                    EE_Working::Write(EE_LC_INTERNAL_SHAKER_ZOOM_HPF, tmp);
                }
                hpfZoom=*((float*)(&tmp));
                if ((hpfZoom<0.03)||(hpfZoom>maxFriqValue))
                {
                    hpfZoom=0.03;
                    EE_Working::Write(EE_LC_INTERNAL_SHAKER_ZOOM_HPF, *((uint32_t*)&hpfZoom));
                }
                sprintf(text[7],"ZOOM FRIQ HPF:%2.3f",hpfZoom);
                axisShaker[3]->getFilter().setFriquensyLeftFilter(hpfZoom);

                UpdateValue::readBlocking();
                axisShaker[0]->setBlokingMode(UpdateValue::blockingValue.BlockingBit.panRNDShakerBlocking);
                axisShaker[1]->setBlokingMode(UpdateValue::blockingValue.BlockingBit.tiltRNDShakerBlocking);
                axisShaker[2]->setBlokingMode(UpdateValue::blockingValue.BlockingBit.rollRNDShakerBlocking);
                axisShaker[3]->setBlokingMode(UpdateValue::blockingValue.BlockingBit.zoomRNDShakerBlocking);
                Table_Cell[0]->Active_Style.Cell_Color=colorCell[UpdateValue::blockingValue.BlockingBit.panRNDShakerBlocking];
                Table_Cell[0]->UnActive_Style.Cell_Color=colorCell[UpdateValue::blockingValue.BlockingBit.panRNDShakerBlocking];
                Table_Cell[1]->Active_Style.Cell_Color=colorCell[UpdateValue::blockingValue.BlockingBit.tiltRNDShakerBlocking];
                Table_Cell[1]->UnActive_Style.Cell_Color=colorCell[UpdateValue::blockingValue.BlockingBit.tiltRNDShakerBlocking];
                Table_Cell[2]->Active_Style.Cell_Color=colorCell[UpdateValue::blockingValue.BlockingBit.rollRNDShakerBlocking];
                Table_Cell[2]->UnActive_Style.Cell_Color=colorCell[UpdateValue::blockingValue.BlockingBit.rollRNDShakerBlocking];
                Table_Cell[3]->Active_Style.Cell_Color=colorCell[UpdateValue::blockingValue.BlockingBit.zoomRNDShakerBlocking];
                Table_Cell[3]->UnActive_Style.Cell_Color=colorCell[UpdateValue::blockingValue.BlockingBit.zoomRNDShakerBlocking];
            }
        };
    }
}



#endif /* PULT_INDICATOR_SHAKERFORM_HPP_ */
