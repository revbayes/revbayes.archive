//
//  RateOnBranchAve.cpp
//  RevBayesCore

#include "RateOnBranchAve.h"
#include "ConstantNode.h"

using namespace RevBayesCore;

RateOnBranchAve::RateOnBranchAve(const StochasticNode< std::vector<double> > *rts, const TypedDagNode<TimeTree> *t, const size_t idx) : TypedFunction<double>( new double(0.0) ), 
nodeRates( rts ),
tree( t ),
scaleValue( new ConstantNode<double>(" ", new double(1.0) )),
nodeID( idx )
{
    // add the tree parameter as a parent
    addParameter( nodeRates );
    addParameter( tree );
    addParameter( scaleValue );
    
    update();
}

RateOnBranchAve::RateOnBranchAve(const StochasticNode< std::vector<double> > *rts, const TypedDagNode<TimeTree> *t, const TypedDagNode< double >* sv, const size_t idx) : TypedFunction<double>( new double(0.0) ), 
nodeRates( rts ),
tree( t ),
scaleValue( sv ),
nodeID( idx )
{
    // add the tree parameter as a parent
    addParameter( nodeRates );
    addParameter( tree );
    addParameter( scaleValue );
    
    update();
}


RateOnBranchAve::RateOnBranchAve(const RateOnBranchAve &n) : TypedFunction<double>( n ), 
nodeRates( n.nodeRates ), 
tree( n.tree ), 
scaleValue( n.scaleValue ),
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
	double sv = scaleValue->getValue();
	
	size_t parID = n.getParent().getIndex();
	double myRt = brV[nodeID];
	double parRt = brV[parID];
	
    *value = ((myRt + parRt) / 2.0) * sv;
}



void RateOnBranchAve::swapParameterInternal(const DagNode *oldP, const DagNode *newP) 
{
    
    if (oldP == nodeRates) {
        nodeRates = static_cast< const StochasticNode< std::vector<double> >* >( newP );
    }
    if (oldP == tree) {
        tree = static_cast<const TypedDagNode<TimeTree>* >( newP );
    }
    if ( oldP == scaleValue ) {
        scaleValue = static_cast< const TypedDagNode< double > * >( newP );
    }
    
}


