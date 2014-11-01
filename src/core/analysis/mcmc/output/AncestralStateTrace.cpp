/*
 *  AncestralStateTrace.cpp
 *  revbayes-proj
 *
 *  Created by mucho mass on 10/16/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */


#include "AncestralStateTrace.h"
#include "RbUtil.h"

#include <sstream>
#include <string>
#include <vector>

using namespace RevBayesCore;

AncestralStateTrace::AncestralStateTrace() {
    invalidate();
}

/**
 * Copy constructor
 *
 */

AncestralStateTrace::AncestralStateTrace(const AncestralStateTrace& t) {
    burnin                          = t.burnin;
    ess                             = t.ess;
    stepSize                        = t.stepSize;
    
    converged                       = t.converged;
    passedStationarityTest          = t.passedStationarityTest;
    passedGewekeTest                = t.passedGewekeTest;
    //    passedHeidelbergerWelchStatistic = NOT_CHECKED;
    //    passedRafteryLewisStatistic = NOT_CHECKED;
    passedEssThreshold              = t.passedEssThreshold;
    passedSemThreshold              = t.passedSemThreshold;
    passedIidBetweenChainsStatistic = t.passedIidBetweenChainsStatistic;
    passedGelmanRubinTest           = t.passedGelmanRubinTest;
    
    values                          = t.values;
	parmName                        = t.parmName;
}



AncestralStateTrace::~AncestralStateTrace() {
    values.clear();
}



void AncestralStateTrace::addObject(const std::string &t) {
    values.push_back(t);
    
    // invalidate for recalculation of meta data
    invalidate();
}

/** Clone function */

AncestralStateTrace* AncestralStateTrace::clone() const {
    
    return new AncestralStateTrace(*this);
}



void AncestralStateTrace::invalidate() {
    // set values to defaults and mark for recalculation
    burnin                          = -1;
    ess                             = -1;
    stepSize                        = 1;
    
    converged                       = NOT_CHECKED;
    passedStationarityTest          = NOT_CHECKED;
    passedGewekeTest                = NOT_CHECKED;
    //    passedHeidelbergerWelchStatistic = NOT_CHECKED;
    //    passedRafteryLewisStatistic = NOT_CHECKED;
    passedEssThreshold              = NOT_CHECKED;
    passedSemThreshold              = NOT_CHECKED;
    passedIidBetweenChainsStatistic = NOT_CHECKED;
    passedGelmanRubinTest           = NOT_CHECKED;
    
    
}

/** Print value for user */

void AncestralStateTrace::printValue(std::ostream &o) const {
    
    o << "AncestralStateTrace values to be printed ...";
}



void AncestralStateTrace::removeObjectAtIndex (int index){
    // create a iterator for the vector
    std::vector<std::string>::iterator it = values.begin();
    
    //jump to the position to remove
    it += index;
    
    // remove the element
    values.erase(it);
    
    // invalidate for recalculation of meta data
    invalidate();
}


void AncestralStateTrace::removeLastObject() {
    // remove object from list
    values.pop_back();
    
    // invalidate for recalculation of meta data
    invalidate();
}
