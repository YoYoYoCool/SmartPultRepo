#ifndef FIFO__H
#define FIFO__H

//typedef FifoStruct {
//
//} FifoStruct

//размер должен быть степенью двойки: 4,8,16,32...128
#define Fifo( size )\
  struct {\
    UInt8 buf[size];\
    UInt8 tail;\
    UInt8 head;\
  }

//количество элементов в очереди
#define Fifo_count(fifo)     (fifo.head-fifo.tail)

//размер fifoFifo_count
#define Fifo_size(fifo)      ( sizeof(fifo.buf)/sizeof(fifo.buf[0]) )

//fifo заполнено?
#define Fifo_isFull(fifo)   (Fifo_count(fifo)==Fifo_size(fifo))

//fifo пусто?
#define Fifo_isEmpty(fifo)  (fifo.tail==fifo.head)

//количество свободного места в fifo
#define Fifo_space(fifo)     (Fifo_size(fifo)-Fifo_count(fifo))

//поместить элемент в fifo
#define Fifo_push(fifo, byte) \
  {\
    fifo.buf[fifo.head & (Fifo_size(fifo)-1)]=byte;\
    fifo.head++;\
  }

//взять первый элемент из fifo
#define Fifo_front(fifo) (fifo.buf[fifo.tail & (Fifo_size(fifo)-1)])

//уменьшить количество элементов в очереди
#define Fifo_pop(fifo)   \
  {\
      fifo.tail++; \
  }

//очистить fifo
#define Fifo_flush(fifo)   \
  {\
    fifo.tail=0;\
    fifo.head=0;\
  }

#endif //FIFO__H
