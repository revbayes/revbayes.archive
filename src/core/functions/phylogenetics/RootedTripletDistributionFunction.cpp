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

RootedTripletDistributionFunction::RootedTripletDistributionFunction( const TypedDagNode< RbVector< TimeTree > > *ts,  const TypedDagNode< RbVector< std::string > > *sn ) : TypedFunction<RootedTripletDistribution>( new RootedTripletDistribution(  ) ) {
//    ts->getValue(), sn->getValue()
    ttrees = ts ;
    species = sn->clone();
    rtd = new RootedTripletDistribution( ttrees->getValue(), species->getValue()  );
    // add the lambda parameter as a parent
    addParameter( ts );
   // value->setSpecies ( species->getValue() );
    
   // update();
}


RootedTripletDistributionFunction::RootedTripletDistributionFunction( const TypedDagNode< RbVector<BranchLengthTree> > *ts,  const TypedDagNode< RbVector< std::string > > *sn ) : TypedFunction<RootedTripletDistribution>( new RootedTripletDistribution(  ) ) {
    //    ts->getValue(), sn->getValue()
    bltrees = ts ;
    species = sn->clone();
    rtd = new RootedTripletDistribution( bltrees->getValue(), species->getValue()  );
    // add the lambda parameter as a parent
    addParameter( ts );
    // value->setSpecies ( species->getValue() );
    
    // update();
}


RootedTripletDistributionFunction::RootedTripletDistributionFunction( const TypedDagNode< RbVector< std::string > > *sn ): TypedFunction<RootedTripletDistribution>( new RootedTripletDistribution(  ) ) {
    //    ts->getValue(), sn->getValue()
    species = sn->clone();
    rtd = new RootedTripletDistribution( species->getValue()  );

}

RootedTripletDistributionFunction::RootedTripletDistributionFunction( const TypedDagNode<std::vector< Taxon > > *t ): TypedFunction<RootedTripletDistribution>( new RootedTripletDistribution(  ) ) {
    //    ts->getValue(), sn->getValue()
    taxa = t->clone();
    rtd = new RootedTripletDistribution( taxa->getValue()  );
    
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
        rtd->resetDistribution();
    }
    if (ttrees)
        rtd->setTrees( ttrees->getValue() );
    else if (bltrees)
        rtd->setTrees( bltrees->getValue() );
    rtd->extractTriplets(   );
    value = rtd->clone();
}



void RootedTripletDistributionFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (ttrees)
        ttrees = static_cast<const TypedDagNode< RbVector<TimeTree> >* >( newP );
    else if (bltrees)
        bltrees = static_cast<const TypedDagNode< RbVector<BranchLengthTree> >* >( newP );

}


void RootedTripletDistributionFunction::setTrees(TypedDagNode< RbVector< TimeTree > >* ts)
{

    ttrees = ts;
    removeParameter( ts );
    addParameter( ts );

}


void RootedTripletDistributionFunction::setTrees(TypedDagNode< RbVector< BranchLengthTree > >* ts)
{
    
    bltrees = ts;
    removeParameter( ts );
    addParameter( ts );
    
}

