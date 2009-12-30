/**
 * @file
 * This file contains the implementation of some common functions
 * in Move, which is the abstract base class for moves in mcmc
 * inference.
 *
 * @brief Declaration of Move
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "DAGNode.h"
#include "Move.h"
#include "RbComplex.h"
#include "RbNames.h"
#include "StringVector.h"

#include <iostream>
#include <string>
#include <vector>


/** Constructor */
Move::Move(VariableNode* node, RandomNumberGenerator* rgen) {

    theNode = node;
    rng = rgen;
}


/** Accept the move: update statistics and call derived method */
void Move::acceptMove(void) {

    numAccepted++;
    accept();
}


/** Calculate acceptance probability */
double Move::getAcceptanceProbability(void) {

    return numAccepted/(double)numTried;
}


/** Get class vector describing type of object */
const StringVector& Move::getClass(void) const { 

    static StringVector rbClass = StringVector(Move_name) + RbComplex::getClass();
	return rbClass;
}


/** Perform the move: update statistics and call derived method */
double Move::performMove(void) {

    numTried++;
    return perform();
}


/** Reject the move: call derived method so we can restore node(s) */
void Move::rejectMove(void) {

    reject();
}


