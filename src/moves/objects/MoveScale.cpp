/*
 * MoveScale.cpp
 *
 *  Created on: 25 nov 2009
 *      Author: Sebastian
 */

#include "DAGNode.h"
#include "MoveScale.h"
#include "RandomNumberGenerator.h"
#include "RbDouble.h"
#include "RbException.h"
#include "RbModel.h"
#include "RbMove.h"
#include "RbNames.h"
#include "RbObject.h"
#include "StringVector.h"
#include <string>
#include <cmath>


MoveScale::MoveScale(DAGNode* n, RbDouble* tn, RandomNumberGenerator* r) : RbMove(n,r) {

	tuningParm = tn;
}

MoveScale::MoveScale(const MoveScale& m) : RbMove(m.node,m.rng) {

}

MoveScale::~MoveScale() {
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
bool MoveScale::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
    const MoveScale* x = dynamic_cast<const MoveScale*>(obj);
    if (x != NULL)
        return node == x->node;

    // Try converting the value to a double
    x = dynamic_cast<const MoveScale*>(obj->convertTo("scaleMove"));
    if (x == NULL)
        return false;

    bool result = true;
    if (node == x->node)
        {
        result = true;
        }
    else
        result = false;
    delete x;
    return result;
}


/** Get class vector describing type of object */
const StringVector& MoveScale::getClass() const {

    static StringVector rbClass = StringVector(RbNames::MoveScale::name) + RbMove::getClass();
    return rbClass;
}


void MoveScale::print(std::ostream& o) const {

    o << "Scaling Move on:" << std::endl;
    node->printValue(o);
}

RbObject* MoveScale::clone(void) const {

    MoveScale* x = new MoveScale(*this);
    return (RbObject*)x;
}

void MoveScale::printValue(std::ostream& o) const {

    node->printValue(o);
}

std::string MoveScale::toString(void) const {

    std::string tempStr = "Scaling move on:\n";
    //tempStr += node->toString();
    return tempStr;
}


double MoveScale::perform(void) {

	RbObject* tmp = (RbObject*)node->getValue();
	double curVal = ((RbDouble*) tmp)->getValue();
	double tuning = tuningParm->getValue();
	double u = rng->nextDouble();
	double newVal = curVal * exp(tuning*(u-0.5));
	((RbDouble*) tmp)->setValue(newVal);
	
	return log(newVal/curVal);
}

void  MoveScale::accept(void) {
    // for statistic purposes

}

void MoveScale::reject(void) {

}

RbObject& MoveScale::operator=(const RbObject& obj) {


    try {
        // Use built-in fast down-casting first
        const MoveScale& x = dynamic_cast<const MoveScale&> (obj);

        MoveScale& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const MoveScale& x = dynamic_cast<const MoveScale&> (*(obj.convertTo(RbNames::MoveScale::name)));

            MoveScale& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::MoveScale::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbMove& MoveScale::operator=(const RbMove& obj) {


    try {
        // Use built-in fast down-casting first
        const MoveScale& x = dynamic_cast<const MoveScale&> (obj);

        MoveScale& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const MoveScale& x = dynamic_cast<const MoveScale&> (*(obj.convertTo(RbNames::MoveScale::name)));

            MoveScale& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::MoveScale::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}


MoveScale& MoveScale::operator=(const MoveScale& obj) {

    *rng        = *(obj.rng);
    *tuningParm = *obj.tuningParm;
    nAcceptances = obj.nAcceptances;
    nTries      = obj.nTries;
    *node       = *obj.node;
    
    return (*this);
}
