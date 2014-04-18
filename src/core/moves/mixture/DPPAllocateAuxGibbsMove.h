/**
 * @file
 * This file contains the declaration of DPPAllocateAuxGibbsMove, which performs the DPP move based on Neal (2000) Algorithm 8
 *
 * @brief Declaration of DPPAllocateAuxGibbsMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-11 14:54:35 +0200 (Fri, 11 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-20, version 1.0
 *
 * $Id: DPPAllocateAuxGibbsMove.h $
 */

#ifndef DPPAllocateAuxGibbsMove_H
#define DPPAllocateAuxGibbsMove_H

#include <ostream>

#include "DirichletProcessPriorDistribution.h"
#include "SimpleMove.h"

namespace RevBayesCore {

    template <class valueType>
    class DPPAllocateAuxGibbsMove : public Move {
    
    public:
        DPPAllocateAuxGibbsMove(StochasticNode< std::vector<valueType> >* v, int na, double w);                                                                      //!< Internal constructor
    
        // Basic utility functions
        DPPAllocateAuxGibbsMove<valueType>*						clone(void) const;                                                                  //!< Clone object
        void                                                    swapNode(DagNode *oldN, DagNode *newN);
		const std::string&                                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
		bool													isGibbs(void) const;

    protected:
        void													performGibbsMove(void);                                                            //!< Perform move
        void													acceptMove(void);                                                                   //!< Accept the InferenceMoveSimple
        double													performMove(double& probRatio);                                                     //!< Perform the InferenceMoveSimple
        void													rejectMove(void);                                                                   //!< Reject the InferenceMoveSimple
    
    private:
		double													getLnProbabilityForMove(void);
		int														findTableIDForVal(std::vector<valueType> tvs, valueType val);
		
        StochasticNode<std::vector<valueType> >*				variable;
		int														numAuxCat;
		
 
    };
    
}

#include "NormalizeVectorFunction.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathVector.h"

#include <cmath>

template <class valueType>
RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::DPPAllocateAuxGibbsMove(StochasticNode< std::vector<valueType> > *v, int na, double w) : Move( v, w, false ), variable( v ) {
    
	// set isGibbs to true
	numAuxCat = na;
}


/** Clone object */
template <class valueType>
RevBayesCore::DPPAllocateAuxGibbsMove<valueType>* RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::clone( void ) const {
    
    return new DPPAllocateAuxGibbsMove<valueType>( *this );
}



template <class valueType>
const std::string& RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::getMoveName( void ) const {
    static std::string name = "DPPAllocateAuxGibbsMove";
    
    return name;
}

template <class valueType>
bool RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::isGibbs( void ) const {
    
    return true;
}



/** Perform the move */
template <class valueType>
void RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::performGibbsMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
	
	DirichletProcessPriorDistribution<valueType>& dist = static_cast<DirichletProcessPriorDistribution<valueType> &>( variable->getDistribution() );
	
	dist.createRestaurantVectors();
	int numTables = dist.getNumberOfCategories();
	int numElements = dist.getNumberOfElements();
	std::vector<valueType> tableVals = dist.getTableParameters();
	std::vector<int> allocVec = dist.getElementAllocationVec();
	std::vector<int> numPerTab = dist.getNumElemPerTable();
	std::vector<valueType>& elementVals = variable->getValue();
	double cp = dist.getConcentrationParam();
	TypedDistribution<valueType>* g0 = dist.getBaseDistribution();
    
	int numAuxiliary = numAuxCat; 
	double lnCPOverNumAux = log(cp/numAuxiliary);
//	std::cout << cp << std::endl;
	// loop over elements, remove i from current table, and try in all others
	for(int i=0; i<numElements; i++){
		std::vector<valueType> tempTables;
		std::vector<double> lnProb;
		int currentTable = allocVec[i];
		int emptyTab = -1;
		numPerTab[currentTable] -= 1;
		for(int j=0; j<numTables; j++){
			int numSeated = numPerTab[j];
			if(numSeated > 0){
				valueType newV = tableVals[j];
				tempTables.push_back(newV);
				elementVals[i] = newV;
				variable->touch();
				double tabLnL = getLnProbabilityForMove(); // get lnL after changing value
				lnProb.push_back( log(numSeated) + tabLnL );
			}
			else{
				emptyTab = j;
				tableVals[j] = NULL;
				numPerTab[j] = 0;
			}
		}
		for(int j=0; j<numAuxiliary; j++){
			g0->redrawValue();
			valueType newV = g0->getValue();
			tempTables.push_back(newV);
			elementVals[i] = newV;
			variable->touch();
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
		if(reseat >= tempTables.size() - numAuxiliary){
			// this is a new table
			numTables += 1;
			numPerTab.push_back(1);
			tableVals.push_back(tempTables[reseat]);
		}
		else{
			// this is an existing table, need to find the right one and increment the num elements
			int tID = findTableIDForVal(tableVals, elementVals[i]);
			numPerTab[tID] += 1;
		}
		variable->keep();
		lnProb.clear();
		tempTables.clear();
//		std::cout << elementVals[i] << std::endl;
	}
	dist.createRestaurantVectors();
//	std::cout << dist.getNumberOfCategories() << std::endl;
//	for(int j=0; j<elementVals.size(); j++){
//		std::cout << elementVals[j] << " -- ";
//	}
//	std::cout << std::endl;
}

template <class valueType>
void RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    Move::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode< std::vector<valueType> > * >( newN );
}


template <class valueType>
double RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::getLnProbabilityForMove(void) {
	
	std::set<DagNode*> affected;
	variable->getAffectedNodes( affected );
	double lnProb = 0.0;
	for (std::set<DagNode*>::iterator it = affected.begin(); it != affected.end(); ++it) {
		lnProb += (*it)->getLnProbability();
	}
	return lnProb;
	
}

template <class valueType>
int RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::findTableIDForVal(std::vector<valueType> tvs, valueType val) {
	
	for (int j=0; j<tvs.size(); j++){
		if(tvs[j] == val){
			return j;
		}
	}
	return -1;
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


#endif

