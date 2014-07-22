#include "DagNode.h"
#include "Distribution.h"

using namespace RevBayesCore;


/** Default constructor: nothing to do here. */
Distribution::Distribution( void )
{
}


/* Method stub: override for specialized treatment. */
void Distribution::getAffected(std::set<DagNode *> &affected, DagNode* affecter)
{
    // do nothing
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

