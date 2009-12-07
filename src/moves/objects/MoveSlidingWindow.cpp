/*
 * MoveSlidingWindow.cpp
 *
 *  Created on: 25 nov 2009
 *      Author: Sebastian
 */

#include "DAGNode.h"
#include "MoveSlidingWindow.h"
#include "RandomNumberGenerator.h"
#include "RbDouble.h"
#include "RbException.h"
#include "RbModel.h"
#include "RbMove.h"
#include "RbNames.h"
#include "RbObject.h"
#include "StringVector.h"
#include <string>


MoveSlidingWindow::MoveSlidingWindow(DAGNode* n, RbDouble* l, RbDouble* u, RbDouble* tn, RandomNumberGenerator* r) : RbMove(n,r) {

	lower = l;
	upper = u;
	tuningParm = tn;
}

MoveSlidingWindow::MoveSlidingWindow(const MoveSlidingWindow& m) : RbMove(m.node,m.rng) {

}

MoveSlidingWindow::~MoveSlidingWindow() {
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
bool MoveSlidingWindow::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
    const MoveSlidingWindow* x = dynamic_cast<const MoveSlidingWindow*>(obj);
    if (x != NULL)
        return node == x->node;

    // Try converting the value to a double
    x = dynamic_cast<const MoveSlidingWindow*>(obj->convertTo((RbNames::MoveSlidingWindow::name)));
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
const StringVector& MoveSlidingWindow::getClass() const {

    static StringVector rbClass = StringVector(RbNames::MoveSlidingWindow::name) + RbMove::getClass();
    return rbClass;
}


void MoveSlidingWindow::print(std::ostream& o) const {

    o << "Sliding Move on:" << std::endl;
    node->print(o);
}

RbObject* MoveSlidingWindow::clone(void) const {

    MoveSlidingWindow* x = new MoveSlidingWindow(*this);
    return (RbObject*)x;
}

void MoveSlidingWindow::printValue(std::ostream& o) const {

    node->printValue(o);
}

std::string MoveSlidingWindow::toString(void) const {

    std::string tempStr = "Sliding move on:\n";
    tempStr += node->toString();
    return tempStr;
}


double MoveSlidingWindow::perform(void) {

	RbObject* tmp = node->getValue();
	double curVal = ((RbDouble*) tmp)->getValue();
	double window = tuningParm->getValue();
	double low = lower->getValue();
	double up = upper->getValue();
	double u = rng->nextDouble();
	double newVal = curVal + (window*(u-0.5));
	do
		{
		if (newVal < low)
			newVal = 2*low - newVal;
		else if (newVal > up)
			newVal = 2.0 * up - newVal;
		} while ( newVal < low || newVal > up );
	((RbDouble*) tmp)->setValue(newVal);
	
	return 0.0;
}

void  MoveSlidingWindow::accept(void) {
    // for statistic purposes

}

void MoveSlidingWindow::reject(void) {

}

RbObject& MoveSlidingWindow::operator=(const RbObject& obj) {


    try {
        // Use built-in fast down-casting first
        const MoveSlidingWindow& x = dynamic_cast<const MoveSlidingWindow&> (obj);

        MoveSlidingWindow& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const MoveSlidingWindow& x = dynamic_cast<const MoveSlidingWindow&> (*(obj.convertTo(RbNames::MoveSlidingWindow::name)));

            MoveSlidingWindow& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::MoveSlidingWindow::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbMove& MoveSlidingWindow::operator=(const RbMove& obj) {


    try {
        // Use built-in fast down-casting first
        const MoveSlidingWindow& x = dynamic_cast<const MoveSlidingWindow&> (obj);

        MoveSlidingWindow& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const MoveSlidingWindow& x = dynamic_cast<const MoveSlidingWindow&> (*(obj.convertTo(RbNames::MoveSlidingWindow::name)));

            MoveSlidingWindow& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::MoveSlidingWindow::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}

MoveSlidingWindow& MoveSlidingWindow::operator=(const MoveSlidingWindow& obj) {

    *lower      = *(obj.lower);
    *upper      = *(obj.upper);
    *rng        = *(obj.rng);
    *tuningParm = *obj.tuningParm;
    nAcceptances = obj.nAcceptances;
    nTries      = obj.nTries;
    *node       = *obj.node;
    
    return (*this);
}
