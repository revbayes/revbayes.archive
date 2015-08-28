//
//  RateAgeACLNMixingMove.cpp



#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RateAgeACLNMixingMove.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

RateAgeACLNMixingMove::RateAgeACLNMixingMove( std::vector<DagNode*> n, double ep, bool t, double w) : CompoundMove( n, w, t), epsilon( ep ) {
    
    tree = static_cast<StochasticNode<TimeTree>* > (n[0]);
    rates = static_cast<StochasticNode< RbVector<double> >* > (n[1]);
	rootRate = static_cast<StochasticNode<double >* >( n[2] ) ;
    
}



/* Clone object */
RateAgeACLNMixingMove* RateAgeACLNMixingMove::clone( void ) const {
    
    return new RateAgeACLNMixingMove( *this );
}



const std::string& RateAgeACLNMixingMove::getMoveName( void ) const {
    static std::string name = "RateAgeACLNMixingMove";
    
    return name;
}


/** Perform the move */
double RateAgeACLNMixingMove::performCompoundMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    TimeTree& tau = tree->getValue();
	std::vector<double>& nrates = rates->getValue();
	double &rootR = rootRate->getValue();
	
	
	size_t nn = tau.getNumberOfNodes();
	double u = rng->uniform01();
	double c = exp( epsilon * (u - 0.5) );
	
	for(size_t i=0; i<nn; i++){
		TopologyNode* node = &tau.getNode(i);
		if(node->isTip() == false){
			double curAge = node->getAge();
			double newAge = curAge * c;
			tau.setAge( node->getIndex(), newAge );
			if(node->isRoot()){
				storedRootAge = curAge;
			}
		}
	}
	
	size_t nr = nrates.size();
	rootR = rootR / c;
	for(size_t i=0; i<nrates.size(); i++){
        double curRt = nrates[i];
        double newRt = curRt / c;
        nrates[i] = newRt;
	}
	
	storedC = c;
	double pr = (((double)nn) - (double)nr) * log(c);
	return pr;
}


void RateAgeACLNMixingMove::printParameterSummary(std::ostream &o) const {
    o << "epsilon = " << epsilon;
}


void RateAgeACLNMixingMove::rejectCompoundMove( void ) {
    	
    // undo the proposal
	TimeTree& tau = tree->getValue();
	std::vector<double>& nrates = rates->getValue();
	double &rootR = rootRate->getValue();
	
	size_t nn = tau.getNumberOfNodes();
	double c = storedC;
	
	for(size_t i=0; i<nn; i++){
		TopologyNode* node = &tau.getNode(i);
		if(node->isTip() == false){
			double curAge = node->getAge();
			double undoAge = curAge / c;
			tau.setAge( node->getIndex(), undoAge );
		}
	}
	
	size_t nr = nrates.size();
	rootR = rootR * c;
	for(size_t i=0; i<nr; i++){
        double curRt = nrates[i];
        double undoRt = curRt * c;
        nrates[i] = undoRt;
	}
	
#ifdef ASSERTIONS_TREE
    if ( fabs(storedRootAge - tau.getRoot().getAge()) > 1E-8 ) {
        throw RbException("Error while rejecting RateAgeACLNMixingMove proposal: Node ages were not correctly restored!");
    }
#endif
}


void RateAgeACLNMixingMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    CompoundMove::swapNode(oldN, newN);
    
	if(oldN == tree)
		tree = static_cast<StochasticNode<TimeTree>* >(newN) ;
    if(oldN == rates)
		rates = static_cast<StochasticNode< RbVector<double> >* >( newN ) ;
    if(oldN == rootRate)
		rootRate = static_cast<StochasticNode<double >* >( newN ) ;
}


void RateAgeACLNMixingMove::tune( void )
{
//    double rate = numAccepted / double(numTried);
    
    // do nothing right now
}


