#include "RootedTripletDistributionFunction.h"
#include "RbException.h"
#include "BranchLengthTree.h"
#include "RootedTripletDistribution.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "Tree.h"
#include "TreeUtilities.h"

#include <boost/dynamic_bitset.hpp>

using namespace RevBayesCore;

RootedTripletDistributionFunction::RootedTripletDistributionFunction( const TypedDagNode<std::vector<TimeTree> > *ts,  const TypedDagNode<std::vector< std::string > > *sn ) : TypedFunction<RootedTripletDistribution>( new RootedTripletDistribution( ts->getValue(), sn->getValue() ) ), trees( ts ), species( sn ) {
    // add the lambda parameter as a parent
    addParameter( ts );
   // value->setSpecies ( species->getValue() );
    
   // update();
}


RootedTripletDistributionFunction::~RootedTripletDistributionFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



RootedTripletDistributionFunction* RootedTripletDistributionFunction::clone( void ) const
{
    return new RootedTripletDistributionFunction( *this );
}


void RootedTripletDistributionFunction::update( void )
{
    if (rtd != NULL  ) {
        delete rtd;
    }
    rtd->resetDistribution();
    rtd->extractTriplets(  trees->getValue() );
    value = rtd->clone();
}



void RootedTripletDistributionFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    trees = static_cast<const TypedDagNode<std::vector<TimeTree> >* >( newP );
    
}



