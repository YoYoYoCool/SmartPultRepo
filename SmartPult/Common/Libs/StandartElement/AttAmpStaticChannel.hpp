/*
 * AttAmpStaticChannel.hpp
 *
 *  Created on: 6 θών. 2019 γ.
 *      Author: User9
 */

#ifndef COMMON_LIBS_UNITELEMENT_ATTAMPSTATICCHANNEL_HPP_
#define COMMON_LIBS_UNITELEMENT_ATTAMPSTATICCHANNEL_HPP_


#include "AmplifierStage.hpp"
#include "attenuator.hpp"


namespace Schematic {



struct AttAmpSettings{
    StandartElement::AmplifierSettings amplifierSettings;
    StandartElement::AttenuatorSettings attenuatorSettings;
    };


class AttAmpStaticChannal {
private:
        StandartElement::AmplifierStage amplifier;
        StandartElement::Attenuator attenuator;

public:
    AttAmpStaticChannal(){

    }

    inline void setting (AttAmpSettings * settings) {
        amplifier.setting(settings->amplifierSettings);
        attenuator.setting(settings->attenuatorSettings);
    }



    inline float calkulate(float data,const float resistance) {
        data = attenuator.attenuation(data);
        data=  amplifier.gain(data,resistance);
        return data;
        }

    };

}


#endif /* COMMON_LIBS_UNITELEMENT_ATTAMPSTATICCHANNEL_HPP_ */
