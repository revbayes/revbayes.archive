#include "DagNode.h"
#include "Function.h"
#include "RbException.h"

using namespace RevBayesCore;



Function::Function(void)  :
    dirty( true ),
    parameters()
{
    
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


/**
 * Method stumb that can be overwritten for specialized treatment.
 * Note that it is extremely important NOT to set the dirty flag to
 * false here, because we may have been touched without having been
 * updated.
 */
void Function::keep( DagNode* affecter ) {

    // restore flags
//    dirty = false;
}


void Function::removeParameter(const RevBayesCore::DagNode *p)
{
    std::set<const DagNode *>::iterator it = parameters.find( p );
    if ( it != parameters.end() )
    {
        parameters.erase( it );
    }
}


void Function::reInitialized( void )
{
    // do nothing
}


/* Method stub that can be overwritten for specialized treatment. */
void Function::restore( DagNode *restorer ) {
    
    // restore flags
//    dirty = false;
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


/* Method stub that can be overwritten for specialized treatment. */
void Function::touch( DagNode *toucher ) {
    // do nothing
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const Function& f) {
    
    o << "f(x)";
    
    return o;
}
