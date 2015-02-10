//
//  FossilSafeSlideMove.cpp


#include "ContinuousStochasticNode.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "FossilSafeSlideMove.h"
#include "StochasticNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

FossilSafeSlideMove::FossilSafeSlideMove(std::vector<DagNode*> n, double l, bool t, double w) : CompoundMove( n, w, t),
    lambda(l)
{
    tree   = static_cast<StochasticNode<TimeTree>* >( n[0] );
	scaler = static_cast<ContinuousStochasticNode*>( n[1] ) ;
    
}



/* Clone object */
FossilSafeSlideMove* FossilSafeSlideMove::clone( void ) const {
    
    return new FossilSafeSlideMove( *this );
}



const std::string& FossilSafeSlideMove::getMoveName( void ) const {
    static std::string name = "FossilSafeSlideMove";
    
    return name;
}


/** Perform the move */
double FossilSafeSlideMove::performCompoundMove( void ) {
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    double val = scaler->getValue();
    storedValue = val;
    
    double min = scaler->getMin();
    double max = scaler->getMax();

    //    const std::vector<TopologyNode*>& nodes = t.getNodes();
//    for (size_t i = 0; i < t.getNumberOfTips(); i++)
//    {
//        if (nodes[i]->getAge() > 1e-3 && nodes[i]->getBranchLength() < min) {
//            min = nodes[i]->getBranchLength();
//        }
//    }

    double size = max - min;
    
    double u      = rng->uniform01();
    double delta  = ( lambda * ( u - 0.5 ) );
    
    if ( fabs(delta) > 2.0*size )
    {
        delta -= floor(delta / (2.0*size)) * (2.0*size);
    }
    double newVal = val + delta;
    
    /* reflect the new value */
    do {
        if ( newVal < min )
        {
            newVal = 2.0 * min - newVal;
        }
        else if ( newVal > max )
        {
            newVal = 2.0 * max - newVal;
        }
    } while ( newVal < min || newVal > max );

    val = newVal;
    scaler->setValue(val);
    scaler->touch();
    TimeTree& t = tree->getValue();
    
    const std::vector<TopologyNode*>& nodes = t.getNodes();
    for (size_t i = 0; i < t.getNumberOfTips(); i++)
    {
        const double& len = nodes[i]->getBranchLength();
        if (len <= 0.0) {
            // +Inf forces rejection in CompoundMove
            return RbConstants::Double::inf;
        }
    }
    
	return 0.0;
}


void FossilSafeSlideMove::printParameterSummary(std::ostream &o) const {
    o << "weight = " << weight;
}


void FossilSafeSlideMove::rejectCompoundMove( void ) {
	
    // undo the proposal
//	double& v = scaler->getValue();

    scaler->getValue();
    scaler->setValue(storedValue);
    
    tree->touch();
    tree->getValue();
    
//	v = storedValue;
	
}


void FossilSafeSlideMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    CompoundMove::swapNode(oldN, newN);
    
	if(oldN == scaler)
		scaler = static_cast<ContinuousStochasticNode* >( newN ) ;
    if(oldN == tree)
		tree = static_cast<StochasticNode<TimeTree>* >( newN ) ;
}

void FossilSafeSlideMove::tune(void) {
    
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 )
    {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else
    {
        lambda /= (2.0 - rate/0.44 );
    }
    
    lambda = fmin(10000, lambda);

}

