/**
 * @file
 * This file contains the implementation of a move on the DPP based on Neal (2000) Algorithm 8
 * this move reassigns the allocaton vector
 *
 * @brief Implementation of DPPAllocateAuxGibbsMove
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

#include "DirichletProcessPriorDistribution.h"
#include "DPPAllocateAuxGibbsMove.h"
#include "NormalizeVectorFunction.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathVector.h"

#include <cmath>

template <class valueType>
RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::DPPAllocateAuxGibbsMove(StochasticNode<std::vector<valueType> > *v, double w) : Move( v, w ), variable( v ) {
    
	// set isGibbs to true
}


/** Clone object */
template <class valueType>
RevBayesCore::DPPAllocateAuxGibbsMove<valueType>* RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::clone( void ) const {
    
    return new DPPAllocateAuxGibbsMove<valueType>( *this );
}



template <class valueType>
const std::string& RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::getMoveName( void ) const {
    static std::string name = "DPP Move";
    
    return name;
}



/** Perform the move */
template <class valueType>
void RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::performGibbsMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
	
	DirichletProcessPriorDistribution<valueType>& dist = static_cast<DirichletProcessPriorDistribution<valueType> &>( variable->getDistribution() );
	
	dist->createRestaurantVectors();
	int numTables = dist->getNumberOfCategories();
	int numElements = dist->getNumberOfElements();
	std::vector<valueType> tableVals = dist->getTableParameters();
	std::vector<int> allocVec = dist->getElementAllocationVec();
	std::vector<int> numPerTab = dist->getNumElemPerTable();
	std::vector<valueType>& elementVals = variable->getValue();
	double cp = dist->getConcentrationParam();
	TypedDistribution<valueType>* g0 = dist->getBaseDistribution();
    
	int numAuxiliary = 4; // this should probably be treated like the tuning parameter
	double lnCPOverNumAux = log(cp/numAuxiliary);
	// loop over elements, remove i from current table, and try in all others
	for(int i=0; i<numElements; i++){
		std::vector<valueType> tempTables;
		int currentTable = allocVec[i];
		numPerTab[currentTable] -= 1;
		std::vector<double> lnProb;
		for(int j=0; j<numTables; j++){
			int numSeated = numPerTab[j];
			if(numSeated > 0){
				valueType newV = tableVals[j];
				tempTables.push_back(newV);
				elementVals[i] = newV;
				double tabLnL = getLnProbabilityForMove(); // get lnL after changing value
				lnProb.push_back( log(numSeated) + tabLnL );
			}
			else{
				lnProb.push_back( RbConstants::Double::neginf ); // this should be a value that will be prob=0
				valueType dummy;
				tempTables.push_back(dummy); // just wanted a place-holder so that the vectors are the same size
			}
		}
		for(int j=0; j<numAuxiliary; j++){
			g0->redrawValue();
			valueType newV = g0->getValue();
			tempTables.push_back(newV);
			elementVals[i] = newV;
			double auxLnL = getLnProbabilityForMove(); // get lnL after changing value
			lnProb.push_back( lnCPOverNumAux + auxLnL );
		}


		// Normalize lnProb vector RbMathVector
		RbMath::normalize(lnProb, 1.0);
				
		//choose new table for element i
		double u = rng->uniform01();
		int reseat = -1;
		for (int j = 0; j < lnProb.size(); j++){
			u -= lnProb[j];
			if (u < 0.0){
				reseat = j;
				break;
			}
		}
		elementVals[i] = tempTables[reseat];
		if(reseat >= numTables){
			// this is a new table
			numTables += 1;
			numPerTab.push_back(1);
			tableVals.push_back(tempTables[reseat]);
		}
		else{
			// this is an existing table
			numPerTab[reseat] += 1;
		}
	}
	dist->createRestaurantVectors();
}



template <class valueType>
void RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    Move::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<valueType>* >( newN );
}


template <class valueType>
double RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::getLnProbabilityForMove(void) {

	std::set<DagNode*> affected;
	variable->getAffectedNodes( affected );
	double lnProbRatio = 0.0;
	for (std::set<DagNode*>::iterator it = affected.begin(); it != affected.end(); ++it) {
		lnProbRatio += (*it)->getLnProbabilityRatio();
	}
	return lnProbRatio;

}

template <class valueType>
void RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::acceptMove( void ) {
    
}

template <class valueType>
double RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::performMove(double& probRatio) {
   return 0.0;
}

template <class valueType>
void RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::rejectMove( void ) {
    
}

