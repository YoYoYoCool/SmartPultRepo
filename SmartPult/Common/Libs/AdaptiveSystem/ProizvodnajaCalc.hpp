/*
 * ProizvodnajaCalc.hpp
 *
 *  Created on: 21 но€б. 2019 г.
 *      Author:  ирилл „еверев
 */

#ifndef COMMON_LIBS_ADAPTIVESYSTEM_PROIZVODNAJACALC_HPP_
#define COMMON_LIBS_ADAPTIVESYSTEM_PROIZVODNAJACALC_HPP_





#include "Libs/StandartElement/AttAmpStaticChannel.hpp"
#include "Libs/StandartElement/KalmanFilter.hpp"

namespace Module {

    class ModuleBase {

    protected:
        float & dataInput;
        float & dataOut;

    public:

        ModuleBase(float & dataInput,float & dataOut):dataInput(dataInput),
               dataOut(dataOut){

               }

        inline virtual void calculate() {}
    };

    class InputSystem:public ModuleBase {

    private:

        KalmanFilter::KalmanBase& filter;
        Resistor& speedControl;

    public:

        InputSystem(KalmanFilter::KalmanBase& filter,
                    float & dataInput,
                    Resistor& speedControl,float & dataOut):
                    filter(filter),
                    ModuleBase(dataInput,dataOut),
                    speedControl(speedControl){
        }

        inline virtual void calculate() {
            dataOut=dataInput;
            filter.calculate();
        }

    };


    class ProizvodnajaFilter: public ModuleBase{

    private:
        float & deltaTime;
        float dataZ;
        KalmanFilter::KalmanBase& filterAccelleration;

    public:

        ProizvodnajaFilter(KalmanFilter::KalmanBase& filterAccelleration,
                     float & dataInput,
                     float & dataOut,
                     float & deltaTime):
                     filterAccelleration(filterAccelleration),
                     dataZ(0.0),
                     deltaTime(deltaTime),
        ModuleBase(dataInput,dataOut){

        }

        inline virtual void calculate() {
            float normalInput=dataInput;
            dataOut=normalInput-dataZ;
            dataOut/=deltaTime;
            dataZ=normalInput;
            filterAccelleration.calculate();
        }
    };

    class Proizvodnaja: public ModuleBase{

    private:
        float & deltaTime;
        float dataZ;

    public:

        Proizvodnaja(float & dataInput,
                     float & dataOut,
                     float & deltaTime):
                     dataZ(0.0),
                     deltaTime(deltaTime),
        ModuleBase(dataInput,dataOut){

        }

        inline virtual void calculate() {
            float normalInput=dataInput;
            dataOut=normalInput-dataZ;
            dataOut/=deltaTime;
            dataOut*=5;
            dataZ=normalInput;
        }
    };


    class EylerConvert:public ModuleBase {



    };

    class TastinConvert: public ModuleBase {

    private:

        float &dataOldStep;
        float &deltaTime;
        float &tactingTime;

    public:

        TastinConvert(float & dataInput,float &dataOldStep,float & dataOut,float & delteTime,float &tactingTime):
        deltaTime(delteTime),
        dataOldStep(dataOldStep),
        tactingTime(tactingTime),
        ModuleBase(dataInput,dataOut){

            }

        inline virtual void calculate() {
            dataOut = dataInput - dataOldStep;
            float nViborok = deltaTime/tactingTime;
            dataOut/= nViborok;
        }

    };

    class Aproximation :public ModuleBase {

    private:

        float & dataOldStep;
        float & incrementValue;

    public:

        Aproximation(float & dataInput, float & dataOut, float &dataOldStep, float & incrementValue) :
            ModuleBase(dataInput,dataOut),
            incrementValue(incrementValue),
            dataOldStep(dataOldStep){

        }

        inline virtual void calculate() {
            dataOut+=incrementValue;
            dataOut=max(min(max(dataInput,dataOldStep),dataOut),min(dataInput,dataOldStep));
        }

    private:

        inline float max (float a, float b) {
            if (a>b)
                return a;
            else
                return b;
        }

        inline float min (float a, float b) {
            if (a<b)
                return a;
            else
                return b;
        }

    };

    class CalculateError: public ModuleBase{

    private:

        float & inputOS;
        KalmanFilter::KalmanBase& filter;

    public:

        CalculateError(KalmanFilter::KalmanBase& filter,float & dataInput,float & inputOS,float & dataOut):filter(filter),
        inputOS(inputOS),
        ModuleBase(dataInput,dataOut){

        }

        inline virtual void calculate() {
            dataOut=dataInput-inputOS;
            filter.calculate();
        }

    };

    class Integrator: public ModuleBase {

    public:

        Integrator(float & dataInput,float & dataOut):
        ModuleBase(dataInput,dataOut){

        }

        inline virtual void calculate() {       dataOut+=dataInput;        }

        };

    class Summator: public ModuleBase {

    private:

        float & inputData;

    public:

        Summator(float & dataInput,float & dataOut, float & inputError):
            inputData(inputError),
            ModuleBase(dataInput,dataOut){

        }

        inline virtual void calculate() {       dataOut=dataInput+inputData;        }

    };

}


#endif /* COMMON_LIBS_ADAPTIVESYSTEM_PROIZVODNAJACALC_HPP_ */
