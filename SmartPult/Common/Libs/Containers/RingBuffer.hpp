#ifndef INC_RINGBUFFER_HPP_
#define INC_RINGBUFFER_HPP_

#include "stddef.h"
#include <stdint.h>
#include <stdbool.h>
#include "Libs/Utils/Debug.hpp"

namespace Containers {

template<typename TData, size_t size>
class RingBuffer
{
protected:
    TData _data[size];
    volatile size_t _readCount;
    volatile size_t _writeCount;
    static const size_t _mask = size - 1;
public:
  inline RingBuffer() :_readCount(0), _writeCount(0)  {
      debugAssert((size&(size-1))==0);
  }
  inline RingBuffer(TData initialValue, size_t initialCount) {
      debugAssert((size&(size-1))==0);
      extCount(initialCount);
      for(size_t i = 0; i<initialCount; i++) {
          _data[i] = 0;
      }
  }
  inline void write(TData value) {
        debugAssert(!isFull());
        _data[_writeCount++ & _mask] = value;
  }
  inline void read(TData& value) {
       debugAssert(!isEmpty());
       value = _data[_readCount++ & _mask];
  }
  inline void deleteFirst() {
       debugAssert(!isEmpty());
      _readCount++;
  }
  inline void deleteFirst(size_t count) {
      debugAssert(count<=this->getCount());
      _readCount = _readCount + count;
  }
  inline TData first()const
  {
      debugAssert(!isEmpty());
      return operator[](0);
  }
  inline TData last()const {
      debugAssert(!isFull());
      return operator[](getCount());
  }
   inline TData& operator[] (size_t i) {
       debugAssert(!isEmpty() && i < getCount());
       return _data[(_readCount + i) & _mask];
   }
   inline bool isEmpty()const {
       return _writeCount == _readCount;
   }
   inline bool isFull()const {
       return getCount() >= getSize();
   }
   inline void extCount(size_t addCount) {
       debugAssert(addCount+getCount()<=size);
       _writeCount = _writeCount + addCount;
   }
   inline size_t getCount()const {
       return _writeCount - _readCount;
   }

  inline void clear() {
      _readCount=0;
      _writeCount=0;
  }

  inline size_t getSize() const
  {return size;}

private:
};
};
#endif /* INC_RINGBUFFER_HPP_ */
