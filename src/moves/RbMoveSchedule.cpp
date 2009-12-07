/*
 * RbMoveSchedule.cpp
 *
 *  Created on: 25 nov 2009
 *      Author: Sebastian
 */

#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMoveSchedule.h"
#include "RbNames.h"

#include <map>


RbMoveSchedule::RbMoveSchedule(RandomNumberGenerator* r, double w) {
    rng = r;
    sumWeights = 0.0;
    dagUpdateWeight = w;
}

RbMoveSchedule::~RbMoveSchedule() {
    // TODO Auto-generated destructor stub
}

void RbMoveSchedule::addMove(RbMove* m, double w) {

    sumWeights += w;
    schedule.insert(std::make_pair(w,m));
}

RbObject* RbMoveSchedule::clone() const {
    RbMoveSchedule* x = new RbMoveSchedule(*this);
    return (RbObject*) x;
}

const StringVector& RbMoveSchedule::getClass(void) const { 

    static StringVector rbClass = StringVector(RbNames::MoveSchedule::name) + RbComplex::getClass();
	return rbClass;
}

bool RbMoveSchedule::equals(const RbObject* obj) const {
    return false;
}


/** Get class vector describing type of object */
const StringVector& RbMoveSchedule::getClass() const {

    static StringVector rbClass = StringVector(RbNames::RbMoveSchedule::name) + RbComplex::getClass();
    return rbClass;
}


void RbMoveSchedule::print(std::ostream& o) const {
    o << "move schedule" << std::endl;
}

void RbMoveSchedule::printValue(std::ostream& o) const {
    o << "move schedule" << std::endl;
}

std::string RbMoveSchedule::toString(void) const {
    return "move schedule";
}

RbObject& RbMoveSchedule::operator=(const RbObject& obj) {
    try {
        // Use built-in fast down-casting first
        const RbMoveSchedule& x = dynamic_cast<const RbMoveSchedule&> (obj);

        RbMoveSchedule& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbMoveSchedule& x = dynamic_cast<const RbMoveSchedule&> (*(obj.convertTo(
                    "moveSchedule")));

            RbMoveSchedule& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0]
                    + " to moveSchedule");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbMoveSchedule& RbMoveSchedule::operator=(const RbMoveSchedule& obj) {
    rng = obj.rng;
    schedule = obj.schedule;
    sumWeights = obj.sumWeights;
}

RbMove* RbMoveSchedule::getNext() {

	double ran = rng->nextDouble() * sumWeights;
    std::map<double,RbMove*>::iterator i = schedule.begin();
    ran -= i->first;
    while ((ran-0.00000001) > 0) {
        i++;
        ran -= i->first;
    }
    return i->second;
}
