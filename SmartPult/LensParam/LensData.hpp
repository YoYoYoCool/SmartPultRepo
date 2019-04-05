/*
 * LensData.hpp
 *
 *  Created on: 15 февр. 2019 г.
 *      Author: Cheverev kirill
 */

#ifndef LENSPARAM_LENSDATA_HPP_
#define LENSPARAM_LENSDATA_HPP_

#include "stddef.h"
#include "stdint.h"
#include "Libs/Utils/Debug.hpp"
#include "Libs/Containers/List.hpp"
#include "Libs/Containers/String.hpp"

namespace LensDb {

enum {
    longString=5
};

struct LensPoint {
    float position;
    float percent;
};

class LensAxis {
public:
    LensAxis(Containers::List<LensPoint>& pointsList, bool measurementSystem) :
        pointsList(pointsList),
        measurementSystem(measurementSystem){
    };
    inline void setPoint(uint32_t pointId, LensPoint& point) {
        pointsList.set(pointId, point);
    }

    inline LensPoint& point(uint32_t pointId) {
        return pointsList.get(pointId);
    }
    inline LensPoint* points() {
        return &pointsList[0];
    }
    inline void setSize(size_t size) {pointsList.setSize(size); }

    inline size_t getSize() { return pointsList.getSize(); }
private:
    Containers::List<LensPoint>& pointsList;
    bool measurementSystem;

};

template<size_t maxPoints>
class LensAxisStatic : public LensAxis {
private:
    Containers::ListStatic<LensPoint, maxPoints> pointsList;
public:
    LensAxisStatic(bool measurementSystem=false) : LensAxis(pointsList,measurementSystem) {
    }
};

class LensObjective {
public:

    LensObjective(
            Containers::String& name,
            LensAxis& zoom,
            LensAxis& iris,
            LensAxis& focus) : _name(name),
            _zoom(zoom), 
            _iris(iris), 
            _focus(focus){
        
    }

    inline LensAxis& zoom()    {return _zoom; };
    inline LensAxis& iris()    {return _iris; };
    inline LensAxis& focus()   {return _focus;};
    inline Containers::String& name()     {return _name; };

    inline void setName(Containers::String& name) {
        _name.copy(name);
    }
private:
    Containers::String& _name;
    LensAxis& _zoom;
    LensAxis& _iris;
    LensAxis& _focus;
};

template<size_t maxPoints>
class LensObjectiveStatic : public LensObjective {
public:
    LensObjectiveStatic(Containers::String& name):
        LensObjective(
                name,
                _zoom,
                _iris,
                _focus
                ){

    }
private:
    LensAxisStatic<maxPoints> _zoom;
    LensAxisStatic<maxPoints> _iris;
    LensAxisStatic<maxPoints> _focus;
};

}
#endif /* LENSPARAM_LENSPARAM_HPP_ */
