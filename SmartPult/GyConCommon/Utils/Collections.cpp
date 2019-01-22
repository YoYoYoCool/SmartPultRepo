/*
 * Collections.cpp
 *
 *  Created on: 18 но€б. 2015 г.
 *      Author: cno
 */

//#include <Utils/Collections.h>
//
//template<typename ElementType>
//inline List<ElementType>::List() {
//	maxSize = 0;
//	size = 0;
//}
//
//template<typename ElementType>
//inline ElementType** List<ElementType>::getAll() {
//	return elements;
//}
//
//template<typename ElementType>
//inline ElementType* List<ElementType>::get(UInt32 id) {
//	if (id>=size) id = size - 1;
//	return elements[id];
//}
//
//
//template<typename ElementType>
//inline UInt32 List<ElementType>::getSize() {
//	return size;
//}
//
//template<typename ElementType>
//inline UInt32 List<ElementType>::getMaxSize() {
//    return maxSize;
//}
//
//
//template<typename ElementType>
//inline bool List<ElementType>::add(ElementType* element) {
//	if (size<maxSize) {
//		elements[size++] = element;
//		return true;
//	} else {
//		return false;
//	}
//}
//
//template<typename ElementType>
//inline bool List<ElementType>::set(UInt32 id, ElementType* element) {
//	if (id>=size) {
//		return false;
//	} else {
//        elements[id] = element;
//        return true;
//	}
//}
//
//template<typename ElementType>
//inline void List<ElementType>::remove() {
//    if (size>0) {
//    	size--;
//    	return true;
//    } else {
//    	return false;
//    }
//}
//
//template<typename ElementType>
//inline void List<ElementType>::removeAll() {
//	size = 0;
//}
//
//template<typename ElementType>
//inline bool List<ElementType>::addAll(List<ElementType> *other) {
//	bool res = true;
//    UInt32 maxId = size + other->getSize() - 1;
//    if (maxId >= maxSize) {maxId = maxSize - 1;}
//
//    for (int i = size; i<=maxId; i++) {
//        this->elements[i] = other->elements[i];
//    }
//    return result;
//}
//
//template<typename ElementType, UInt32 maxSize>
//inline StaticList<ElementType, maxSize>::StaticList() {
//		size = 0;
//		this->maxSize = maxSize;
//		this->elements = staticElements;
//}
//
//




