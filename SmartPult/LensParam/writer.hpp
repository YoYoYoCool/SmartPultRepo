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


namespace Write {

enum {paket0=0,paketCrcName,paket1,paket2,paket3,paket4,paket5,paket6,paketCrc,numberOfPackages};
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
        for (Uint16 i=0;i<numberOfPackages; i++) { startAdres[i]=256*i; }
    }
    inline Uint16 getAdres (Uint8 startAdresNumber) {return  startAdres[startAdresNumber];}
private:
    Uint16 startAdres[numberOfPackages];
};

class PaketCreating {

public:
    PaketCreating(){
        for (Uint16 i=0;i<numberOfPackages; i++) {
            if (i==paketCrcName){
                _paket[i].adresFlash.all=0;
                _paket[i].adresFlash.slovo.startAdres=adres.getAdres(paketCrc);
                _paket[i].longPaket=0x3;
                i++;
                }
            _paket[i].adresFlash.all=0;
            _paket[i].adresFlash.slovo.startAdres=adres.getAdres(i);
            _paket[i].longPaket=0xff;
            if (i==paketCrc) {
                _paket[i].adresFlash.all=0;
                _paket[i].adresFlash.slovo.startAdres=adres.getAdres(paketCrc);
                _paket[i].longPaket=0x3;
                }
            }
        }
   inline void creatPaket (Uint8 pakketID);

   inline void setSektor (Uint32 adresSector) {
       for (Uint16 i=0; i<numberOfPackages; i++) {
           _paket[i].adresFlash.slovo.sector=adresSector;
       }
   }


private:
Paket _paket[numberOfPackages];
Adres adres;
};

}


#endif /* LENSPARAM_WRITER_HPP_ */
