//
//  RateOnBranchAve.cpp
//  RevBayesCore

#include "RateOnBranchAve.h"
#include "ConstantNode.h"

using namespace RevBayesCore;

RateOnBranchAve::RateOnBranchAve(const TypedDagNode< RbVector<double> > *rts, const TypedDagNode<TimeTree> *t, const TypedDagNode< double > *rr, const size_t idx) : TypedFunction<double>( new double(0.0) ),
nodeRates( rts ),
tree( t ),
rootRate( rr ),
scaleValue( new ConstantNode<double>(" ", new double(1.0) )),
nodeID( idx )
{
    // add the tree parameter as a parent
    addParameter( nodeRates );
    addParameter( tree );
    addParameter( rootRate );
    addParameter( scaleValue );
    
    update();
}

RateOnBranchAve::RateOnBranchAve(const TypedDagNode< RbVector<double> > *rts, const TypedDagNode<TimeTree> *t, const TypedDagNode< double >* rr, const TypedDagNode< double >* sv, const size_t idx) : TypedFunction<double>( new double(0.0) ),
nodeRates( rts ),
tree( t ),
rootRate( rr ),
scaleValue( sv ),
nodeID( idx )
{
    // add the tree parameter as a parent
    addParameter( nodeRates );
    addParameter( tree );
    addParameter( rootRate );
    addParameter( scaleValue );
    
    update();
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
	double rr = rootRate->getValue();
	double sv = scaleValue->getValue();
	size_t rI = tree->getValue().getRoot().getIndex();
	
	size_t parID = n.getParent().getIndex();
	double myRt = brV[nodeID];
	double parRt;
	if(parID == rI){
		parRt = rr;
	}
	else{
		parRt = brV[parID];
	}
    *value = ((myRt + parRt) * 0.5) * sv;
}



void RateOnBranchAve::swapParameterInternal(const DagNode *oldP, const DagNode *newP) 
{
    
    if (oldP == nodeRates) {
        nodeRates = static_cast< const TypedDagNode< RbVector<double> >* >( newP );
    }
    if (oldP == tree) {
        tree = static_cast<const TypedDagNode<TimeTree>* >( newP );
    }
    if ( oldP == rootRate ) {
        rootRate = static_cast< const TypedDagNode< double > * >( newP );
    }
    if ( oldP == scaleValue ) {
        scaleValue = static_cast< const TypedDagNode< double > * >( newP );
    }
    
}


