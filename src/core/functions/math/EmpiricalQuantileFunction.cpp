#include "EmpiricalQuantileFunction.h"

#include <cmath>

using namespace RevBayesCore;

/**
 * Constructor. Here we simply set up the parameter dependencies.
 */
EmpiricalQuantileFunction::EmpiricalQuantileFunction(const TypedDagNode< RbVector<double> > *v, const TypedDagNode<double>* k) : TypedFunction<double>( new double(0.0) ),
    vals( v ),
    kth_quantile( k )
{
    // add the parameters as parents
    this->addParameter( vals );
    this->addParameter( kth_quantile );
    
    update();
}


/**
 * Empty destructor.
 */
EmpiricalQuantileFunction::~EmpiricalQuantileFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



/**
 * Clone function for deep copies.
 */
EmpiricalQuantileFunction* EmpiricalQuantileFunction::clone( void ) const
{
    return new EmpiricalQuantileFunction( *this );
}


/**
 * Update the current value based on the current parameter values.
 */
void EmpiricalQuantileFunction::update( void )
{
    
    std::vector<double> v = vals->getValue();
    std::sort(v.begin(),v.end());
    
    std::size_t index = round( (v.size()-1) * kth_quantile->getValue());
    
    *this->value = v[index];
    
}



/**
 * Swap the internal parameters. This happens when the parameters are re-assigned or the entire model graph is cloned.
 * Here we only need to store the new pointer to the vector of real values.
 */
void EmpiricalQuantileFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if ( oldP == vals )
    {
        vals = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    
    if ( oldP == kth_quantile )
    {
        kth_quantile = static_cast<const TypedDagNode<double>* >( newP );
    }
}
