#include "HyperbolicTangentFunction.h"
#include <cmath>

using namespace RevBayesCore;

HyperbolicTangentFunction::HyperbolicTangentFunction(const TypedDagNode<double> *a) : ContinuousFunction( new double(0.0) ),
x( a )
{
    addParameter( a );
    
}


HyperbolicTangentFunction* HyperbolicTangentFunction::clone( void ) const
{
    return new HyperbolicTangentFunction(*this);
}


void HyperbolicTangentFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == x)
    {
        x = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}

void HyperbolicTangentFunction::update( void )
{
    
    // recompute and set the internal value
    double x1 = exp( x->getValue() );
    *value = x1 / (1+x1);
    
}


