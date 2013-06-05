/**
 * @file
 * This file contains the declaration of the DPP class. 
 * A mixture object holds the mapping between parameter values and the indix of this parameters.
 *
 *
 * @brief Declaration of the Mixture class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-18
 *
 * $Id$
 */

#ifndef DirichletProcessPriorDistribution_H
#define DirichletProcessPriorDistribution_H

#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template <class valueType>
    class DirichletProcessPriorDistribution : public TypedDistribution< std::vector<valueType> > {
        
    public:
        // constructor(s)
        DirichletProcessPriorDistribution(const TypedDagNode< TypedDistribution<valueType> > *g, const TypedDagNode< double > *cp, const TypedDagNode< int > *n);
        DirichletProcessPriorDistribution(const DirichletProcessPriorDistribution<valueType> &n);
        
        // public member functions
        DirichletProcessPriorDistribution*                  clone(void) const;                                                                      //!< Create an independent clone
        double                                              computeLnProbability(void);
        size_t                                              getNumberOfCategories(void) const;
        void                                                redrawValue(void);
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                                //!< Implementation of swaping parameters
		
        
    private:
        // helper methods
        void                                                computeDenominator();
		std::vector<valueType>*                             simulate();
		        
        // private members
        const TypedDagNode< TypedDistribution<valueType> >*		baseDistribution;
		const TypedDagNode< double > *						concentration;
		const TypedDagNode< int > *							numElements;
        size_t                                              numTables;
		
        double                                              denominator;
        bool                                                concentrationHasChanged;
		std::vector<size_t>									numCustomerPerTable;
		std::vector<valueType>								valuePerTable;
    };
    
}

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbMathCombinatorialFunctions.h"
#include "VectorIndexOperator.h"

#include <cmath>

template <class valueType>
RevBayesCore::DirichletProcessPriorDistribution<valueType>::DirichletProcessPriorDistribution(const TypedDagNode< TypedDistribution<valueType> > *g, const TypedDagNode< double > *cp, 
																							  const TypedDagNode< int > *n) : 
																							  TypedDistribution<valueType>( new valueType() ), baseDistribution( g ), concentration( cp ), 
																							  numElements( n ), numTables( 0 ), concentrationHasChanged( true ), denominator( 0.0 ) {
    
    this->addParameter( baseDistribution );
    this->addParameter( concentration );
	this->addParameter( numElements );
    
	delete this->value;

    this->value = simulate();
                                                                                                  
}

template <class valueType>
RevBayesCore::DirichletProcessPriorDistribution<valueType>::DirichletProcessPriorDistribution(const DirichletProcessPriorDistribution<valueType> &n) : TypedDistribution<valueType>( n ), 
																								baseDistribution( n.baseDistribution ), concentration( n.concentration ), 
																								numElements( n.numElements ), numTables( n.numTables ), concentrationHasChanged( true ), denominator( 0.0 ) {
    // parameters are added automatically
    delete this->value;
    this->value = simulate();
                                                                                                    
}



template <class valueType>
RevBayesCore::DirichletProcessPriorDistribution<valueType>* RevBayesCore::DirichletProcessPriorDistribution<valueType>::clone( void ) const {
	
    return new DirichletProcessPriorDistribution<valueType>( *this );
}


template <class valueType>
void RevBayesCore::DirichletProcessPriorDistribution<valueType>::computeDenominator( void ) {
    // the denominator is sum^N_{i=1} ln( alpha + i - 1 )
    
    denominator = 0;
    int N = numElements->getValue();
    double cp = concentration->getValue() - 1.0;
    for (int i = 1; i <= N; ++i)
    {
        denominator += log( cp + i );
    }
    
//    concentrationHasChanged = false;
}




template <class valueType>
double RevBayesCore::DirichletProcessPriorDistribution<valueType>::computeLnProbability( void ) {
    
    // the probability is given by:
    // lnP = K * ln( alpha ) + sum^K_{i=1}( ln( factorial( n_i - 1 ) ) ) - sum^N_{i=1} ln( alpha + i - 1 )
    
    // reset the lnProb and set it to log( alpha^K )
    this->lnProb = log( concentration->getValue() ) * numTables;
    
    if ( concentrationHasChanged == true )
    {
        computeDenominator();
    }
    
    TypedDistribution<valueType>& d = baseDistribution->getValue();
    // compute the probability of having n_k customers per at table k
    for (size_t i = 0; i < numTables; ++i) 
    {
        this->lnProb += RbMath::lnFactorial( numCustomerPerTable[i] - 1 );
        
        // we also need to multiply with the probability of the value for this table
        d.setValue( valuePerTable[i] );
        this->lnProb += d.computeLnProbability();
    }
    
    this->lnProb -= denominator;
	
	
    return this->lnProb;
}



template <class valueType>
size_t RevBayesCore::DirichletProcessPriorDistribution<valueType>::getNumberOfCategories( void ) const {
    
    return numTables;
}


template <class valueType>
std::vector<valueType>* RevBayesCore::DirichletProcessPriorDistribution<valueType>::simulate() {
    
    // simulator function
	std::vector<valueType>* rv = new std::vector<valueType>();
	std::vector<valueType>& temp = *rv;
	double cp = concentration->getValue();
	
	RandomNumberGenerator* rng = GLOBAL_RNG;
	
	for( int i=0; i<numElements->getValue(); i++)
    {
		double probNewCat = cp / (i + cp);
		double u = rng->uniform01();
		if( u < probNewCat)
        {
			numCustomerPerTable.push_back(1);
			baseDistribution->redrawValue();
			valueType newVal = baseDistribution->getValue(); //simulate function
			valuePerTable.push_back(newVal);
			temp.push_back(newVal);
		}
		else
        {
			double sum = 0.0;
			double m = rng->uniform01();
			for(int j=0; j<numCustomerPerTable.size(); j++)
            {
				sum += (double)numCustomerPerTable[j] / i;
				if(m < sum)
                {
					numCustomerPerTable[j] += 1;
					temp.push_back(valuePerTable[j]);
					break;
				}
			}
		}		
	}
    
	return rv ;
}


template <class valueType>
void RevBayesCore::DirichletProcessPriorDistribution<valueType>::redrawValue( void ) {

    delete this->value;
    this->value = simulate();
    
}



template <class valueType>
void RevBayesCore::DirichletProcessPriorDistribution<valueType>::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == baseDistribution) 
    {
        baseDistribution = static_cast<const TypedDagNode< TypedDistribution<valueType> >* >( newP );
    }
    if (oldP == concentration) 
    {
        concentration = static_cast<const TypedDagNode< double > *>( newP );
    }
    if (oldP == numElements) 
    {
        numElements = static_cast<const TypedDagNode< int > * >( newP );
    }
    
}




#endif

