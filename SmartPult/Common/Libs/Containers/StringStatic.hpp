/*
 * StaticString.h
 *
 *  Created on: 30 мая 2017 г.
 *      Author: KapustinIV
 */

#ifndef COMMONLIBS_UTILS_CONTAINERS_STATICSTRING_H_
#define COMMONLIBS_UTILS_CONTAINERS_STATICSTRING_H_

#include "String.hpp"

namespace Containers {

template<size_t maxLen>
class StringStatic: public String {
private:
    char buf[maxLen + 1];
public:
    inline StringStatic():String(buf, maxLen+1) {;};
    inline StringStatic(const char* str):String(buf, maxLen+1, str) {;};
    inline StringStatic(const char* str,size_t length):length(length),
            String(buf, maxLen+1, str) {;};

};

} /* namespace Containers */

#endif /* COMMONLIBS_UTILS_CONTAINERS_STATICSTRING_H_ */
