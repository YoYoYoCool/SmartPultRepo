/*
 * VarList.h
 *
 *  Created on: 19 но€б. 2015 г.
 *      Author: cno
 */

#ifndef PULT_MENUMODELS_VIEWLISTS_H_
#define PULT_MENUMODELS_VIEWLISTS_H_
#include <../GyConCommon/Utils/Collections.h>

#define VARIABLES_COUNT 3
#define EDIT_VARIABLES_COUNT 8

class Var {
protected:
	volatile float *valuePtr;
public:
	Var(const char* name, volatile float *valuePtr):name(name),valuePtr(valuePtr){};
    const char* name;
    float getValue()  {return  *valuePtr;}
};

class EditVar: public Var {
public:
	EditVar(const char* name, volatile float* valuePtr):Var(name,valuePtr){};
    void setNewValue(float value){*valuePtr = value;};
};

class ViewLists {
private:
	StaticList<Var*, VARIABLES_COUNT> varList;
	StaticList<EditVar*, EDIT_VARIABLES_COUNT> editVarList;
public:
	ViewLists();
	List<Var*>* getVarList();
	List<EditVar*>* getEditVarList();
};


#endif /* PULT_MENUMODELS_VIEWLISTS_H_ */
