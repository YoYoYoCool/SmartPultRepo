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

        virtual float calculate (float  inputData) {

        }

        virtual float calculate (float * inputData) {

        }

        virtual void updateKoafficient(float kalmanKoaf) {

        }

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


        virtual float calculate (float inputData) {
            float neweOptUp = (sigmaEta[0]*sigmaEta[0])*(eOpt+(sigmaPsi[0]*sigmaPsi[0]));
            float neweOptDn = (sigmaEta[0]*sigmaEta[0])+eOpt+(sigmaPsi[0]*sigmaPsi[0]);
            eOpt=neweOptUp/neweOptDn;
            kalmanKoaf=eOpt/(sigmaEta[0]*sigmaEta[0]);
            xOpt*=(1-kalmanKoaf);
            xOpt+=kalmanKoaf*inputData;
            return xOpt;
            }
        };

    class KalmanLineUpr: public KalmanBase {

    private:

        float kalmanKoaf;

    public:

        KalmanLineUpr(float  kalmanKoaf): kalmanKoaf(kalmanKoaf)
                {

                }

        virtual float calculate (float inputData) {
            xOpt*=(1-kalmanKoaf);
            xOpt+=kalmanKoaf*inputData;
            return xOpt;
        }

        virtual float calculate (float * inputData) {
            xOpt*=(1-kalmanKoaf);
            xOpt+=kalmanKoaf*inputData[0];
            return xOpt;
        }

        virtual void updateKoafficient(float kalmanKoaf) {
            this->kalmanKoaf=kalmanKoaf;
        }
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
