/*
 * VarList.cpp
 *
 *  Created on: 19 но€б. 2015 г.
 *      Author: cno
 */

#include <Pult/MenuModels/ViewLists.h>

volatile float viewListValue0,viewListValue1, viewListValue2;
static volatile float editViewListValue0=0;
static volatile float editViewListValue1=0;
static volatile float editViewListValue2=0;
static volatile float editViewListValue3=0;
static volatile float editViewListValue4=0;
static volatile float editViewListValue5=0;
static volatile float editViewListValue6=0;
static volatile float editViewListValue7=0;


Var var0("Var0: ", &viewListValue0), var1("Var1: ", &viewListValue1), var2("Var2: ", &viewListValue2);
EditVar
	editVar0("EditVar0: ", &editViewListValue0),
	editVar1("EditVar1: ", &editViewListValue1),
	editVar2("EditVar2: ", &editViewListValue2),
	editVar3("EditVar3: ", &editViewListValue3),
	editVar4("EditVar4: ", &editViewListValue4),
	editVar5("EditVar5: ", &editViewListValue5),
	editVar6("EditVar6: ", &editViewListValue6),
	editVar7("EditVar7: ", &editViewListValue7);

ViewLists::ViewLists() {
		viewListValue0=1.1;
		viewListValue1=2.2;
		viewListValue2=2.1;
	    varList.add(&var0);
		varList.add(&var1);
		varList.add(&var2);


		editVarList.add(&editVar0);
		editVarList.add(&editVar1);
		editVarList.add(&editVar2);
		editVarList.add(&editVar3);
		editVarList.add(&editVar4);
		editVarList.add(&editVar5);
		editVarList.add(&editVar6);
		editVarList.add(&editVar7);

}

List<Var*>* ViewLists::getVarList() {
	return &varList;
}

List<EditVar*>* ViewLists::getEditVarList() {
	return &editVarList;
}
