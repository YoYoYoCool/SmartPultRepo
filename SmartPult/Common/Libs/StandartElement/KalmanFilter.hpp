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

    protected:
        float xOpt;
        float & inputData;
        float & outData;

    public:

        KalmanBase(float & inputData,float & outData): inputData(inputData),outData(outData)
        {

        }

        virtual void calculate () {

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

        KalmanLine(float & inputData,float & outData):KalmanBase(inputData,outData)
        {
            eOpt=sigmaEta[0];
            xOpt=0;
        }


        virtual void calculate () {
            float neweOptUp = (sigmaEta[0]*sigmaEta[0])*(eOpt+(sigmaPsi[0]*sigmaPsi[0]));
            float neweOptDn = (sigmaEta[0]*sigmaEta[0])+eOpt+(sigmaPsi[0]*sigmaPsi[0]);
            eOpt=neweOptUp/neweOptDn;
            kalmanKoaf=eOpt/(sigmaEta[0]*sigmaEta[0]);
            xOpt*=(1-kalmanKoaf);
            xOpt+=kalmanKoaf*inputData;
            outData=xOpt;
            }
        };

    class KalmanLineUpr: public KalmanBase {

    private:

        float kalmanKoaf;

    public:

        KalmanLineUpr(float  kalmanKoaf,float & inputData,float & outData): kalmanKoaf(kalmanKoaf*6.28/500),
        KalmanBase(inputData,outData)
                {

                }


        virtual void calculate () {
            xOpt*=(1-kalmanKoaf);
            xOpt+=kalmanKoaf*inputData;
            outData=xOpt;
        }

        virtual void updateKoafficient(float kalmanKoaf) {
            this->kalmanKoaf=kalmanKoaf;
        }
    };

    class KalmanLineUprFriq: public KalmanBase {

    private:

        float kalmanKoaf;
        float & fCLK;

    public:

        KalmanLineUprFriq(float  friq, float& fCLK,float & inputData,float & outData):
        fCLK(fCLK),
        kalmanKoaf((2*3.14*friq)/fCLK),
        KalmanBase(inputData,outData)
                {

                }


        virtual void calculate () {
            xOpt*=(1-kalmanKoaf);
            xOpt+=kalmanKoaf*inputData;
            outData=xOpt;
        }

        virtual void updateKoafficient(float friq) {
            kalmanKoaf=2*3.14*friq/fCLK;
        }
    };



    }


#endif /* COMMON_LIBS_STANDARTELEMENT_KALMANFILTER_HPP_ */
