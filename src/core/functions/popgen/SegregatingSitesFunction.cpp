#include "SegregatingSitesFunction.h"
#include "RbException.h"

using namespace RevBayesCore;

SegregatingSitesFunction::SegregatingSitesFunction(const TypedDagNode<AbstractDiscreteCharacterData> *a) : TypedFunction<int>( new int(0) ),
    alignment( a )
{
    // add the lambda parameter as a parent
    addParameter( alignment );
    
    update();
}


SegregatingSitesFunction::~SegregatingSitesFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



SegregatingSitesFunction* SegregatingSitesFunction::clone( void ) const
{
    
    return new SegregatingSitesFunction( *this );
}


void SegregatingSitesFunction::update( void )
{
    
    *value = int( alignment->getValue().getNumberOfSegregatingSites() );

}



void SegregatingSitesFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == alignment)
    {
        alignment = static_cast<const TypedDagNode< AbstractDiscreteCharacterData >* >( newP );
    }
    
}


