#include "SumFunction.h"

using namespace RevBayesCore;

SumFunction::SumFunction(const TypedDagNode<std::vector<double> > *v) : TypedFunction<double>( new double(0.0) ), vals( v )
{
    // add the parameters as parents
    this->addParameter( vals );
    
    update();
}


SumFunction::~SumFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



SumFunction* SumFunction::clone( void ) const
{
    return new SumFunction( *this );
}


void SumFunction::update( void )
{
    
    double m = 0;
    const std::vector<double> &v = vals->getValue();
    for ( std::vector<double>::const_iterator it = v.begin(); it != v.end(); ++it)
    {
        m += *it;
    }
    
    *this->value = m ;
    
}



void SumFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if ( oldP == vals )
    {
        vals = static_cast<const TypedDagNode<std::vector<double> >* >( newP );
    }
    
}

