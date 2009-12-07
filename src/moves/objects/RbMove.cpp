/*
 * RbMove.cpp
 *
 *  Created on: 25 nov 2009
 *      Author: Sebastian
 */

#include <iostream>
#include <string>
#include <vector>

#include "DAGNode.h"
#include "RbComplex.h"
#include "RbMove.h"
#include "RbNames.h"
#include "StringVector.h"




RbMove::RbMove(DAGNode* n, RandomNumberGenerator* r) {
    node = n;
    rng = r;
}

RbMove::~RbMove(void){

}


double RbMove::performMove(void) {

    nTries++;
    perform();
}

void RbMove::acceptMove(void) {

    nAcceptances++;
    accept();
}

const StringVector& RbMove::getClass(void) const { 

    static StringVector rbClass = StringVector(RbNames::Move::name) + RbComplex::getClass();
	return rbClass;
}

void RbMove::rejectMove(void) {

    reject();
}

double RbMove::getAcceptanceProbability(void) {

    return nAcceptances/(double)nTries;
}


