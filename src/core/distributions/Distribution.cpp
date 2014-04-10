#include "DagNode.h"
#include "Distribution.h"

using namespace RevBayesCore;

Distribution::Distribution( void ) : parameters( std::set<const DagNode*>() ) {
    
}



void Distribution::addParameter(const DagNode *p) {
    
    parameters.insert( p );
    
}


void Distribution::getAffected(std::set<DagNode *> &affected, DagNode* affecter) {
    // do nothing
    
}


const std::set<const DagNode*>& Distribution::getParameters( void ) const {
    
    return parameters;
}


/* Method stumb that can be overwritten for specialized treatment. */
void Distribution::keep( DagNode* affecter ) {
    // delegate to specialization
    keepSpecialization(affecter);
    
}

/* Method stumb that can be overwritten for specialized treatment. */
void Distribution::keepSpecialization( DagNode* affecter ) {
    // do nothing
    
}


void Distribution::reInitialized( void ) {
    // do nothing
}


void Distribution::removeParameter(const DagNode *p) {
    
    std::set<const DagNode *>::iterator it = parameters.find( p );
    if ( it != parameters.end() ) 
    {
        parameters.erase( it );
    }
    
}


/* Method stumb that can be overwritten for specialized treatment. */
void Distribution::restore( DagNode *restorer ) {
    // delegate to specialization
    restoreSpecialization( restorer );
    
}


/* Method stumb that can be overwritten for specialized treatment. */
void Distribution::restoreSpecialization( DagNode *restorer ) {
    // do nothing
    
}


/* Method stumb that can be overwritten for specialized treatment. */
void Distribution::touch( DagNode *toucher ) {
    // do some general stuff for all distributions ...
    
    
    
    // delegate to specialization
    touchSpecialization( toucher );
}


/* Method stumb that can be overwritten for specialized treatment. */
void Distribution::touchSpecialization( DagNode *toucher ) {
    // do nothing
}

