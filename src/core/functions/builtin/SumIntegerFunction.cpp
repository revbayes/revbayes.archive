#include "SumIntegerFunction.h"

using namespace RevBayesCore;

SumIntegerFunction::SumIntegerFunction(const TypedDagNode<RbVector<int> > *v) : TypedFunction<int>( new int(0.0) ), vals( v )
{
    // add the parameters as parents
    this->addParameter( vals );
    
    update();
}


SumIntegerFunction::~SumIntegerFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



SumIntegerFunction* SumIntegerFunction::clone( void ) const
{
    return new SumIntegerFunction( *this );
}


void SumIntegerFunction::update( void )
{
    
    double m = 0;
    const RbVector<int> &v = vals->getValue();
    for ( RbConstIterator<int> it = v.begin(); it != v.end(); ++it)
    {
        m += *it;
    }
    
    *this->value = m ;
    
}



void SumIntegerFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if ( oldP == vals )
    {
        vals = static_cast<const TypedDagNode<RbVector<int> >* >( newP );
    }
    
}

