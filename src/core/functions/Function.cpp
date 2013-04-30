//
//  Function.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 9/4/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DagNode.h"
#include "Function.h"
#include "RbException.h"

using namespace RevBayesCore;

Function::Function(void)  : parameters( std::set<const DagNode*>() ) {
    
}



void Function::addParameter(const DagNode *p) {
    
    parameters.insert( p );

}


void Function::getAffected(std::set<DagNode *> &affected, DagNode* affecter) {
    
    // do nothing
}


const std::set<const DagNode*>& Function::getParameters( void ) const {
    
    return parameters;
}


/* Method stumb that can be overwritten for specialized treatment. */
void Function::keep( DagNode* affecter ) {
    // do nothing
}


/* Method stumb that can be overwritten for specialized treatment. */
void Function::restore( DagNode *restorer ) {
    // do nothing
}


void Function::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    std::set<const DagNode *>::iterator position = parameters.find(oldP);
    if ( position != parameters.end() ) 
    {
        parameters.erase( position );
        parameters.insert( newP );
        swapParameterInternal( oldP, newP );
    } 
    else 
    {
        throw RbException("Could not find the parameter to be swapped.");
    }
    
}


/* Method stumb that can be overwritten for specialized treatment. */
void Function::touch( DagNode *toucher ) {
    // do nothing
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const Function& f) {
    
    o << "f(x)";
    
    return o;
}
