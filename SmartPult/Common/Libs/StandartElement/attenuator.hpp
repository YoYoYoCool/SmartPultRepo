/*
 * attenuator.hpp
 *
 *  Created on: 5 θών. 2019 γ.
 *      Author: User9
 */

#ifndef COMMON_LIBS_STANDARTELEMENT_ATTENUATOR_HPP_
#define COMMON_LIBS_STANDARTELEMENT_ATTENUATOR_HPP_




namespace StandartElement {

struct AttenuatorSettings {
    float deadZone;
    float attenuationK;
    float Vcc;
    float Vee;
};

class Attenuator {

private:

    AttenuatorSettings settings;

public:

    Attenuator ()  {        }

    Attenuator (AttenuatorSettings settings) : settings(settings) {        }

    inline void setting (AttenuatorSettings & settings) {
        this->settings=settings;
        }

    inline float attenuation (float data) {
        return data*settings.attenuationK;
        }

    inline void attenuationOnDeadZone (float  & data) {
        if (data<settings.deadZone)
            return;
        if (data>-settings.deadZone)
            return;
        data*=settings.attenuationK;
        }

    inline void attenuationSaturation (float &data) {
        data*=settings.attenuationK;
        if (data>settings.Vcc) {
            data=settings.Vcc;
            return;}
        if (data<settings.Vee) {
            data=settings.Vee;
            return;}
        }


    inline void attenuationSaturationAndDeadZone (float & data) {
        if (data<settings.deadZone)
            return;
        if (data>-settings.deadZone)
            return;
        data*=settings.attenuationK;
        if (data>settings.Vcc)
            data=settings.Vcc;
        if (data<settings.Vee)
            data=settings.Vee;
            }


    };






}



#endif /* COMMON_LIBS_STANDARTELEMENT_ATTENUATOR_HPP_ */
