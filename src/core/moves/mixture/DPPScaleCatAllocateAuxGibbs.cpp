/**
 * @file
 * This file contains the implementation of a move on the DPP based on Neal (2000) Algorithm 8
 * this move changes the value assigned to each table
 * works on doubles
 *
 * @brief Implementation of DPPScaleCatAllocateAuxGibbs
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

#include "DPPScaleCatAllocateAuxGibbs.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "DirichletProcessPriorDistribution.h"

#include <cmath>

RevBayesCore::DPPScaleCatAllocateAuxGibbs::DPPScaleCatAllocateAuxGibbs(StochasticNode< RbVector<double> >* v, double l, int na, double w) : MoveOld( v, w, false ), variable( v ) {
    
	// set isGibbs to true
	lambda = l;
    numAuxCat = na;
}


/** Clone object */
RevBayesCore::DPPScaleCatAllocateAuxGibbs* RevBayesCore::DPPScaleCatAllocateAuxGibbs::clone( void ) const {
    
    return new DPPScaleCatAllocateAuxGibbs( *this );
}



const std::string& RevBayesCore::DPPScaleCatAllocateAuxGibbs::getMoveName( void ) const {
    static std::string name = "DPP Scale Cat Values Move (double)";
    
    return name;
}

bool RevBayesCore::DPPScaleCatAllocateAuxGibbs::isGibbs( void ) const {
    
    return true;
}


/** Perform the move */
void RevBayesCore::DPPScaleCatAllocateAuxGibbs::performGibbsMove( void ) {
    
    doScaleTablesMove();
    doAllocateAuxMove();
   
}

void RevBayesCore::DPPScaleCatAllocateAuxGibbs::doScaleTablesMove( void ) {
    
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

void RevBayesCore::DPPScaleCatAllocateAuxGibbs::doAllocateAuxMove( void ) {
    RandomNumberGenerator* rng     = GLOBAL_RNG;
	
	DirichletProcessPriorDistribution<double>& dist = static_cast<DirichletProcessPriorDistribution<double> &>( variable->getDistribution() );
	
	dist.createRestaurantVectors();
	int numTables = dist.getNumberOfCategories();
	int numElements = dist.getNumberOfElements();
	std::vector<double> tableVals = dist.getTableParameters();
	std::vector<int> allocVec = dist.getElementAllocationVec();
	std::vector<int> numPerTab = dist.getNumElemPerTable();
	std::vector<double>& elementVals = variable->getValue();
	double cp = dist.getConcentrationParam();
	TypedDistribution<double>* g0 = dist.getBaseDistribution();
    
	int numAuxiliary = numAuxCat; 
	double lnCPOverNumAux = log( cp/( (double)numAuxiliary ) );
	// loop over elements, remove i from current table, and try in all others
	for(int i=0; i<numElements; i++){
		std::vector<double> tempTables;
		std::vector<double> lnProb;
		int currentTable = allocVec[i];
		numPerTab[currentTable] -= 1;
		for(int j=0; j<numTables; j++){
			int numSeated = numPerTab[j];
			if(numSeated > 0){
				double newV = tableVals[j];
				tempTables.push_back(newV);
				elementVals[i] = newV;
				variable->touch();
				double tabLnL = getCurrentLnProbabilityForMove(); // get lnL after changing value
				lnProb.push_back( log(numSeated) + tabLnL );
			}
			else{
				tableVals[j] = 0.0;
				numPerTab[j] = 0;
			}
		}
		for(int j=0; j<numAuxiliary; j++){
			g0->redrawValue();
			double newV = g0->getValue();
			tempTables.push_back(newV);
			elementVals[i] = newV;
			variable->touch();
			double auxLnL = getCurrentLnProbabilityForMove(); // get lnL after changing value
			lnProb.push_back( lnCPOverNumAux + auxLnL );
		}
		
		// Normalize lnProb vector 		
		dppNormalizeVector(lnProb);
        
		//choose new table for element i
		double u = rng->uniform01();
		int reseat = findElementNewTable(u, lnProb);
        
		double afterValue = tempTables[reseat];
		elementVals[i] = afterValue;
		variable->touch();
		int tID = findTableIDForVal(tableVals, afterValue);
		if(tID == -1){
			numTables += 1;
			numPerTab.push_back(1);
			tableVals.push_back(afterValue);
		}
		else{
			// this is an existing table
			numPerTab[tID] += 1;
		}
		variable->keep();
		lnProb.clear();
		tempTables.clear();
	}
	allocVec.clear();
	tableVals.clear();
	numPerTab.clear();
	dist.createRestaurantVectors();
	variable->touch();
	variable->keep();

}


void RevBayesCore::DPPScaleCatAllocateAuxGibbs::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    MoveOld::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode< RbVector<double> >* >( newN );
}

double RevBayesCore::DPPScaleCatAllocateAuxGibbs::getCurrentLnProbabilityForMove(void) {
	
	std::set<DagNode*> affected;
	variable->getAffectedNodes( affected );
	double lnProb = 0.0;
	for (std::set<DagNode*>::iterator it = affected.begin(); it != affected.end(); ++it) {
		double lp = (*it)->getLnProbability();
		lnProb += lp;
	}
	return lnProb;
}

double RevBayesCore::DPPScaleCatAllocateAuxGibbs::safeExponentiation(double x) {
	
	if (x < -300.0)
		return 0.0;
	else if (x > 0.0)
		return 1.0;
	else
		return exp(x);
}

void RevBayesCore::DPPScaleCatAllocateAuxGibbs::dppNormalizeVector(std::vector<double> &v) {
    
	size_t n = v.size();
	double lnC = v[0];
	for (int i=1; i<n; i++){
		if (v[i] > lnC)
			lnC = v[i];
	}
	
	for (int i=0; i<n; i++)
		v[i] -= lnC;
	
	double sum = 0.0;
	for (int i=0; i<n; i++){
		if ( v[i] < -300.0 )
			v[i] = 0.0;
		else
			v[i] = exp( v[i] );
		sum += v[i];
	}
	
	for (int i=0; i<n; i++)
		v[i] /= sum;    
}

int RevBayesCore::DPPScaleCatAllocateAuxGibbs::findElementNewTable(double u, std::vector<double> lnProb) {
	for (int j = 0; j < lnProb.size(); j++){
		u -= lnProb[j];
		if (u < 0.0){
			return j;
		}
	}
	return -1;
}

int RevBayesCore::DPPScaleCatAllocateAuxGibbs::findTableIDForVal(std::vector<double> tvs, double val) {
	
	for (int j=0; j<tvs.size(); j++){
		if(tvs[j] == val){
			// this is an existing table
			return j;
		}
	}
	return -1; // new table
}


void RevBayesCore::DPPScaleCatAllocateAuxGibbs::acceptMove( void ) {
    
}

double RevBayesCore::DPPScaleCatAllocateAuxGibbs::performMove(double& probRatio) {
    return 0.0;
}

void RevBayesCore::DPPScaleCatAllocateAuxGibbs::rejectMove( void ) {
    
}



