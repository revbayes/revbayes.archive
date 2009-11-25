/*
 * RbModel.cpp
 *
 *  Created on: 25 nov 2009
 *      Author: Sebastian
 */

#include "DAGNode.h"
#include "RbModel.h"
#include "StringVector.h"

const StringVector RbModel::rbClass = StringVector("model") + RbComplex::rbClass;

RbModel::RbModel(std::vector<DAGNode*>& s) {

    sinkDags = s;
}

RbModel::RbModel(const RbModel& m) {

    sinkDags = m.sinkDags;
}

RbModel::~RbModel() {
    // TODO Auto-generated destructor stub
}

/**
 * @brief Pointer-based equal comparison
 *
 * Compares equality of this object to another RbObject.
 *
 * @param obj   The object of the comparison
 * @returns     Result of comparison
 *
 */
bool RbModel::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
    const RbModel* x = dynamic_cast<const RbModel*>(obj);
    if (x != NULL)
        return value == x->value;

    // Try converting the value to a double
    x = dynamic_cast<const RbModel*>(obj->convertTo("model"));
    if (x == NULL)
        return false;

    bool result = true;
    if (sink == x->sink)
        {
        result = true;
        }
    else
        result = false;
    delete x;
    return result;
}

void RbModel::print(std::ostream& o) const {

    o << "Model:" << std::endl;
    for (int i=0; i<sinkDags.size(); i++)
    	sink[i]->print(o);
}

RbObject* RbModel::clone(void) const {

    RbModel* x = new RbModel(*this);
    return (RbObject*)x;
}

void RbModel::printValue(std::ostream& o) const {

    sink->printValue(o);
}

std::string RbModel::toString(void) const {

    std::string tempStr = "Model\n";
	//tmpStr += sink->toString();
    return tempStr;
}
