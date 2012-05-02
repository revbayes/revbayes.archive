//
//  InferenceMove.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 4/16/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "InferenceMove.h"
#include "StochasticInferenceNode.h"

InferenceMove::InferenceMove( void ) {
    
}


InferenceMove::~InferenceMove( void ) {
    
}


void InferenceMove::accept( void ) {
    // increment the counter for accepted
    numAccepted++;
    
    // call accept for each node
    for (std::vector<StochasticInferenceNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i) {
        (*i)->keep();
    }
    
    // delegate the call to the derived class
    acceptMove();
}



double InferenceMove::getAcceptanceRatio( void ) const {
    return numTried / double( numAccepted );
}


double InferenceMove::getUpdateWeight( void ) const {
    return weight;
}


double InferenceMove::perform(double &probRatio) {
    // increment the counter for tried
    numTried++;
    
//    // call accept for each node
//    for (std::vector<StochasticInferenceNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i) {
//        (*i)->keep();
//    }
    
    // delegate the call to the derived class
    return performMove(probRatio);
}


void InferenceMove::reject( void ) {
    
    // call restore for each node
    for (std::vector<StochasticInferenceNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i) {
        (*i)->restore();
    }
    
    // delegate the call to the derived class
    rejectMove();
}


void InferenceMove::resetCounters( void ) {
    numTried = 0;
    numAccepted = 0;
}


void InferenceMove::setArguments(const std::vector<StochasticInferenceNode *> &args) {
    nodes = args;
    
    // delegate
    setInternalArguments( args );
}