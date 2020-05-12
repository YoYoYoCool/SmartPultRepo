/*
 * estimatingSistem.hpp
 *
 *  Created on: 26 но€б. 2019 г.
 *      Author:  ирилл „еверев
 */

#ifndef COMMON_LIBS_ADAPTIVESYSTEM_ESTIMATINGSISTEM_HPP_
#define COMMON_LIBS_ADAPTIVESYSTEM_ESTIMATINGSISTEM_HPP_


#include "ProizvodnajaCalc.hpp"

namespace Module {

    namespace ComplexSystems {

        class ComplexSystemsBase {

        public:

            inline virtual void calculate() {

            }

        };

        class EstimateSignal: public ComplexSystemsBase {

            private:

            ModuleBase& moduleError;
            ModuleBase& moduleIntegrator;

            public:

            EstimateSignal(ModuleBase& moduleErrorCalc,
                           ModuleBase& moduleIntegralCalc):
                               moduleError(moduleErrorCalc),
                               moduleIntegrator(moduleIntegralCalc){

            }

           inline virtual void calculate() {
                moduleError.calculate();
                moduleIntegrator.calculate();
            }

        };



    }

}



#endif /* COMMON_LIBS_ADAPTIVESYSTEM_ESTIMATINGSISTEM_HPP_ */
