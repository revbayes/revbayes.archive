/**
 * @file
 * This file contains the implementation of a move on the DPP based on Neal (2000) Algorithm 8
 * this move changes the value assigned to each table
 * works on doubles
 *
 * @brief Implementation of DPPScaleCatValsMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-11 14:54:35 +0200 (Fri, 11 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-08-14, version 1.0
 *
 * $Id$
 */

#include "DPPScaleCatValsMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "DirichletProcessPriorDistribution.h"

#include <cmath>

RevBayesCore::DPPScaleCatValsMove::DPPScaleCatValsMove(StochasticNode<RbVector<double> > *v, double l, double w) : MoveOld( v, w, false ),
    variable( v )
{
    
	// set isGibbs to true
	lambda = l;
}


/** Clone object */
RevBayesCore::DPPScaleCatValsMove* RevBayesCore::DPPScaleCatValsMove::clone( void ) const {
    
    return new DPPScaleCatValsMove( *this );
}



const std::string& RevBayesCore::DPPScaleCatValsMove::getMoveName( void ) const {
    static std::string name = "DPP Scale Cat Values Move (double)";
    
    return name;
}

bool RevBayesCore::DPPScaleCatValsMove::isGibbs( void ) const {
    
    return true;
}


/** Perform the move */
void RevBayesCore::DPPScaleCatValsMove::performGibbsMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
	
	DirichletProcessPriorDistribution<double>& dist = static_cast<DirichletProcessPriorDistribution<double> &>( variable->getDistribution() );
	
	dist.createRestaurantVectors();
	int numTables = dist.getNumberOfCategories();
	int numElements = dist.getNumberOfElements();
	std::vector<double> tableVals = dist.getTableParameters();
	std::vector<int> allocVec = dist.getElementAllocationVec();
	std::vector<double>& elementVals = variable->getValue();
	TypedDistribution<double>* g0 = dist.getBaseDistribution();
	
	double storedValue;
	variable->touch();
	for(int i=0; i<numTables; i++){
		
		// get old lnL
		double oldLnl = getCurrentLnProbabilityForMove();
		
		storedValue = tableVals[i];
		double u = rng->uniform01();
		double scalingFactor = std::exp( lambda * ( u - 0.5 ) );
		double newValue = storedValue * scalingFactor;
		tableVals[i] = newValue;
		
		// Assign new value to elements
		for(int j=0; j<numElements; j++){
			if(allocVec[j] == i)
				elementVals[j] = tableVals[i];
		}
		
		g0->getValue() = tableVals[i]; // new
		double priorRatio = g0->computeLnProbability(); 
		g0->getValue() = storedValue; // old
		priorRatio -= g0->computeLnProbability();
		
		variable->touch();
		double newLnl = getCurrentLnProbabilityForMove();
		double lnProbRatio = newLnl - oldLnl;
				
		double r = safeExponentiation(priorRatio + lnProbRatio + log(scalingFactor));
		u = rng->uniform01();
		if ( u < r ) //accept
			variable->keep();
		else{ // reject
			for(int j=0; j<numElements; j++){
				if(allocVec[j] == i)
					elementVals[j] = storedValue;
			}
			tableVals[i] = storedValue;
			variable->touch();
			variable->keep();
		}
	}
    dist.createRestaurantVectors();
}



void RevBayesCore::DPPScaleCatValsMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    MoveOld::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode< RbVector<double> >* >( newN );
}

double RevBayesCore::DPPScaleCatValsMove::getCurrentLnProbabilityForMove(void) {
	
	std::set<DagNode*> affected;
	variable->getAffectedNodes( affected );
	double lnProb = 0.0;
	for (std::set<DagNode*>::iterator it = affected.begin(); it != affected.end(); ++it) {
		double lp = (*it)->getLnProbability();
		lnProb += lp;
	}
	return lnProb;
}


double RevBayesCore::DPPScaleCatValsMove::safeExponentiation(double x) {
	
	if (x < -300.0)
		return 0.0;
	else if (x > 0.0)
		return 1.0;
	else
		return exp(x);
}

void RevBayesCore::DPPScaleCatValsMove::acceptMove( void ) {
    
}

double RevBayesCore::DPPScaleCatValsMove::performMove(double& probRatio) {
   return 0.0;
}

void RevBayesCore::DPPScaleCatValsMove::rejectMove( void ) {
    
}



