#include "RateMultiplierPhyloFunction.h"


using namespace RevBayesCore;


/**
 * The constructor does nothing except allocating the object.
 *
 * \param[in]   d    The distribution from which the new values are drawn if a jump occured.
 * \param[in]   t    The time tree along which the process is applied.
 * \param[in]   d    A scaling parameter for the branch length.
 */
RateMultiplierPhyloFunction::RateMultiplierPhyloFunction(const TypedDagNode< TimeTree > *t, const TypedDagNode< RbVector<double> >* rm, const TypedDagNode< double > *br): TypedFunction< RbVector< double > >( new RbVector< double >(t->getValue().getNumberOfNodes() -1, 0.0 ) ),
    baseRate( br ),
    rateMultipliers( rm ),
    tau( t ) 
{
        
    this->addParameter( tau );
    this->addParameter( rateMultipliers );
    this->addParameter( baseRate );
    
    update();
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the function. 
 */
RateMultiplierPhyloFunction* RateMultiplierPhyloFunction::clone(void) const 
{
    
    return new RateMultiplierPhyloFunction( *this );
}


/**
 * Recursively update the branch rates for the children of this node and 
 * call recursiveUpdate for its children.
 *
 * \param[in]    n   The node below which we need to update the branch rates.
 */
void RateMultiplierPhyloFunction::recursiveUpdate(const RevBayesCore::TopologyNode &n, double parentValue)    
{
    
    for (size_t i = 0; i < n.getNumberOfChildren(); ++i) 
    {
        const TopologyNode& child = n.getChild( i );
        size_t childIndex = child.getIndex();
        double v = parentValue * rateMultipliers->getValue()[childIndex];
        (*value)[childIndex] = v;
        
        // only if the child is not a tip we continue the recursion
        if ( !child.isTip() ) 
        {
            recursiveUpdate( child, v );
        }
    }
        
}


/**
 * Swap the parameters held by this function.
 *
 * 
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void RateMultiplierPhyloFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) 
{
    
    if ( oldP == tau ) 
    {
        tau = static_cast< const TypedDagNode< TimeTree > * >( newP );
    }
    
    if ( oldP == rateMultipliers ) 
    {
        rateMultipliers = static_cast< const TypedDagNode< RbVector<double> >* >( newP );
    }
    
    if ( oldP == baseRate ) 
    {
        baseRate = static_cast< const TypedDagNode< double > * >( newP );
    }
}


/**
 * Update the current value of the function by recomputing it given the current parameter values.
 * This will happen when execute is called for the function.
 */
void RateMultiplierPhyloFunction::update( void )    
{
    
    // get the root
    const TopologyNode& root = tau->getValue().getRoot();
    recursiveUpdate(root, baseRate->getValue());
    
}

