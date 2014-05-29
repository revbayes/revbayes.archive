//
//  RateOnBranchAve.cpp
//  RevBayesCore

#include "RateOnBranchAve.h"

using namespace RevBayesCore;

RateOnBranchAve::RateOnBranchAve(const StochasticNode< std::vector<double> > *rts, const TypedDagNode<TimeTree> *t, const size_t idx) : TypedFunction<double>( new double(0.0) ), 
nodeRates( rts ),
tree( t ),
nodeID( idx )
{
    // add the tree parameter as a parent
    addParameter( nodeRates );
    addParameter( tree );
    
    update();
}


RateOnBranchAve::RateOnBranchAve(const RateOnBranchAve &n) : TypedFunction<double>( n ), 
nodeRates( n.nodeRates ), 
tree( n.tree ), 
nodeID( n.nodeID ) 
{
    // no need to add parameters, happens automatically
}


RateOnBranchAve::~RateOnBranchAve( void ) 
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



RateOnBranchAve* RateOnBranchAve::clone( void ) const 
{
    
    return new RateOnBranchAve( *this );
}


void RateOnBranchAve::update( void ) 
{
	
    const TopologyNode& n = tree->getValue().getNode(nodeID);
	const std::vector<double>& brV = nodeRates->getValue();
	
	size_t parID = n.getParent().getIndex();
	double myRt = brV[nodeID];
	double parRt = brV[parID];
	
    *value = (myRt + parRt) / 2.0;
}



void RateOnBranchAve::swapParameterInternal(const DagNode *oldP, const DagNode *newP) 
{
    
    if (oldP == nodeRates) {
        nodeRates = static_cast< const StochasticNode< std::vector<double> >* >( newP );
    }
    else if (oldP == tree) {
        tree = static_cast<const TypedDagNode<TimeTree>* >( newP );
    }
    
}


