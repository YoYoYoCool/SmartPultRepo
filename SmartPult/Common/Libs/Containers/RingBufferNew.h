
#ifndef RINGBUFFERNEW_H_
#define RINGBUFFERNEW_H_

#include "stddef.h"
#include <stdint.h>
#include <stdbool.h>
#include "encoder.hpp"




namespace Containers {

	template<typename TData>
	class RingBufferN
	{
	protected:
		StandartElement::Encoder readEncoder;
		StandartElement::Encoder writeEncoder;
	public:

		RingBufferN(int16_t size) : readEncoder(size-1), writeEncoder(size-1){

		}
	
		inline virtual void write(TData value) =0;

		inline virtual TData readRight() = 0;

		inline virtual TData readLeft() = 0;

		inline virtual void clearBuffer() = 0;

		inline virtual void deleteFirst() = 0;

		inline virtual bool isEmpty() = 0;

		inline virtual int16_t getMaxRisc() =0;

	    };

	template<typename TData, uint16_t size>
	class RingBufferStatic : public RingBufferN<TData> {
	private:
		TData data[size];

	public:
		RingBufferStatic() :RingBuffer((int16_t)size) {
		    for (int16_t i=0; i<size; i++)
		        data[i]=0;
			}
		inline virtual void write(TData value) {
			data[writeEncoder.getActualPosition()] = value;
			writeEncoder.increment();
			}

		inline virtual TData readRight() {
			TData a = data[readEncoder.getActualPosition()];
			readEncoder.increment();
			return a;
			}

		inline virtual TData readLeft() {
		    TData a = data[readEncoder.getActualPosition()];
		    readEncoder.decrement();
		    return a;
		    }

		inline virtual void clearBuffer() {
			for (size_t i = readEncoder.getActualPosition(); i != readEncoder.getActualPosition(); readEncoder.increment()) {
				data[readEncoder.getActualPosition()] = (TData)0;
				}
			readEncoder.setActualPosition(0);
			writeEncoder.setActualPosition(0);
			}

		inline virtual void deleteFirst() {
			readEncoder.increment();
			}

		inline virtual bool isEmpty() {
			return readEncoder.getActualPosition() == writeEncoder.getActualPosition();
			}

		inline virtual int16_t getMaxRisc()  {
		     return writeEncoder.getMaxRisk();
		    }


		};
};
#endif /* INC_RINGBUFFER_HPP_ */
