#include "DagNode.h"
#include "Distribution.h"
#include "RbException.h"

#include <algorithm>

using namespace RevBayesCore;



Distribution::Distribution(void) : Parallelizable(),
    parameters()
{
    
}


Distribution::Distribution(const Distribution &d) : Parallelizable(d),
    parameters( d.parameters )
{
    
    for (std::vector<const DagNode*>::iterator it=parameters.begin(); it!=parameters.end(); ++it)
    {
        (*it)->incrementReferenceCount();
    }
    
}


Distribution::~Distribution( void )
{
    
    for (std::vector<const DagNode*>::iterator it=parameters.begin(); it!=parameters.end(); ++it)
    {
        const DagNode *theNode = *it;
        if ( theNode->decrementReferenceCount() == 0 )
        {
            delete theNode;
        }
    }
    
}



Distribution& Distribution::operator=(const Distribution &d)
{
    Parallelizable::operator=(d);
    
    if ( this != &d )
    {
        
        for (std::vector<const DagNode*>::iterator it=parameters.begin(); it!=parameters.end(); ++it)
        {
            const DagNode *theNode = *it;
            if ( theNode->decrementReferenceCount() == 0 )
            {
                delete theNode;
            }
        }
        parameters.clear();
        
        parameters = d.parameters;
        
        for (std::vector<const DagNode*>::iterator it=parameters.begin(); it!=parameters.end(); ++it)
        {
            (*it)->incrementReferenceCount();
        }
    }
    
    return *this;
}


/**
 * Add this parameter to our set of parameters.
 */
void Distribution::addParameter(const DagNode *p)
{
    
    // only if the parameter is not NULL
    if ( p != NULL )
    {
        std::vector<const DagNode*>::iterator pos = std::find(parameters.begin(), parameters.end(), p);
        if ( pos == parameters.end() )
        {
            parameters.push_back( p );
            
            // increment reference count
            p->incrementReferenceCount();
            
        }
    
    }
    
}



void Distribution::executeProcedure(const std::string &n, const std::vector<DagNode *> args, bool &f)
{
    // no function found
}



/* Method stub: override for specialized treatment. */
void Distribution::getAffected(std::set<DagNode *> &affected, DagNode* affecter)
{
    // do nothing
}


/**
 * Get a const reference to the set of parameters for this distribution.
 */
const std::vector<const DagNode*>& Distribution::getParameters( void ) const {
    
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
    
    // only if the parameter is not NULL
    if ( p != NULL )
    {
     
        std::vector<const DagNode *>::iterator it = std::find( parameters.begin(), parameters.end(), p );
        if ( it != parameters.end() )
        {
            parameters.erase( it );
            if ( p->decrementReferenceCount() == 0 )
            {
                delete p;
            }
            
        }
    
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
 * Set the flag if we are in MCMC mode or not.
 * This default implementation does nothing but derived classes can simply overwrite this.
 * It is just a placeholder.
 */
void Distribution::setMcmcMode(bool tf)
{
    // do nothing
}


/**
 * Swap the old parameter with a new one.
 * This will be called for example when the entire model graph is cloned or
 * when we replace a variable with the same name (re-assignment).
 * Here we update our set and delegate to the derived class.
 */
void Distribution::swapParameter(const DagNode *oldP, const DagNode *newP)
{
    
    std::vector<const DagNode *>::iterator position = std::find(parameters.begin(), parameters.end(), oldP);
    if ( position != parameters.end() )
    {
//        parameters.erase( position );
//        parameters.push_back( newP );
        (*position) = newP;
        swapParameterInternal( oldP, newP );
        
        // increment and decrement the reference counts
        newP->incrementReferenceCount();
        if ( oldP->decrementReferenceCount() == 0 )
        {
            throw RbException("Memory leak in distribution. Please report this bug to Sebastian.");
        }
        
    }
    else
    {
        
        throw RbException("Could not find the distribution parameter to be swapped: " + oldP->getName());
    
    }
    
}


/* Method stub: override for specialized treatment. */
void Distribution::touch( DagNode *toucher, bool touchAll )
{
    // do some general stuff for all distributions ...
    
    // delegate to specialization
    touchSpecialization( toucher, touchAll );
}


/* Method stub: override for specialized treatment. */
void Distribution::touchSpecialization( DagNode *toucher, bool touchAll )
{
    // do nothing
}

