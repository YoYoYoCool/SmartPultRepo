/*
 * MemoryControl.hpp
 *
 *  Created on: 25 окт. 2018 г.
 *      Author: AhrameshinGN
 */

#ifndef PULT_INDICATOR_MEMORYCONTROL_HPP_
#define PULT_INDICATOR_MEMORYCONTROL_HPP_


#include "stdint.h"
#include "driverlib/eeprom.h"


#define PROFILE_INFO                0x0
#define EEStartAdress               0x4
#define EECount                     200             //количество 32х значений в ЕЕПРОМ
#define EE_CALIBRATION_VARS_START ((EECount*(MAX_PROFILE_COUNT+1)*4)+EEStartAdress)
#define MAX_PROFILE_COUNT           5

enum EepromCalibrationAddressMap
{
    EE_CAL_OFFSET_MAIN_PAN=0,
    EE_CAL_OFFSET_MAIN_DUTCH,
    EE_CAL_OFFSET_MAIN_TILT,
    EE_CAL_OFFSET_MAIN_ZOOM,
    EE_CAL_OFFSET_EXT_PAN,
    EE_CAL_OFFSET_EXT_DUCTH,
    EE_CAL_OFFSET_EXT_TILT,
    EE_CAL_OFFSET_EXT2_DUCTH,
    EE_CAL_HOURS,
    EE_CAL_DATA_PRODUCTION,
    EE_CAL_FACTORI_NUMBER,
    EE_CAL_INVENARY_NAMBER,
    EE_CAL_CRC
};

enum EEPROM_Filds
{
    EE_PK=0,
    EE_DK,
    EE_TK,
    EE_PI,
    EE_DI,
    EE_TI,
    EE_Bright,
    EE_PK_Line_0,   EE_PK_Line_1,   EE_PK_Line_2,   EE_PK_Line_3,   EE_PK_Line_4,
    EE_PK_Line_5,   EE_PK_Line_6,   EE_PK_Line_7,   EE_PK_Line_8,   EE_PK_Line_9,
    EE_DK_Line_0,   EE_DK_Line_1,   EE_DK_Line_2,   EE_DK_Line_3,   EE_DK_Line_4,
    EE_DK_Line_5,   EE_DK_Line_6,   EE_DK_Line_7,   EE_DK_Line_8,   EE_DK_Line_9,
    EE_TK_Line_0,   EE_TK_Line_1,   EE_TK_Line_2,   EE_TK_Line_3,   EE_TK_Line_4,
    EE_TK_Line_5,   EE_TK_Line_6,   EE_TK_Line_7,   EE_TK_Line_8,   EE_TK_Line_9,
    PAN_FUNC_VALUE,
    DATCH_FUNC_VALUE,
    TILT_FUNC_VALUE,
    ABS_PARAMETR0,ABS_PARAMETR1,ABS_PARAMETR2,ABS_PARAMETR3,ABS_PARAMETR4,
    ABS_PARAMETR5,ABS_PARAMETR6,ABS_PARAMETR7,ABS_PARAMETR8,ABS_PARAMETR9,
    EE_JOYSTIC_CONFIGURATION,
    EE_LC_MOTOR1_MODEL,
    EE_LC_MOTOR2_MODEL,
    EE_LC_MOTOR3_MODEL,
    EE_LC_MOTOR1_TYPE,
    EE_LC_MOTOR2_TYPE,
    EE_LC_MOTOR3_TYPE,
    EE_LC_ZOOM_MN,
    EE_LC_IRIS_MN,
    EE_LC_FOCUS_MN,
    EE_ZK_Line_0,   EE_ZK_Line_1,   EE_ZK_Line_2,   EE_ZK_Line_3,   EE_ZK_Line_4,
    EE_ZK_Line_5,   EE_ZK_Line_6,   EE_ZK_Line_7,   EE_ZK_Line_8,   EE_ZK_Line_9,
    EE_LC_ZOOM_SENSE,EE_LC_CAMERA_START,
    EE_LC_OVERSLANG_ON_OFF,
    EE_LC_WHEEL_ANALOG_DATA,
    EE_LC_WHEEL_DIGITAL_DATA,
    EE_LC_TILT_SUSPENSION_RES_TYPE,
    EE_LC_TILT_SUSPENSION_EDIT_FREQ,
    EE_LC_PAN_MAX_TORQUE,
    EE_LC_DUTCH_MAX_TORQUE,
    EE_LC_TILT_MAX_TORQUE,
    EE_LC_PAN_JOY_DZ,
    EE_LC_DUTCH_JOY_DZ,
    EE_LC_TILT_JOY_DZ,
    EE_LC_ZOOM_JOY_DZ,
    EE_LC_DUTCH_WHEEL_SPEED,
    EE_LC_PAN_SUSPENSION_RES_TYPE,
    EE_LC_PAN_SUSPENSION_EDIT_FREQ,
    EE_LC_PAN_START_FLUID,
    EE_LC_DUTCH_START_FLUID,
    EE_LC_TILT_START_FLUID,
    EE_LC_ZOOM_START_FLUID,
    EE_LC_ZOOM_DRIFT,
    EE_LC_MOTION_MASTER_PREROL,
    EE_LC_MOTION_SYNCRO_SOURCE,
    EE_LC_MOTION_SLAVE1_PREROL,
    EE_LC_MOTION_SLAVE2_PREROL,
    EE_LC_MOTION_SLAVE3_PREROL,
    EE_LC_ECO_MOD_ON,
    EE_LC_PAN_FOLOWING_SETTINGS,
    EE_LC_PROFILE_CRC_OLD,
    EE_LC_ZIF_REVERS,
    EE_LC_EXTERNAL_LENS_CONTROL,
    EE_LC_SHAKER_BLOCKING,
    EE_LC_INTERNAL_SHAKER_PAN_SPEED,
    EE_LC_INTERNAL_SHAKER_TILT_SPEED,
    EE_LC_INTERNAL_SHAKER_ROLL_SPEED,
    EE_LC_INTERNAL_SHAKER_ZOOM_SPEED,
    EE_LC_INTERNAL_SHAKER_PAN_HPF,
    EE_LC_INTERNAL_SHAKER_TILT_HPF,
    EE_LC_INTERNAL_SHAKER_ROLL_HPF,
    EE_LC_INTERNAL_SHAKER_ZOOM_HPF,
    EE_LC_CAMERA_ID_DOPREAL,
    EE_LC_PROFILE_CRC_NEW=199

};

struct EepromDefault
{
    uint32_t min;
    uint32_t def;
    uint32_t max;
};


const EepromDefault& getSettingsLimits(EEPROM_Filds filed);
const EepromDefault& getCalibrationLimits(EepromCalibrationAddressMap filed);

class EE_Working
{

    private:
        union WordToByteBM
        {
            uint32_t all;
            uint8_t bytes[4];
        };

        static uint32_t profileID;

        static inline uint32_t finedProfileCRC()
        {
            uint16_t TMP, CRCL, CRCH, CRC = 0xffff;
            CRCL = CRC;
            CRCH = (CRC >> 8);

            WordToByteBM eeValue;

            for(uint16_t i=0;i!=EE_LC_PROFILE_CRC_NEW;i++)
            {
                eeValue.all=Read(i);
                for (uint16_t i = 0; i !=4; i++)
                {
                    int16_t dataTmp = eeValue.bytes[i];
                    dataTmp = (dataTmp ^ CRCL);
                    TMP = (dataTmp << 4);
                    dataTmp = (TMP ^ dataTmp);
                    TMP = (dataTmp >> 5);
                    TMP &= 0x07;
                    CRCL = CRCH;
                    CRCH = (dataTmp ^ TMP);
                    TMP = (dataTmp << 3);
                    CRCL = (CRCL ^ TMP);
                    TMP = (dataTmp >> 4);
                    TMP &= 0x0F;
                    CRCL = (CRCL ^ TMP);
                }
            }
            CRC = (CRCH << 8) + (CRCL & 0x00FF);
            return CRC;
        }

        static inline uint32_t finedProfileCRC(EEPROM_Filds addresCRC)
                {
                    uint16_t TMP, CRCL, CRCH, CRC = 0xffff;
                    CRCL = CRC;
                    CRCH = (CRC >> 8);

                    WordToByteBM eeValue;

                    for(uint16_t i=0;i!=addresCRC;i++)
                    {
                        eeValue.all=Read(i);
                        for (uint16_t i = 0; i !=4; i++)
                        {
                            int16_t dataTmp = eeValue.bytes[i];
                            dataTmp = (dataTmp ^ CRCL);
                            TMP = (dataTmp << 4);
                            dataTmp = (TMP ^ dataTmp);
                            TMP = (dataTmp >> 5);
                            TMP &= 0x07;
                            CRCL = CRCH;
                            CRCH = (dataTmp ^ TMP);
                            TMP = (dataTmp << 3);
                            CRCL = (CRCL ^ TMP);
                            TMP = (dataTmp >> 4);
                            TMP &= 0x0F;
                            CRCL = (CRCL ^ TMP);
                        }
                    }
                    CRC = (CRCH << 8) + (CRCL & 0x00FF);
                    return CRC;
                }


        static inline uint32_t finedCalibrationCRC()
        {
            uint16_t TMP, CRCL, CRCH, CRC = 0xffff;
            CRCL = CRC;
            CRCH = (CRC >> 8);

            WordToByteBM eeValue;

            for(uint16_t i=0;i!=EE_CAL_CRC;i++)
            {
                EEPROMRead(&eeValue.all, (uint32_t)(EE_CALIBRATION_VARS_START+i*4), 4);
                for (uint16_t i = 0; i !=4; i++)
                {
                    int16_t dataTmp = eeValue.bytes[i];
                    dataTmp = (dataTmp ^ CRCL);
                    TMP = (dataTmp << 4);
                    dataTmp = (TMP ^ dataTmp);
                    TMP = (dataTmp >> 5);
                    TMP &= 0x07;
                    CRCL = CRCH;
                    CRCH = (dataTmp ^ TMP);
                    TMP = (dataTmp << 3);
                    CRCL = (CRCL ^ TMP);
                    TMP = (dataTmp >> 4);
                    TMP &= 0x0F;
                    CRCL = (CRCL ^ TMP);
                }
            }
            CRC = (CRCH << 8) + (CRCL & 0x00FF);
            return CRC;
        }

        static void validateProfileOld()
        {
            uint32_t crc;
            uint32_t findedCrc;

            EEPROMRead(&crc, (uint32_t)(EEStartAdress+(profileID*EECount*4) + EE_LC_PROFILE_CRC_NEW*4), 4);
            findedCrc=finedProfileCRC();
            if(findedCrc!=crc)
            {
                for(uint32_t i=0;i!=EE_LC_PROFILE_CRC_NEW;i++)
                {
                    EEPROMProgram((uint32_t*)(&getSettingsLimits((EEPROM_Filds)i).def),(uint32_t)(EEStartAdress+(profileID*EECount*4) + i*4), 4);
                }
            }
            findedCrc=finedProfileCRC();
            EEPROMProgram(&findedCrc,(uint32_t)(EEStartAdress+(profileID*EECount*4) + EE_LC_PROFILE_CRC_NEW*4), 4);
        }

        static void validateCalibrationData()
        {
            uint32_t crc;
            uint32_t findedCrc;

            EEPROMRead(&crc, (uint32_t)(EE_CALIBRATION_VARS_START+EE_CAL_CRC*4), 4);
            findedCrc=finedCalibrationCRC();
            if(findedCrc!=crc)
            {
                for(uint32_t i=0;i!=EE_CAL_CRC;i++)
                {
                    EEPROMProgram((uint32_t*)(&getCalibrationLimits((EepromCalibrationAddressMap)i).def),(uint32_t)(EE_CALIBRATION_VARS_START+i*4), 4);
                }
            }
            findedCrc=finedCalibrationCRC();
            EEPROMProgram(&findedCrc,(uint32_t)(EE_CALIBRATION_VARS_START+EE_CAL_CRC*4), 4);
        }


    public:

        static inline void refactoringEEPROM () {
            uint32_t crc;
            for (uint8_t profileId=0; profileId<5; profileId++) {
                EEPROMRead(&crc, (uint32_t)(EEStartAdress+(profileId*EECount*4) + EE_LC_PROFILE_CRC_OLD*4), 4);
                if (crc==(uint32_t)finedProfileCRC(EE_LC_PROFILE_CRC_OLD)) {
                    Write(EE_LC_PROFILE_CRC_OLD,0xFFFFFFFF);
                    }
                }

            }

        static inline uint32_t Write(uint8_t EE_num, uint32_t data)
        {
               uint32_t rezult;
               uint32_t crc;

               if(EE_num>=EECount) return 0;
               rezult = EEPROMProgram(&data,(uint32_t)(EEStartAdress+(profileID*EECount*4) + EE_num*4), 4);

               if(rezult==0)
                   {
                   crc= finedProfileCRC();
                   EEPROMProgram(&crc,(uint32_t)(EEStartAdress+(profileID*EECount*4) + EE_LC_PROFILE_CRC_NEW*4), 4);
                   }
               return rezult;
        }

        static inline uint32_t Read(uint8_t EE_num)
        {
               uint32_t ui32Data=0;

               if(EE_num>=EECount) return 0;

               EEPROMRead(&ui32Data, (uint32_t)(EEStartAdress+(profileID*EECount*4) + EE_num*4), 4);
               return ui32Data;
        }

        static inline uint8_t WriteCalibrationData(uint8_t EE_num, uint32_t data)
        {
               uint32_t rezult;
               uint32_t crc;

               if(EE_num>=EE_CAL_CRC) return 0;
               rezult = EEPROMProgram(&data,(uint32_t)(EE_CALIBRATION_VARS_START+EE_num*4), 4);

               if(rezult==0)
               {
                   crc= finedCalibrationCRC();
                   EEPROMProgram(&crc,(uint32_t)(EE_CALIBRATION_VARS_START+EE_CAL_CRC*4), 4);
               }

               return rezult;
        }

        static inline uint32_t ReadCalibrationData(uint8_t EE_num)
        {
                uint32_t ui32Data=0;
                if(EE_num>=EE_CAL_CRC) return 0;
                validateCalibrationData();
                EEPROMRead(&ui32Data, (uint32_t)(EE_CALIBRATION_VARS_START+EE_num*4), 4);
                return ui32Data;
        }

        static inline void setProfile  (uint32_t profileNumber)
        {
                profileID=profileNumber;
                validateProfileOld();
                EEPROMProgram(&profileNumber,PROFILE_INFO, 4);
        }
        static inline bool setDefaultProfile (uint32_t profileNumber)
        {
           // uint32_t crc;
            uint32_t findedCrc;
            profileID=profileNumber;
            for(uint32_t i=0;i!=EE_LC_PROFILE_CRC_NEW;i++)
            {
                EEPROMProgram((uint32_t*)(&getSettingsLimits((EEPROM_Filds)i).def),(uint32_t)(EEStartAdress+(profileNumber*EECount*4) + i*4), 4);
            }
            findedCrc=finedProfileCRC();
            EEPROMProgram(&findedCrc,(uint32_t)(EEStartAdress+(profileNumber*EECount*4) + EE_LC_PROFILE_CRC_NEW*4), 4);
            return true;
        }
        static inline uint32_t  getProfile()
        {
                uint32_t ret;
                EEPROMRead(&ret, PROFILE_INFO, 4);
                if(ret>=MAX_PROFILE_COUNT){ret=0;}
                profileID=ret;
                validateProfileOld();
                return ret;
        }
        static inline void    cleanEEPROM()
        {
               uint32_t data=0xFFFFFFFF;
               for(uint32_t i=0;i!=500;i++)
               {
                   EEPROMProgram((unsigned int*)(&data),4*i, 4);
               }
        }

        static inline uint32_t  getProfileID()
        {
            return profileID;
        }
};


#endif /* PULT_INDICATOR_MEMORYCONTROL_HPP_ */
