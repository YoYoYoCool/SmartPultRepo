/*
 * AttAmpDinamicChannel.hpp
 *
 *  Created on: 6 θών. 2019 γ.
 *      Author: User9
 */

#ifndef COMMON_LIBS_UNITELEMENT_ATTAMPDINAMICCHANNEL_HPP_
#define COMMON_LIBS_UNITELEMENT_ATTAMPDINAMICCHANNEL_HPP_





#include "galetniy Perecluxhatel.hpp"
#include "AttAmpStaticChannel.hpp"



namespace Schematic {


template <size_t sizeChannel>
class AttAmpDinamicChannal {

private:
    StandartElement::GaletniyPerecluxhatel galetnic;
    Schematic::AttAmpStaticChannal channel[sizeChannel];


public:

    AttAmpDinamicChannal(): galetnic(0, (int8_t)sizeChannel-1) {

        }

    void setup(uint8_t idChannel, AttAmpSettings * settings ) {
        channel[idChannel].setting(settings);
        }

    void stepLeft() {
        galetnic.stepLeft();
//        updateFunction ();
        }

    void stepRight() {
        galetnic.stepRight();
//        updateFunction ();
        }

    void setFunction (int8_t functionName) {
        galetnic.setNeedPosition(functionName);
//        updateFunction ();
        }
    inline int8_t getFunction() {
        return (int8_t)galetnic.getActualPosition();
        }

    inline float updateData(float data,const float resistance) {
        return channel[galetnic.getActualPosition()].calkulate(data,resistance);
        }
    };


}


#endif /* COMMON_LIBS_UNITELEMENT_ATTAMPDINAMICCHANNEL_HPP_ */
