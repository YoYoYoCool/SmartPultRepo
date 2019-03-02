/*
 * Array.hpp
 *
 *  Created on: 6 џэт. 2019 у.
 *      Author: KapustinIV
 */

#ifndef LIBS_CONTAINERS_ARRAY_HPP_
#define LIBS_CONTAINERS_ARRAY_HPP_

#include "Libs/Utils/Debug.hpp"
#include "stdint.h"
#include "stddef.h"

namespace Containers {

template<typename TData>
class Array {
protected:
    TData* data;
    size_t size;
public:
    inline Array(TData* data, size_t size) :
        data(data){
        debugAssert(data!=NULL);
    }
    inline Array(TData* dataArr, size_t size, TData initData) :
        data(dataArr),
        size(size){
        debugAssert(dataArr!=NULL);
        setAll(initData);
    }
    inline void setAll(TData& data) {
        for (size_t i = 0; i< size; i++) {
            this->data[i] = data;
        }
    }
    inline TData& operator[](size_t i) {
        debugAssert(i<size);
        return data[i];
    };
    inline size_t getSize() {return size;};
};

template<typename TData, size_t arraySize>
class ArrayStatic : public  Array<TData> {
protected:
    TData arr[arraySize];
public:
    inline ArrayStatic() :
        Array<TData>(arr, arraySize){
    }
    inline ArrayStatic(TData initData) :
        Array<TData>(arr, arraySize, initData) {
    }
};


} /* namespace Containers */

#endif /* LIBS_CONTAINERS_ARRAY_HPP_ */
