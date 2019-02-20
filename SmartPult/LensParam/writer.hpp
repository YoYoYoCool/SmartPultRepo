/*
 * writer.hpp
 *
 *  Created on: 18 февр. 2019 г.
 *      Author: Cheverev kirill
 */

#ifndef LENSPARAM_WRITER_HPP_
#define LENSPARAM_WRITER_HPP_
#include "../LensParam/LensParam.hpp"
#include "../LensParam/driver.hpp"
#include "../LensParam/builder.hpp"



namespace Write {

enum {paketName=0,
    paketCrcName,
    paketZoomPosition,
    paketZoomPercent,
    paketIrisPosition,
    paketIrisPercent,
    paketFocusPosition,
    paketFocusPercent,
    paketCrcParamObj,
    numberOfPackages};


union AdresStruct {
    Uint32 all;
    struct {
    Uint16 startAdres    :12;
    Uint32 sector   :20;
    }slovo;
};

struct Paket {
    AdresStruct adresFlash;
    Uint8 data[256];
    Uint8 longPaket;
    };

class Adres {

public:
    Adres () {
        startAdres[paketName]=0;
        startAdres[paketCrcName]=256;
        startAdres[paketZoomPosition]=startAdres[paketCrcName]+4;
        for (Uint16 i=paketZoomPercent;i<numberOfPackages; i++) { startAdres[i]=startAdres[i-1]+256; }

    }
    inline Uint16 getAdres (Uint8 startAdresNumber) {return  startAdres[startAdresNumber];}
private:
    Uint16 startAdres[numberOfPackages];
};

class PaketCreating {

public:
    PaketCreating(){
        _paket[paketName].adresFlash.all=0;
        _paket[paketName].adresFlash.slovo.startAdres=adres.getAdres(paketCrcParamObj);
        _paket[paketName].longPaket=0xFF;
        _paket[paketCrcName].adresFlash.all=0;
        _paket[paketCrcName].adresFlash.slovo.startAdres=adres.getAdres(paketCrcParamObj);
        _paket[paketCrcName].longPaket=0x3;
        for (Uint16 i=paketZoomPosition;i<paketCrcParamObj; i++) {
            _paket[i].adresFlash.all=0;
            _paket[i].adresFlash.slovo.startAdres=adres.getAdres(i);
            _paket[i].longPaket=0xff;
            }
        _paket[paketCrcParamObj].adresFlash.all=0;
        _paket[paketCrcParamObj].adresFlash.slovo.startAdres=adres.getAdres(paketCrcParamObj);
        _paket[paketCrcParamObj].longPaket=0x3;
        }

   inline void creatPaket (Uint8 pakketID){


       }

   inline void setSektor (Uint32 adresSector) {
       for (Uint16 i=0; i<numberOfPackages; i++) {
           _paket[i].adresFlash.slovo.sector=adresSector;
       }
   }


private:
Paket _paket[numberOfPackages];
Adres adres;
Builder::DataStruct data;

};



}


#endif /* LENSPARAM_WRITER_HPP_ */
