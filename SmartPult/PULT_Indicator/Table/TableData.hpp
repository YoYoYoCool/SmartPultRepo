/*
 * TableData.hpp
 *
 *  Created on: 22 мар. 2019 г.
 *      Author: Cheverev Kirill
 */

#ifndef PULT_INDICATOR_TABLEDATA_HPP_
#define PULT_INDICATOR_TABLEDATA_HPP_
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

template <typename TData,size_t sizeStringData, size_t sizeColumnData>
class TableData {
private:
    TData tableData[sizeStringData][sizeColumnData];
public:
    TableEdit() {
        for (size_t idStr=0; idStr<sizeStringData; idStr++) {
            for (size_t idColumn=0; idColumn<sizeColumnData; idColumn++) {
                tableData[idStr][idColumn]=0;
            }
        }
    }

    inline void setTableData (const Orientation & position, const TData data) {
        tableData[position.string][position.column]=data;
    }

    inline const TData getTableData (const Orientation & position) {
        return tableData[position.string][position.column];
    }
};


}



#endif /* PULT_INDICATOR_TABLEDATA_HPP_ */
