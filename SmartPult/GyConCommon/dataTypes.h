/*
 * dataTypes.h
 *
 *  Created on: 03 џэт. 2015 у.
 *      Author: CNO
 */

#ifndef DATATYPES_H_
#define DATATYPES_H_

typedef unsigned char UInt8;
typedef signed char Int8;
typedef unsigned short UInt16;
typedef short Int16;
typedef unsigned int UInt32;
typedef int Int32;
#define NULL 0

typedef union {
   UInt16 all;
   struct {
	   UInt16 byte0:8;
	   UInt16 byte1:8;
   } bytes;
} UInt16Bytes;

typedef union {
   UInt32 all;
   struct {
	   UInt32 byte0:8;
	   UInt32 byte1:8;
	   UInt32 byte2:8;
	   UInt32 byte3:8;
   } bytes;
} UInt32Bytes;

#endif /* DATATYPES_H_ */
