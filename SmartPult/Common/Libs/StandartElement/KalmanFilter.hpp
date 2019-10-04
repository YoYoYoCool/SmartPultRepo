/*
 * KalmanFilter.hpp
 *
 *  Created on: 30 сент. 2019 г.
 *      Author: Кирилл Чеверев
 */

#ifndef COMMON_LIBS_STANDARTELEMENT_KALMANFILTER_HPP_
#define COMMON_LIBS_STANDARTELEMENT_KALMANFILTER_HPP_




    namespace KalmanFilter {

    class KalmanBase {
    private:

    protected:
        float xOpt;

    public:

        void setData(float xOpt) { this->xOpt=xOpt;}

        virtual void calculate (float * inputData) {

        }

        virtual void calculate (float  inputData) {

        }

        virtual void set(float  parametr1, float parametr2) {

        }

        virtual float getData() {}


    };

    class KalmanLine: public KalmanBase {
    private:
        float eOpt;
        float* sigmaEta; // дисперсия ошибки модели
        float* sigmaPsi;// дисперсия ошибки сенсора
        float kalmanKoaf;

    public:

        KalmanLine():KalmanBase()
        {
            eOpt=sigmaEta[0];
            xOpt=0;
        }


        virtual void calculate (float * inputData) {
            float neweOptUp = (sigmaEta[0]*sigmaEta[0])*(eOpt+(sigmaPsi[0]*sigmaPsi[0]));
            float neweOptDn = (sigmaEta[0]*sigmaEta[0])+eOpt+(sigmaPsi[0]*sigmaPsi[0]);
            eOpt=neweOptUp/neweOptDn;
            kalmanKoaf=eOpt/(sigmaEta[0]*sigmaEta[0]);
            xOpt*=(1-kalmanKoaf);
            xOpt+=kalmanKoaf*inputData[0];
            }

        virtual void calculate (float  inputData) {
            float neweOptUp = (sigmaEta[0]*sigmaEta[0])*(eOpt+(sigmaPsi[0]*sigmaPsi[0]));
            float neweOptDn = (sigmaEta[0]*sigmaEta[0])+eOpt+(sigmaPsi[0]*sigmaPsi[0]);
            eOpt=neweOptUp/neweOptDn;
            kalmanKoaf=eOpt/(sigmaEta[0]*sigmaEta[0]);
            xOpt*=(1-kalmanKoaf);
            xOpt+=kalmanKoaf*inputData;
        }


        virtual float getData() { return xOpt;}

        virtual void set(float*  parametr1, float* parametr2) {
            this->sigmaEta=parametr1;
            this->sigmaPsi=parametr2;
            eOpt=sigmaEta[0];
            }

        };

    class KalmanLineUpr: public KalmanBase {

    private:

        float * kalmanKoaf;

    public:

        KalmanLineUpr(float * kalmanKoaf): kalmanKoaf(kalmanKoaf)
                {

                }

        KalmanLineUpr()
                {

                }

        virtual void calculate (float* inputData) {
            xOpt*=(1-kalmanKoaf[0]);
            xOpt+=kalmanKoaf[0]*inputData[0];
            }

        virtual void calculate (float inputData) {
            xOpt*=(1-kalmanKoaf[0]);
            xOpt+=kalmanKoaf[0]*inputData;
        }

        virtual float getData() { return xOpt;}

        virtual void set(float * parametr1, float* parametr2) {     kalmanKoaf=parametr1;   }

    };


    template <uint8_t channel> class KalmanMultiChannel {

    private:
        KalmanBase* filter[channel];

    public:

        void setFilter(KalmanBase**filter) {
            for (uint8_t i=0; i<channel; i++)     {
                this->filter[i]= filter[i];     }
        }

        void set(float ** parametr1, float ** parametr2) {
            for (uint8_t i =0; i<channel;i++) {
                filter[i]->set(parametr1[i],parametr2[i])
            }
        }

        void calculateAll (float* inputData) {
            for (uint8_t i=0; i<channel; i++) {
                filter[i]->calculate(inputData);    }
        }

        void calculate (float* inputData, int8_t filterId) {   filter[filterId]->calculate(inputData);    }

        float getData (int8_t filterId) {   return filter[filterId]->getData();   }

        void setData(float data,uint8_t filterId) { filter[filterId]->setData(data);     }

        };
    }


#endif /* COMMON_LIBS_STANDARTELEMENT_KALMANFILTER_HPP_ */
