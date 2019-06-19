/*
 * MemoryControl.cpp
 *
 *  Created on: 25 окт. 2018 г.
 *      Author: AhrameshinGN
 */


#include "MemoryControl.hpp"

    uint32_t EE_Working::profileID=0;

    const EepromDefault pkDefault               =  {       0,      1,         100};     //0 koafFirst
    const EepromDefault dkDefault               =  {       0,      1,         100};     //1 foafSecond
    const EepromDefault tkDefault               =  {       0,      1,         100};     //2 koafThird
    const EepromDefault piDefault               =  {       0,      1,         100};     //3
    const EepromDefault diDefault               =  {       0,      1,         100};     //4
    const EepromDefault tiDefault               =  {       0,      1,         100};     //5
    const EepromDefault brightnessDefault       =  {       0,      1,         100};     //6

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

    const EepromDefault panSenseDefault         =  {       0,               2,                  3};     //37
    const EepromDefault dutchSenseDefault       =  {       0,               2,                  3};     //38
    const EepromDefault tiltSenseDefault        =  {       0,               2,                  3};     //39

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

    const EepromDefault joyConfigDefault        =  {       0,               0,                  2};             //50
    const EepromDefault motors1ModelDefault     =  {       0,      0,         0};                               //51    не используются
    const EepromDefault motors2ModelDefault     =  {       0,      0,         0};                               //52    не используются
    const EepromDefault motors3ModelDefault     =  {       0,      0,         0};                               //53    не используются
    const EepromDefault motors1TypeDefault      =  {       0,               0,                  1};             //54
    const EepromDefault motors2TypeDefault      =  {       0,               0,                  1};             //55
    const EepromDefault motors3TypeDefault      =  {       0,               0,                  1};             //56

    const EepromDefault zoomMotorIdDefault      =  {       0,               0,                  2};             //57
    const EepromDefault irisMotorIdDefault      =  {       0,               2,                  2};             //58
    const EepromDefault focusMotorIdDefault     =  {       0,               1,                  2};             //59

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

    const EepromDefault zoomSenseDefault        =  {       0,               0,                  3};             //70
    const EepromDefault cameraStartDefault      =  {       0,               0,                  7};             //71
    const EepromDefault overslangDefault        =  {       0,               1,                  1};             //72

    const EepromDefault analogWheelData         =  {       0,      0x00050505,         0xffffffff};             //73
    const EepromDefault digitalWheelata          =  {       0,     0xBEB880A4,         0xffffffff};             //74
    const EepromDefault tiltResonTypeDefault    =  {       0,               0,                  0};             //75
    const EepromDefault tiltResonFreqDefault    =  {       0,      0x40B3332D,         0x41100000};             //76

    const EepromDefault panMaxTorqDefault       =  {       0,             100,                100};             //77
    const EepromDefault dutchMaxTorqDefault     =  {       0,             100,                100};             //78
    const EepromDefault tiltMaxTorqDefault      =  {       0,             100,                100};             //79

    const EepromDefault panJoyDzDefault         =  {       0,              7,                200};             //80
    const EepromDefault dutchJoyDzDefault       =  {       0,              7,                200};             //81
    const EepromDefault tiltJoyDzDefault        =  {       0,              7,                200};             //82
    const EepromDefault zoomJoyDzDefault        =  {       0,              7,                200};             //83

    const EepromDefault dutchWheelSpeedDefault  =  {       0,      0xffffffff,         0xffffffff};             //84

    const EepromDefault panResonanceTypeDefault =  {       0,               0,                  2};             //85
    const EepromDefault panResonanceFreqDefault =  {       0,      0x40BFFFF9,         0x41100000};             //86

    const EepromDefault panStartFluidDefault    =  {       0,               0,               100};              //87
    const EepromDefault dutchStartFluidDefault  =  {       0,               0,               100};              //88
    const EepromDefault tiltStartFluidDefault   =  {       0,               0,               100};              //89
    const EepromDefault zoomStartFluidDefault   =  {       0,               0,               100};              //90

    const EepromDefault zoomDriftDefault        =  {       0xFFFFFF00,      0,               100};              //91

    const EepromDefault prerolMasterDefault     =  {       0,               0,             30000};              //92
    const EepromDefault synchroSourceDefault    =  {       0,               0,                 1};              //93
    const EepromDefault prerolSlave1Default     =  {       0,               0,             30000};              //94
    const EepromDefault prerolSlave2Default     =  {       0,               0,             30000};              //95
    const EepromDefault prerolSlave3Default     =  {       0,               0,             30000};              //96

    const EepromDefault ecoModeDefault          =  {       0,               0,                 1};              //97
    const EepromDefault panFolowingDefault      =  {       0,               0,                 2};              //98
    const EepromDefault CRCDefaultOld           =  {       0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF};     //99
    const EepromDefault ZIFRevers               =  {       0,               0x7,                0x7};     //100
    const EepromDefault externalLensControlOnOff=  {       0,               0,                  1};        //101
    const EepromDefault empty[97]               =  {       0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //102
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //103
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //104
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //105
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //106
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //107
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //108
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //109
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //110
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //111
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //112
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //113
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //114
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //115
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //116
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //117
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //118
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //119
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //120
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //121
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //122
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //123
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //124
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //125
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //126
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //127
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //128
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //129
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //130
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //131
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //132
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //133
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //134
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //135
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //136
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //137
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //138
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //139
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //140
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //141
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //142
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //143
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //144
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //145
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //146
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //147
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //148
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //149
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //150
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //151
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //152
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //153
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //154
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //155
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //156
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //157
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //158
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //159
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //160
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //161
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //162
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //163
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //164
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //165
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //166
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //167
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //168
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //169
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //170
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //171
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //172
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //173
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //174
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //175
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //176
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //177
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //178
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //179
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //180
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //181
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //182
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //183
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //184
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //185
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //186
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //187
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //188
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //189
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //190
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //191
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //192
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //193
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //194
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //195
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //196
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF,     //197
                                                           0xFFFFFFFF,      0xFFFFFFFF,         0xFFFFFFFF};     //198

    const EepromDefault CRCDefault              =  {       0,      0,         0};                               //199




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
         &CRCDefaultOld,
         &ZIFRevers,
         &externalLensControlOnOff,
         &empty[0],
         &empty[1],
         &empty[2],
         &empty[3],
         &empty[4],
         &empty[5],
         &empty[6],
         &empty[7],
         &empty[8],
         &empty[9],
         &empty[10],
         &empty[11],
         &empty[12],
         &empty[13],
         &empty[14],
         &empty[15],
         &empty[16],
         &empty[17],
         &empty[18],
         &empty[19],
         &empty[20],
         &empty[21],
         &empty[22],
         &empty[23],
         &empty[24],
         &empty[25],
         &empty[26],
         &empty[27],
         &empty[28],
         &empty[29],
         &empty[30],
         &empty[31],
         &empty[32],
         &empty[33],
         &empty[34],
         &empty[35],
         &empty[36],
         &empty[37],
         &empty[38],
         &empty[39],
         &empty[40],
         &empty[41],
         &empty[42],
         &empty[43],
         &empty[44],
         &empty[45],
         &empty[46],
         &empty[47],
         &empty[48],
         &empty[49],
         &empty[50],
         &empty[51],
         &empty[52],
         &empty[53],
         &empty[54],
         &empty[55],
         &empty[56],
         &empty[57],
         &empty[58],
         &empty[59],
         &empty[60],
         &empty[61],
         &empty[62],
         &empty[63],
         &empty[64],
         &empty[65],
         &empty[66],
         &empty[67],
         &empty[68],
         &empty[69],
         &empty[70],
         &empty[71],
         &empty[72],
         &empty[73],
         &empty[74],
         &empty[75],
         &empty[76],
         &empty[77],
         &empty[78],
         &empty[79],
         &empty[80],
         &empty[81],
         &empty[82],
         &empty[83],
         &empty[84],
         &empty[85],
         &empty[86],
         &empty[87],
         &empty[88],
         &empty[89],
         &empty[90],
         &empty[91],
         &empty[92],
         &empty[93],
         &empty[94],
         &empty[95],
         &empty[96],
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

