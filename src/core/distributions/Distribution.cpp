#include "DagNode.h"
#include "Distribution.h"
#include "RbException.h"

using namespace RevBayesCore;



Distribution::Distribution(void)  :
    parameters()
{
    
}


Distribution::Distribution(const Distribution &f)  :
    parameters( f.parameters )
{
    
    for (std::set<const DagNode*>::iterator it=parameters.begin(); it!=parameters.end(); ++it)
    {
        (*it)->incrementReferenceCount();
    }
    
}


Distribution::~Distribution( void )
{
    
    for (std::set<const DagNode*>::iterator it=parameters.begin(); it!=parameters.end(); ++it)
    {
        const DagNode *theNode = *it;
        if ( theNode->decrementReferenceCount() == 0 )
        {
            delete theNode;
        }
    }
    
}



Distribution& Distribution::operator=(const Distribution &f)
{
    
    if ( this != &f )
    {
        
        for (std::set<const DagNode*>::iterator it=parameters.begin(); it!=parameters.end(); ++it)
        {
            const DagNode *theNode = *it;
            if ( theNode->decrementReferenceCount() == 0 )
            {
                delete theNode;
            }
        }
        parameters.clear();
        
        parameters = f.parameters;
        
        for (std::set<const DagNode*>::iterator it=parameters.begin(); it!=parameters.end(); ++it)
        {
            (*it)->incrementReferenceCount();
        }
    }
    
    return *this;
}


/* Method stub: override for specialized treatment. */
void Distribution::getAffected(std::set<DagNode *> &affected, DagNode* affecter)
{
    // do nothing
}


/**
 * Get a const reference to the set of parameters for this distribution.
 */
const std::set<const DagNode*>& Distribution::getParameters( void ) const {
    
    return parameters;
}


/* Method stub: override for specialized treatment. */
void Distribution::keep( DagNode* affecter )
{
    // delegate to specialization
    keepSpecialization(affecter);
    
}


/* Method stub: override for specialized treatment. */
void Distribution::keepSpecialization( DagNode* affecter )
{
    // do nothing
}


/* Method stub: override for specialized treatment. */
void Distribution::reInitialized( void )
{
    // do nothing
}


/**
 * Remove this parameter from our list of parameters.
 */
void Distribution::removeParameter(const RevBayesCore::DagNode *p)
{
    std::set<const DagNode *>::iterator it = parameters.find( p );
    if ( it != parameters.end() )
    {
        parameters.erase( it );
    }
    
    if ( p->decrementReferenceCount() == 0 )
    {
        delete p;
    }
}


/* Method stub: override for specialized treatment. */
void Distribution::restore( DagNode *restorer )
{
    // delegate to specialization
    restoreSpecialization( restorer );
}


/* Method stub: override for specialized treatment. */
void Distribution::restoreSpecialization( DagNode *restorer )
{
    // do nothing
}

/**
 * Swap the old parameter with a new one.
 * This will be called for example when the entire model graph is cloned or
 * when we replace a variable with the same name (re-assignment).
 * Here we update our set and delegate to the derived class.
 */
void Distribution::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    std::set<const DagNode *>::iterator position = parameters.find(oldP);
    if ( position != parameters.end() )
    {
        parameters.erase( position );
        parameters.insert( newP );
        swapParameterInternal( oldP, newP );
        
        // increment and decrement the reference counts
        newP->incrementReferenceCount();
        if ( oldP->decrementReferenceCount() == 0 )
        {
            throw RbException("Memory leak in Function. Please report this bug to Sebastian.");
        }
    }
    else
    {
        throw RbException("Could not find the parameter to be swapped.");
    }
    
}


/* Method stub: override for specialized treatment. */
void Distribution::touch( DagNode *toucher )
{
    // do some general stuff for all distributions ...
    
    // delegate to specialization
    touchSpecialization( toucher );
}


/* Method stub: override for specialized treatment. */
void Distribution::touchSpecialization( DagNode *toucher )
{
    // do nothing
}

