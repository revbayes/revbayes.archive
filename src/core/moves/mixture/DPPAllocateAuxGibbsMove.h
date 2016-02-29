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
#include "AbstractGibbsMove.h"
#include "RbVector.h"

namespace RevBayesCore {

    template <class valueType>
    class DPPAllocateAuxGibbsMove : public AbstractGibbsMove {
    
    public:
        DPPAllocateAuxGibbsMove(StochasticNode< RbVector<valueType> >* v, int na, double w);                                                                      //!< Internal constructor
    
        // Basic utility functions
        DPPAllocateAuxGibbsMove<valueType>*						clone(void) const;                                                                  //!< Clone object
        const std::string&                                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
		
    protected:
        void													performGibbsMove(void);                                                            //!< Perform move
        void                                                    swapNodeInternal(DagNode *oldN, DagNode *newN);
        
    private:
		double													getLnProbabilityForMove(void);
		int														findTableIDForVal(std::vector<valueType> tvs, valueType val);
		void													dppNormalizeVector(std::vector<double> &v);
		int														findElementNewTable(double u, std::vector<double> lnProb);
		
        StochasticNode< RbVector<valueType> >*                  variable;
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
RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::DPPAllocateAuxGibbsMove(StochasticNode< RbVector<valueType> > *v, int na, double w) : AbstractGibbsMove( w ),
    variable( v )
{
    addNode( variable );
    
	numAuxCat = na;
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
template <class valueType>
RevBayesCore::DPPAllocateAuxGibbsMove<valueType>* RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::clone( void ) const
{
    
    return new DPPAllocateAuxGibbsMove<valueType>( *this );
}

template <class valueType>
const std::string& RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::getMoveName( void ) const
{
    static std::string name = "DPPAllocateAuxGibbsMove";
    
    return name;
}


/** Perform the move */
template <class valueType>
void RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::performGibbsMove( void )
{
    
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
	double lnCPOverNumAux = log( cp/( (double)numAuxiliary ) );
	// loop over elements, remove i from current table, and try in all others
	for(int i=0; i<numElements; i++)
    {
        
		std::vector<valueType> tempTables;
		std::vector<double> lnProb;
		int currentTable = allocVec[i];
		numPerTab[currentTable] -= 1;
		for(int j=0; j<numTables; j++)
        {
			int numSeated = numPerTab[j];
			if(numSeated > 0)
            {
				valueType newV = tableVals[j];
				tempTables.push_back(newV);
				elementVals[i] = newV;
				variable->touch();
				double tabLnL = getLnProbabilityForMove(); // get lnL after changing value
				lnProb.push_back( log(numSeated) + tabLnL );
			}
			else
            {
				tableVals[j] = valueType();
				numPerTab[j] = 0;
			}
		}
        
		for(int j=0; j<numAuxiliary; j++)
        {
			g0->redrawValue();
			valueType newV = g0->getValue();
			tempTables.push_back(newV);
			elementVals[i] = newV;
			variable->touch();
			double auxLnL = getLnProbabilityForMove(); // get lnL after changing value
			lnProb.push_back( lnCPOverNumAux + auxLnL );
		}
		
		// Normalize lnProb vector 		
		dppNormalizeVector(lnProb);
				
		//choose new table for element i
		double u = rng->uniform01();
		int reseat = findElementNewTable(u, lnProb);

		valueType afterValue = tempTables[reseat];
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

template <class valueType>
void RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::swapNodeInternal(DagNode *oldN, DagNode *newN)
{

    variable = static_cast<StochasticNode< RbVector<valueType> > * >( newN );

}


template <class valueType>
double RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::getLnProbabilityForMove(void)
{
	
	RbOrderedSet<DagNode*> affected;
	variable->getAffectedNodes( affected );
	double lnProb = 0.0;
	for (RbOrderedSet<DagNode*>::iterator it = affected.begin(); it != affected.end(); ++it)
    {
		double lp = (*it)->getLnProbability();
		lnProb += lp;
	}
	return lnProb;
}

template <class valueType>
int RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::findTableIDForVal(std::vector<valueType> tvs, valueType val) {
	
	for (size_t j=0; j<tvs.size(); j++)
    {
		if(tvs[j] == val)
        {
			// this is an existing table
			return int(j);
		}
	}
    
	return -1; // new table
}


template <class valueType>
void RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::dppNormalizeVector(std::vector<double> &v)
{

	size_t n = v.size();
	double lnC = v[0];
	for (size_t i=1; i<n; i++)
    {
		if (v[i] > lnC)
        {
			lnC = v[i];
        }
	}
	
	for (size_t i=0; i<n; i++)
    {
		v[i] -= lnC;
    }
    
	double sum = 0.0;
	for (size_t i=0; i<n; i++)
    {
		if ( v[i] < -300.0 )
        {
			v[i] = 0.0;
        }
        else
        {
			v[i] = exp( v[i] );
        }
        sum += v[i];
	
    }
	
	for (size_t i=0; i<n; i++)
    {
		v[i] /= sum;
    }
    
}

template <class valueType>
int RevBayesCore::DPPAllocateAuxGibbsMove<valueType>::findElementNewTable(double u, std::vector<double> lnProb)
{

    for (size_t j = 0; j < lnProb.size(); j++)
    {
		u -= lnProb[j];
        if (u < 0.0){
            return (int)j;
		}
	}
    
	return -1;
}



#endif

