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
#include "StringVector.h"

const StringVector RbMove::rbClass = StringVector("move") + RbComplex::rbClass;

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

void RbMove::rejectMove(void) {
    reject();
}

double RbMove::getAcceptanceProbability(void) {
    return nAcceptances/(double)nTries;
}


