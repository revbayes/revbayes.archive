/*
 * RbMoveSchedule.cpp
 *
 *  Created on: 25 nov 2009
 *      Author: Sebastian
 */

#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMove.h"
#include "RbMoveSchedule.h"
#include "RbNames.h"

#include <map>


RbMoveSchedule::RbMoveSchedule(RandomNumberGenerator* r, double w) {
    rng = r;
    sumWeights = 0.0;
    dagUpdateWeight = w;
}

RbMoveSchedule::RbMoveSchedule(const RbMoveSchedule& ms) {
    rng = ms.rng;
    sumWeights = ms.sumWeights;
    dagUpdateWeight = ms.dagUpdateWeight;

    for (std::map<double,RbMove*>::const_iterator it=ms.schedule.begin(); it!=ms.schedule.end(); it++) {

        double w = it->first;
        RbMove* m = (RbMove*)it->second->clone();
        schedule.insert(std::make_pair(w,m));
    }
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


void RbMoveSchedule::print(std::ostream& o) const {
    o << "move schedule" << std::endl;
}

void RbMoveSchedule::printValue(std::ostream& o) const {
    o << "move schedule" << std::endl;
}

std::string RbMoveSchedule::toString(void) const {
    return "move schedule";
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
