/*
 * Lib.h
 *
 *  Created on: 16 февр. 2019 г.
 *      Author: User9
 */

#ifndef LIB_LIB_H_
#define LIB_LIB_H_
#include "stddef.h"


inline Uint8 strLen (const char * str, Uint8 maxSize) {
for (Uint8 i=0; i<maxSize; i++) {
    if (str[i]=='\0') {

    return i;}
    }
}



#endif /* LIB_LIB_H_ */
