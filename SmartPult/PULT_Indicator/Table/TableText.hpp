/*
 * TableText.hpp
 *
 *  Created on: 22 мар. 2019 г.
 *      Author: User9
 */

#ifndef PULT_INDICATOR_TABLE_TABLETEXT_HPP_
#define PULT_INDICATOR_TABLE_TABLETEXT_HPP_

#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "LCD Classes.h"
#include "LCD Lens.hpp"
#include "LensParam/LensData.hpp"
#include "Libs/Containers/String.hpp"
#include "Libs/Containers/StringStatic.hpp"
#include "Libs/Utils/Debug.hpp"





namespace Table {

template <size_t sizeStringData, size_t sizeColumnData, size_t longStr>
class TableText {
private:
    Containers::StringStatic<longStr> _string[sizeStringData][sizeColumnData];
public:

    TableText () {
        for (size_t idStr=0; idStr<sizeStringData; idStr++) {
            for (size_t idColumn=0; idColumn<sizeColumnData; idColumn++) {
             _string[idStr][idColumn].setChar(0,'0');
             _string[idStr][idColumn].setChar(1,'.');
             _string[idStr][idColumn].setChar(2,'0');
             _string[idStr][idColumn].setChar(3,'\0');
            }
        }
    }

    inline Containers::String & string (const Orientation & position) {
        return _string[position.string][position.column];
        }
    inline Containers::String & string (size_t string, size_t column) {
        return _string[string][column];
        }
    };


}



#endif /* PULT_INDICATOR_TABLE_TABLETEXT_HPP_ */
