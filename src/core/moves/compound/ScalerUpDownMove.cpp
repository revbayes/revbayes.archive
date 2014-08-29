//
//  ScalerUpDownMove.cpp



#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "ScalerUpDownMove.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

ScalerUpDownMove::ScalerUpDownMove( std::vector<DagNode*> n, double sf, bool t, double w) : CompoundMove( n, w, t), scaleFactor( sf ) {
    
    scalerVal1 = static_cast<StochasticNode< double >* >( n[0] );
	scalerVal2 = static_cast<StochasticNode< double >* >( n[1] ) ;
    
}



/* Clone object */
ScalerUpDownMove* ScalerUpDownMove::clone( void ) const {
    
    return new ScalerUpDownMove( *this );
}



const std::string& ScalerUpDownMove::getMoveName( void ) const {
    static std::string name = "ScalerUpDownMove";
    
    return name;
}


/** Perform the move */
double ScalerUpDownMove::performCompoundMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    double& sv1 = scalerVal1->getValue();
    double& sv2 = scalerVal2->getValue();
	
	
	double u = rng->uniform01();
	double c = exp( scaleFactor * (u - 0.5) );
	

	
	double sv1Current = sv1;
	double sv2Current = sv2;
	
	double newSV1 = sv1Current * c;
	storedSV1 = sv1Current;
	sv1 = newSV1;
	
	double newSV2 = sv2Current / c;
	storedSV2 = sv2Current;
	sv2 = newSV2;
	
	
	
	storedC = c;
	double pr = (1.0 - 1.0 - 2.0) * log(c);
	return pr;
}


void ScalerUpDownMove::printParameterSummary(std::ostream &o) const {
    o << "scaleFactor = " << scaleFactor;
}


void ScalerUpDownMove::rejectCompoundMove( void ) {
	
    // undo the proposal
	double& sv1 = scalerVal1->getValue();
	double& sv2 = scalerVal2->getValue();

	double c = storedC;
	
	sv1 = storedSV1 / c;
	sv2 = storedSV2 * c;
	
}


void ScalerUpDownMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    CompoundMove::swapNode(oldN, newN);
    
	if(oldN == scalerVal1)
		scalerVal1 = static_cast<StochasticNode<double >* >( newN ) ;
    if(oldN == scalerVal2)
		scalerVal2 = static_cast<StochasticNode<double >* >( newN ) ;
}


void ScalerUpDownMove::tune( void ){
    double rate = numAccepted / double(numTried);
	double target = 0.44;
    if ( rate > target ){
        scaleFactor *= (1.0 + ((rate-target)/(1.0 - target) ));
    }
    else{
        scaleFactor /= (2.0 - rate/target );
    }
}


