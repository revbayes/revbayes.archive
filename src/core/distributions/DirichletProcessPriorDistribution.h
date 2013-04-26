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
		std::vector<valueType>*                             simulate();
		        
        // private members
        const TypedDagNode< TypedDistribution<valueType> >*		baseDistribution;
		const TypedDagNode< double > *						concentration;
		const TypedDagNode< int > *							numElements;
        size_t                                              numTables;
		
		std::vector<size_t>									numCustomerPerTable;
		std::vector<valueType>								valuePerTable;
    };
    
}

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "VectorIndexOperator.h"

#include <cmath>

template <class valueType>
RevBayesCore::DirichletProcessPriorDistribution<valueType>::DirichletProcessPriorDistribution(const TypedDagNode< TypedDistribution<valueType> > *g, const TypedDagNode< double > *cp, 
																							  const TypedDagNode< int > *n) : 
																							  TypedDistribution<valueType>( new valueType() ), baseDistribution( g ), concentration( cp ), 
																							  numElements( n ), numTables( 0 ) {
    
    this->addParameter( baseDistribution );
    this->addParameter( concentration );
	this->addParameter( numElements );
    
	delete this->value;

    this->value = simulate();
}

template <class valueType>
RevBayesCore::DirichletProcessPriorDistribution<valueType>::DirichletProcessPriorDistribution(const DirichletProcessPriorDistribution<valueType> &n) : TypedDistribution<valueType>( n ), 
																								baseDistribution( n.baseDistribution ), concentration( n.concentration ), 
																								numElements( n.numElements ), numTables( n.numTables ) {
    // parameters are added automatically
    delete this->value;
    this->value = simulate();
}



template <class valueType>
RevBayesCore::DirichletProcessPriorDistribution<valueType>* RevBayesCore::DirichletProcessPriorDistribution<valueType>::clone( void ) const {
	return new DirichletProcessPriorDistribution<valueType>( *this );
}



template <class valueType>
double RevBayesCore::DirichletProcessPriorDistribution<valueType>::computeLnProbability( void ) {
    
	// Todo compute prob
//	double v1 = 1.0 / (1.0 + concentration->getValue());
	
	
    return 0.0;
}



template <class valueType>
size_t RevBayesCore::DirichletProcessPriorDistribution<valueType>::getNumberOfCategories( void ) const {
    return numTables;
}


//template <class valueType>
//const std::vector<valueType>& RevBayesCore::DirichletProcessPriorDistribution<valueType>::getParameterValues( void ) const {
//    return parameterValues->getValue();
//}


template <class valueType>
std::vector<valueType>* RevBayesCore::DirichletProcessPriorDistribution<valueType>::simulate() {
    
    // simulator function
	std::vector<valueType>* rv = new std::vector<valueType>();
	std::vector<valueType>& temp = *rv;
	double cp = concentration->getValue();
	
	RandomNumberGenerator* rng = GLOBAL_RNG;
	
	for( int i=0; i<numElements->getValue(); i++){
		double probNewCat = cp / (i + cp);
		double u = rng->uniform01();
		if( u < probNewCat){
			numCustomerPerTable.push_back(1);
			baseDistribution->redrawValue();
			valueType newVal = baseDistribution->getValue(); //simulate function
			valuePerTable.push_back(newVal);
			temp.push_back(newVal);
		}
		else{
			double sum = 0.0;
			double m = rng->uniform01();
			for(int j=0; j<numCustomerPerTable.size(); j++){
				sum += (double)numCustomerPerTable[j] / i;
				if(m < sum){
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
    if (oldP == baseDistribution) {
        baseDistribution = static_cast<const TypedDagNode< TypedDistribution<valueType> >* >( newP );
    }
    if (oldP == concentration) {
        concentration = static_cast<const TypedDagNode< double > *>( newP );
    }
    if (oldP == numElements) {
        numElements = static_cast<const TypedDagNode< int > * >( newP );
    }
}




#endif

