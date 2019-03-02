/*
 * List.hpp
 *
 *  Created on: 6 џэт. 2019 у.
 *      Author: KapustinIV
 */

#ifndef LIBS_CONTAINERS_LIST_HPP_
#define LIBS_CONTAINERS_LIST_HPP_

#include "../Utils/Debug.hpp"
#include "stdint.h"
#include "stddef.h"
#include "Array.hpp"

namespace Containers {

template<class TData>
class List : public  Array<TData> {
protected:
    size_t maxSize;
public:
    inline List(TData* arr, size_t maxSize) :
        Array<TData>(arr, 0), maxSize(maxSize) {
    }
    inline List(TData* arr, size_t size, size_t maxSize) :
        Array<TData>(arr, size), maxSize(maxSize) {
        debugAssert(this->size<maxSize);
    }
    inline void add(TData& data) {
        debugAssert(this->size<maxSize);
        this->data[this->size++] = data;
    }
    inline void set(size_t id, TData& data) {
        debugAssert(id<this->maxSize);
        if (id>=this->size) this->size=id+1;
        this->data[id] = data;
    }
    inline TData& get(size_t id) {
        debugAssert(id<this->size);
        return this->data[id];
    }
    inline void clear() {
        this->size = 0;
    }
    inline size_t getMaxSize() {return maxSize;};
};

template<class TData, size_t maxListSize>
class ListStatic : public  List<TData> {
protected:
    TData list[maxListSize];
public:
    inline ListStatic() :
        List<TData>(list, maxListSize){
    }
};

} /* namespace Containers */

#endif /* LIBS_CONTAINERS_LIST_HPP_ */
