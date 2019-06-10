/*
 * MemoryControl2.cpp
 *
 *  Created on: 30 мая 2019 г.
 *      Author: Cheverev Kirill
 */
/*
#include "MemoryControl.hpp"
#include "MemoryStructur.hpp"

MemoryStruct::StructAxisData proportionKoafMin= {
    .axis.pan=1,
    .axis.dutch=1,
    .axis.tilt=1,
    .axis.zoom = 0
    };

MemoryStruct::StructAxisData proportionKoafDefalt= {
    .axis.pan=1,
    .axis.dutch=1,
    .axis.tilt=1,
    .axis.zoom = 0
    };

MemoryStruct::StructAxisData proportionKoafMax= {
    .axis.pan=100,
    .axis.dutch=100,
    .axis.tilt=100,
    .axis.zoom = 0
    };

MemoryStruct::StructAxisData integralKoafMin= {
    .axis.pan=1,
    .axis.dutch=1,
    .axis.tilt=1,
    .axis.zoom = 0
    };

MemoryStruct::StructAxisData integralKoafDefalt= {
    .axis.pan=1,
    .axis.dutch=1,
    .axis.tilt=1,
    .axis.zoom = 0
    };

MemoryStruct::StructAxisData integralKoafMax= {
    .axis.pan=100,
    .axis.dutch=100,
    .axis.tilt=100,
    .axis.zoom = 0
    };

MemoryStruct::StructAxisData joyDeadZoneMin = {
    .axis.pan = 1,
    .axis.dutch = 1,
    .axis.tilt = 1,
    .axis.zoom = 1
};

MemoryStruct::StructAxisData joyDeadZoneDef = {
    .axis.pan = 7,
    .axis.dutch = 7,
    .axis.tilt = 7,
    .axis.zoom = 7
};

MemoryStruct::StructAxisData joyDeadZoneMax = {
    .axis.pan = 200,
    .axis.dutch = 200,
    .axis.tilt = 200,
    .axis.zoom = 200
};

MemoryStruct::StructAxisData torqueMin = {
    .axis.pan=0,
    .axis.dutch=0,
    .axis.tilt=0,
    .axis.zoom=0
};

MemoryStruct::StructAxisData torqueDef = {
    .axis.pan=100,
    .axis.dutch=100,
    .axis.tilt=100,
    .axis.zoom=0
};

MemoryStruct::StructAxisData torqueMax = {
    .axis.pan=100,
    .axis.dutch=100,
    .axis.tilt=100,
    .axis.zoom=0
};

MemoryStruct::StructAxisData startFluidMin= {
    .axis.pan=0,
    .axis.dutch=0,
    .axis.tilt=0,
    .axis.zoom = 0
    };

MemoryStruct::StructAxisData startFluidDefalt= {
    .axis.pan=0,
    .axis.dutch=0,
    .axis.tilt=0,
    .axis.zoom = 0
    };

MemoryStruct::StructAxisData startFluidMax= {
    .axis.pan=100,
    .axis.dutch=100,
    .axis.tilt=100,
    .axis.zoom = 100
    };

MemoryStruct::Configurate configurateMin = {
  .data.brightness = 1,
  .data.panSense =0,
  .data.dutchSense =0,
  .data.tiltSense =0,
  .data.joyConfig =0,
  .data.overslangUnderslang =0,
  .data.ecoMode =0,
  .data.panFolovingMode =0,
  .data.NU =0,
};

MemoryStruct::Configurate configurateDef = {
  .data.brightness = 100,
  .data.panSense =2,
  .data.dutchSense =2,
  .data.tiltSense =2,
  .data.joyConfig =0,
  .data.overslangUnderslang =1,
  .data.ecoMode =0,
  .data.panFolovingMode =0,
  .data.NU =0,
};

MemoryStruct::Configurate configurateMax = {
  .data.brightness = 100,
  .data.panSense =3,
  .data.dutchSense =3,
  .data.tiltSense =3,
  .data.joyConfig =2,
  .data.overslangUnderslang =1,
  .data.ecoMode = 1,
  .data.panFolovingMode = 2,
  .data.NU = 0,
};

MemoryStruct::ZifData zifParamMin = {
  .data.motors1Type  = 0,
  .data.motors2Type  = 0,
  .data.motors3Type  = 0,
  .data.zoomMotorId  = 0,
  .data.irisMotorId  = 0,
  .data.focusMotorId = 0,
  .data.zoomSense = 0,
  .data.zoomRevers = 0,
  .data.irisRevers = 0,
  .data.focusRevers = 0,
  .data.zoomDrift = - 100,
  .data.NU=0
};

MemoryStruct::ZifData zifParamDef = {
  .data.motors1Type  = 0,
  .data.motors2Type  = 0,
  .data.motors3Type  = 0,
  .data.zoomMotorId  = 0,
  .data.irisMotorId  = 2,
  .data.focusMotorId = 1,
  .data.zoomSense = 0,
  .data.zoomRevers = 0,
  .data.irisRevers = 0,
  .data.focusRevers = 0,
  .data.zoomDrift = 0,
  .data.NU=0
};

MemoryStruct::ZifData zifParamMax = {
  .data.motors1Type  = 1,
  .data.motors2Type  = 1,
  .data.motors3Type  = 1,
  .data.zoomMotorId  = 2,
  .data.irisMotorId  = 2,
  .data.focusMotorId = 2,
  .data.zoomSense = 3,
  .data.zoomRevers = 1,
  .data.irisRevers = 1,
  .data.focusRevers = 1,
  .data.zoomDrift = 100,
  .data.NU=0
};

MemoryStruct:: WheelAnalog wheelAnalogMin = {
    .data.wheelSpeedPan=0,
    .data.wheelSpeedTilt=0,
    .data.wheelSpeedRoll=0,
    .data.analogWheelPanActive=0,
    .data.analogWheelTiltActive=0,
    .data.analogWheelRollActive=0,
    .data.analogPedalActive=0,
    .data.NU = 0
    };

MemoryStruct:: WheelAnalog wheelAnalogDef = {
    .data.wheelSpeedPan=5,
    .data.wheelSpeedTilt=5,
    .data.wheelSpeedRoll=5,
    .data.analogWheelPanActive=0,
    .data.analogWheelTiltActive=0,
    .data.analogWheelRollActive=0,
    .data.analogPedalActive=0,
    .data.NU = 0
    };

MemoryStruct:: WheelAnalog wheelAnalogMax = {
    .data.wheelSpeedPan=255,
    .data.wheelSpeedTilt=255,
    .data.wheelSpeedRoll=255,
    .data.analogWheelPanActive=1,
    .data.analogWheelTiltActive=1,
    .data.analogWheelRollActive=1,
    .data.analogPedalActive=1,
    .data.NU = 0
    };

MemoryStruct:: WheelDigital  wheelDigitalMin = {
    .data.digitalWheelFerstID = 0,
    .data.digitalWheelSecondID =0,
    .data.digitalWheelThirdID = 0,
    .data.digitalWheelPanTransmission=0,
    .data.digitalWheelTiltTransmission=0,
    .data.digitalWheelRollTransmission=0,
    .data.NU=0
};


MemoryStruct:: WheelDigital  wheelDigitalDef = {
    .data.digitalWheelFerstID = 0,
    .data.digitalWheelSecondID =1,
    .data.digitalWheelThirdID = 2,
    .data.digitalWheelPanTransmission=0,
    .data.digitalWheelTiltTransmission=0,
    .data.digitalWheelRollTransmission=0,
    .data.NU=0
};

MemoryStruct:: WheelDigital  wheelDigitalMax = {
    .data.digitalWheelFerstID = 2,
    .data.digitalWheelSecondID =2,
    .data.digitalWheelThirdID = 2,
    .data.digitalWheelPanTransmission=3,
    .data.digitalWheelTiltTransmission=3,
    .data.digitalWheelRollTransmission=3,
    .data.NU=0
};

MemoryStruct::Sinhro sinhroMin = {
    .data.synchroSourceRS232 = 0,
    .data.NULow = 0,
    .data.synchroSourceRS485 = 0,
    .data.NUHigh = 0
};

MemoryStruct::Sinhro sinhroDef = {
    .data.synchroSourceRS232 = 0,
    .data.NULow = 0,
    .data.synchroSourceRS485 = 0,
    .data.NUHigh = 0
};

MemoryStruct::Sinhro sinhroMax = {
    .data.synchroSourceRS232 = 0,
    .data.NULow = 0,
    .data.synchroSourceRS485 = 0,
    .data.NUHigh = 0
};

MemoryStruct::LoadParameters loadParametersMin = {
    .data.cameraTipe = 0,
    .data.objectiveTipe = 0
};

MemoryStruct::LoadParameters loadParametersDef = {
    .data.cameraTipe = 0,
    .data.objectiveTipe = 0
};

MemoryStruct::LoadParameters loadParametersMax = {
    .data.cameraTipe = 0,
    .data.objectiveTipe = 0
};

MemoryStruct::Prerol1 prerol1Min = {
    .data.master = 0,
    .data.slave1 = 0
};

MemoryStruct::Prerol1 prerol1Def = {
    .data.master = 0,
    .data.slave1 = 0
};

MemoryStruct::Prerol1 prerol1Max = {
    .data.master = 30000,
    .data.slave1 = 30000
};

MemoryStruct::Prerol2 prerol2Min = {
    .data.slave2 = 0,
    .data.slave3 = 0
};

MemoryStruct::Prerol2 prerol2Def = {
    .data.slave2 = 0,
    .data.slave3 = 0
};

MemoryStruct::Prerol2 prerol2Max = {
    .data.slave2 = 30000,
    .data.slave3 = 30000
};

MemoryStruct::MAC adresWheelFerstMin = 0;
MemoryStruct::MAC adresWheelFerstDef = 0;
MemoryStruct::MAC adresWheelFerstMax = 0xFFFFFFFF;

MemoryStruct::MAC adresWheelSecondMin = 0;
MemoryStruct::MAC adresWheelSecondDef = 0;
MemoryStruct::MAC adresWheelSecondMax = 0xFFFFFFFF;

MemoryStruct::MAC adresWheelThirdMin = 0;
MemoryStruct::MAC adresWheelThirdDef = 0;
MemoryStruct::MAC adresWheelThirdMax = 0xFFFFFFFF;

MemoryStruct::Resonans panMin =  {
    .data.type=0,
    .data.friq=0,
};

MemoryStruct::Resonans panDef =  {
    .data.type=0,
    .data.friq=5600,
};

MemoryStruct::Resonans panMax =  {
    .data.type=2,
    .data.friq=9000,
};

MemoryStruct::Resonans tiltMin =  {
    .data.type=0,
    .data.friq=0,
};

MemoryStruct::Resonans tiltDef =  {
    .data.type=0,
    .data.friq=5600,
};

MemoryStruct::Resonans tiltMax =  {
    .data.type=0,
    .data.friq=9000,
};


/*
    uint32_t EE_Working::profileID=0;

   // //const EepromDefault pkDefault               =  {       0,      1,         100};     //0 koafFirst
   // //const EepromDefault dkDefault               =  {       0,      1,         100};     //1 foafSecond
  //  //const EepromDefault tkDefault               =  {       0,      1,         100};     //2 koafThird
  //  //const EepromDefault piDefault               =  {       0,      1,         100};     //3
  //  //const EepromDefault diDefault               =  {       0,      1,         100};     //4
  //  //const EepromDefault tiDefault               =  {       0,      1,         100};     //5
    //const EepromDefault brightnessDefault       =  {       0,      1,         100};     //6

    const EepromDefault panEqlLine0Default      =  {       0,      0x001C0009,         0x00e60084};     //7
    const EepromDefault panEqlLine1Default      =  {       0,      0x00400018,         0x00e60084};     //8
    const EepromDefault panEqlLine2Default      =  {       0,      0x0060002C,         0x00e60084};     //9
    const EepromDefault panEqlLine3Default      =  {       0,      0x00810048,         0x00e60084};     //10
    const EepromDefault panEqlLine4Default      =  {       0,      0x00A5006C,         0x00e60084};     //11
    const EepromDefault panEqlLine5Default      =  {       0,      0x00C6009E,         0x00e60084};     //12
    const EepromDefault panEqlLine6Default      =  {       0,      0x00E600E6,         0x00e60084};     //13
    const EepromDefault panEqlLine7Default      =  {       0,      0x00E600E6,         0x00e60084};     //14
    const EepromDefault panEqlLine8Default      =  {       0,      0x00E600E6,         0x00e60084};     //15
    const EepromDefault panEqlLine9Default      =  {       0,      0x00E600E6,         0x00e60084};     //16

    const EepromDefault dutchEqlLine0Default    =  {       0,      0x0020000B,         0x00e600e6};     //17
    const EepromDefault dutchEqlLine1Default    =  {       0,      0x00400016,         0x00e600e6};     //18
    const EepromDefault dutchEqlLine2Default    =  {       0,      0x00600022,         0x00e600e6};     //19
    const EepromDefault dutchEqlLine3Default    =  {       0,      0x00810038,         0x00e600e6};     //20
    const EepromDefault dutchEqlLine4Default    =  {       0,      0x00A10060,         0x00e600e6};     //21
    const EepromDefault dutchEqlLine5Default    =  {       0,      0x00C30097,         0x00e600e6};     //22
    const EepromDefault dutchEqlLine6Default    =  {       0,      0x00E600E6,         0x00e600e6};     //23
    const EepromDefault dutchEqlLine7Default    =  {       0,      0x00e60084,         0x00e600e6};     //24
    const EepromDefault dutchEqlLine8Default    =  {       0,      0x00e60084,         0x00e600e6};     //25
    const EepromDefault dutchEqlLine9Default    =  {       0,      0x00e60084,         0x00e600e6};     //26

    const EepromDefault tiltEqlLine0Default     =  {       0,      0x00200009,         0x00e600D8};     //27
    const EepromDefault tiltEqlLine1Default     =  {       0,      0x00400017,         0x00e600D8};     //28
    const EepromDefault tiltEqlLine2Default     =  {       0,      0x00600026,         0x00e600D8};     //29
    const EepromDefault tiltEqlLine3Default     =  {       0,      0x00830041,         0x00e600D8};     //30
    const EepromDefault tiltEqlLine4Default     =  {       0,      0x00A50068,         0x00e600D8};     //31
    const EepromDefault tiltEqlLine5Default     =  {       0,      0x00C8009D,         0x00e600D8};     //32
    const EepromDefault tiltEqlLine6Default     =  {       0,      0x00E600DF,         0x00e600D8};     //33
    const EepromDefault tiltEqlLine7Default     =  {       0,      0x00e60084,         0x00e600D8};     //34
    const EepromDefault tiltEqlLine8Default     =  {       0,      0x00e60084,         0x00e600D8};     //35
    const EepromDefault tiltEqlLine9Default     =  {       0,      0x00e60084,         0x00e600D8};     //36

//    const EepromDefault panSenseDefault         =  {       0,               2,                  3};     //37
//    const EepromDefault dutchSenseDefault       =  {       0,               2,                  3};     //38
//    const EepromDefault tiltSenseDefault        =  {       0,               2,                  3};     //39

    const EepromDefault shadowSettings0Default  =  {       0xC1A000BE,      0x3170A3D7,         0x41A000BE};    //40
    const EepromDefault shadowSettings1Default  =  {       0xC1A000BE,      0x3170A3D7,         0x41A000BE};    //41
    const EepromDefault shadowSettings2Default  =  {       0xC1A000BE,      0x3170A3D7,         0x41A000BE};    //42
    const EepromDefault shadowSettings3Default  =  {       0xC1A000BE,      0x3170A3D7,         0x41A000BE};    //43
    const EepromDefault shadowSettings4Default  =  {       0xC1A000BE,      0x3170A3D7,         0x41A000BE};    //44
    const EepromDefault shadowSettings5Default  =  {       0xC1A000BE,      0x3170A3D7,         0x41A000BE};    //45
    const EepromDefault shadowSettings6Default  =  {       0xC1A000BE,      0x3170A3D7,         0x41A000BE};    //46
    const EepromDefault shadowSettings7Default  =  {       0xC1A000BE,      0x3170A3D7,         0x41A000BE};    //47
    const EepromDefault shadowSettings8Default  =  {       0xC1A000BE,      0x3170A3D7,         0x41A000BE};    //48
    const EepromDefault shadowSettings9Default  =  {       0xC1A000BE,      0x3170A3D7,         0x41A000BE};    //49

//    const EepromDefault joyConfigDefault        =  {       0,               0,                  2};             //50
//    const EepromDefault motors1ModelDefault     =  {       0,      0,         0};                               //51    не используются
//    const EepromDefault motors2ModelDefault     =  {       0,      0,         0};                               //52    не используются
//    const EepromDefault motors3ModelDefault     =  {       0,      0,         0};                               //53    не используются
//    const EepromDefault motors1TypeDefault      =  {       0,               0,                  1};             //54
//    const EepromDefault motors2TypeDefault      =  {       0,               0,                  1};             //55
//    const EepromDefault motors3TypeDefault      =  {       0,               0,                  1};             //56

//    const EepromDefault zoomMotorIdDefault      =  {       0,               0,                  2};             //57
//    const EepromDefault irisMotorIdDefault      =  {       0,               2,                  2};             //58
//    const EepromDefault focusMotorIdDefault     =  {       0,               1,                  2};             //59

    const EepromDefault zoomEqlLine0Default     =  {       0,      0x00030030,         0x00e600e6};             //60
    const EepromDefault zoomEqlLine1Default     =  {       0,      0x00230039,         0x00e600e6};             //61
    const EepromDefault zoomEqlLine2Default     =  {       0,      0x004C0042,         0x00e600e6};             //62
    const EepromDefault zoomEqlLine3Default     =  {       0,      0x00750063,         0x00e600e6};             //63
    const EepromDefault zoomEqlLine4Default     =  {       0,      0x009C009F,         0x00e600e6};             //64
    const EepromDefault zoomEqlLine5Default     =  {       0,      0x00C100E6,         0x00e600e6};             //65
    const EepromDefault zoomEqlLine6Default     =  {       0,      0x00E600E6,         0x00e600e6};             //66
    const EepromDefault zoomEqlLine7Default     =  {       0,      0x00E600E6,         0x00e600e6};             //67
    const EepromDefault zoomEqlLine8Default     =  {       0,      0x00E600E6,         0x00e600e6};             //68
    const EepromDefault zoomEqlLine9Default     =  {       0,      0x00E600E6,         0x00e600e6};             //69

//    const EepromDefault zoomSenseDefault        =  {       0,               0,                  3};             //70
//    const EepromDefault cameraStartDefault      =  {       0,               0,                  7};             //71
//    const EepromDefault overslangDefault        =  {       0,               1,                  1};             //72

//    const EepromDefault analogWheelData         =  {       0,      0x00050505,         0xffffffff};             //73
//    const EepromDefault digitalWheelata          =  {       0,     0xBEB880A4,         0xffffffff};             //74
    //const EepromDefault tiltResonTypeDefault    =  {       0,               0,                  0};             //75
    //const EepromDefault tiltResonFreqDefault    =  {       0,      0x40B3332D,         0x41100000};             //76

//    //const EepromDefault panMaxTorqDefault       =  {       0,             100,                100};             //77
//    //const EepromDefault dutchMaxTorqDefault     =  {       0,             100,                100};             //78
//    //const EepromDefault tiltMaxTorqDefault      =  {       0,             100,                100};             //79

    //const EepromDefault panJoyDzDefault         =  {       0,              7,                200};             //80
    //const EepromDefault dutchJoyDzDefault       =  {       0,              7,                200};             //81
    //const EepromDefault tiltJoyDzDefault        =  {       0,              7,                200};             //82
    //const EepromDefault zoomJoyDzDefault        =  {       0,              7,                200};             //83

    //const EepromDefault dutchWheelSpeedDefault  =  {       0,      0xffffffff,         0xffffffff};             //84

    //const EepromDefault panResonanceTypeDefault =  {       0,               0,                  2};             //85
    //const EepromDefault panResonanceFreqDefault =  {       0,      0x40BFFFF9,         0x41100000};             //86

    //const EepromDefault panStartFluidDefault    =  {       0,               0,               100};              //87
    //const EepromDefault dutchStartFluidDefault  =  {       0,               0,               100};              //88
    //const EepromDefault tiltStartFluidDefault   =  {       0,               0,               100};              //89
    //const EepromDefault zoomStartFluidDefault   =  {       0,               0,               100};              //90

    //const EepromDefault zoomDriftDefault        =  {       0xFFFFFF00,      0,               100};              //91

    //const EepromDefault prerolMasterDefault     =  {       0,               0,             30000};              //92
    //const EepromDefault synchroSourceDefault    =  {       0,               0,                 1};              //93
    //const EepromDefault prerolSlave1Default     =  {       0,               0,             30000};              //94
    //const EepromDefault prerolSlave2Default     =  {       0,               0,             30000};              //95
    //const EepromDefault prerolSlave3Default     =  {       0,               0,             30000};              //96

//    const EepromDefault ecoModeDefault          =  {       0,               0,                 1};              //97
//    const EepromDefault panFolowingDefault      =  {       0,               0,                 2};              //98
    const EepromDefault CRCDefault              =  {       0,      0,         0};                               //99




    const EepromDefault* defaultSettings[]=
    {
         &pkDefault,
         &dkDefault,
         &tkDefault,
         &piDefault,
         &diDefault,
         &tiDefault,
         &brightnessDefault,

         &panEqlLine0Default,
         &panEqlLine1Default,
         &panEqlLine2Default,
         &panEqlLine3Default,
         &panEqlLine4Default,
         &panEqlLine5Default,
         &panEqlLine6Default,
         &panEqlLine7Default,
         &panEqlLine8Default,
         &panEqlLine9Default,

         &dutchEqlLine0Default,
         &dutchEqlLine1Default,
         &dutchEqlLine2Default,
         &dutchEqlLine3Default,
         &dutchEqlLine4Default,
         &dutchEqlLine5Default,
         &dutchEqlLine6Default,
         &dutchEqlLine7Default,
         &dutchEqlLine8Default,
         &dutchEqlLine9Default,

         &tiltEqlLine0Default,
         &tiltEqlLine1Default,
         &tiltEqlLine2Default,
         &tiltEqlLine3Default,
         &tiltEqlLine4Default,
         &tiltEqlLine5Default,
         &tiltEqlLine6Default,
         &tiltEqlLine7Default,
         &tiltEqlLine8Default,
         &tiltEqlLine9Default,

         &panSenseDefault,
         &dutchSenseDefault,
         &tiltSenseDefault,

         &shadowSettings0Default,
         &shadowSettings1Default,
         &shadowSettings2Default,
         &shadowSettings3Default,
         &shadowSettings4Default,
         &shadowSettings5Default,
         &shadowSettings6Default,
         &shadowSettings7Default,
         &shadowSettings8Default,
         &shadowSettings9Default,

         &joyConfigDefault,
         &motors1ModelDefault,
         &motors2ModelDefault,
         &motors3ModelDefault,
         &motors1TypeDefault,
         &motors2TypeDefault,
         &motors3TypeDefault,

         &zoomMotorIdDefault,
         &irisMotorIdDefault,
         &focusMotorIdDefault,

         &zoomEqlLine0Default,
         &zoomEqlLine1Default,
         &zoomEqlLine2Default,
         &zoomEqlLine3Default,
         &zoomEqlLine4Default,
         &zoomEqlLine5Default,
         &zoomEqlLine6Default,
         &zoomEqlLine7Default,
         &zoomEqlLine8Default,
         &zoomEqlLine9Default,

         &zoomSenseDefault,
         &cameraStartDefault,
         &overslangDefault,

         &analogWheelData,
         &digitalWheelata,
         &tiltResonTypeDefault,
         &tiltResonFreqDefault,

         &panMaxTorqDefault,
         &dutchMaxTorqDefault,
         &tiltMaxTorqDefault,

         &panJoyDzDefault,
         &dutchJoyDzDefault,
         &tiltJoyDzDefault,
         &zoomJoyDzDefault,

         &dutchWheelSpeedDefault,

         &panResonanceTypeDefault,
         &panResonanceFreqDefault,

         &panStartFluidDefault,
         &dutchStartFluidDefault,
         &tiltStartFluidDefault,
         &zoomStartFluidDefault,

         &zoomDriftDefault,

         &prerolMasterDefault,
         &synchroSourceDefault,
         &prerolSlave1Default,
         &prerolSlave2Default,
         &prerolSlave3Default,

         &ecoModeDefault,
         &panFolowingDefault,
         &CRCDefault
    };



    const EepromDefault& getSettingsLimits(EEPROM_Filds filed)
    {
        return (*defaultSettings[filed]);
    }


    const EepromDefault calibrationMainPan      =     {       0,      1860,         0xFFFF};
    const EepromDefault calibrationMainDutch    =     {       0,      1860,         0xFFFF};
    const EepromDefault calibrationMainTilt     =     {       0,      1860,         0xFFFF};
    const EepromDefault calibrationMainZoom     =     {       0,      1860,         0xFFFF};
    const EepromDefault calibrationExtPan       =     {       0,      1860,         0xFFFF};
    const EepromDefault calibrationExtDutch     =     {       0,      1860,         0xFFFF};
    const EepromDefault calibrationExtTilt      =     {       0,      1860,         0xFFFF};
    const EepromDefault calibrationExt2Dutch    =     {       0,      2110,         0xFFFF};
    const EepromDefault calibrationHours        =     {       0,      0,            0xFFFFFF};
    const EepromDefault calibrationCRC          =     {       0,      0,            0xFFFFFF};

    const EepromDefault* defaultCalibrationSettings[]=
    {
         &calibrationMainPan,
         &calibrationMainDutch ,
         &calibrationMainTilt,
         &calibrationMainZoom,
         &calibrationExtPan,
         &calibrationExtDutch,
         &calibrationExtTilt,
         &calibrationExt2Dutch,
         &calibrationHours,
         &calibrationCRC
    };

    const EepromDefault& getCalibrationLimits(EepromCalibrationAddressMap filed)
    {
        return (*defaultCalibrationSettings[filed]);
    }

*/
