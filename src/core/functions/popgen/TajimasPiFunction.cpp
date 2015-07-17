#include "TajimasPiFunction.h"
#include "RbException.h"

using namespace RevBayesCore;

TajimasPiFunction::TajimasPiFunction(const TypedDagNode<AbstractHomologousDiscreteCharacterData> *a, bool ps) : TypedFunction<double>( new double(0.0) ),
    alignment( a ),
    perSite( ps )
{
    // add the lambda parameter as a parent
    addParameter( alignment );
    
    update();
}


TajimasPiFunction::~TajimasPiFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



TajimasPiFunction* TajimasPiFunction::clone( void ) const
{
    
    return new TajimasPiFunction( *this );
}


void TajimasPiFunction::update( void )
{
    double pd  = alignment->getValue().getPaiwiseSequenceDifference();
    *value = pd;
    
    if ( perSite == true)
    {
        int nSites = int( alignment->getValue().getNumberOfCharacters() );
        *value /= nSites;
    }
    
}



void TajimasPiFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == alignment)
    {
        alignment = static_cast<const TypedDagNode< AbstractHomologousDiscreteCharacterData >* >( newP );
    }
    
}


