/**
 * @file
 * This file contains the declaration of the number of tables statistic for DPP-distributed variables, 
 * This simply computes the number of distinct value categories
 *
 * @brief Declaration of the DppNumTablesStatistic
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id: DppNumTablesStatistic.h tracyh $
 */

#ifndef DppNumTablesStatistic_H
#define DppNumTablesStatistic_H

#include "DirichletProcessPriorDistribution.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
	template <class valueType>
    class DppNumTablesStatistic : public TypedFunction<int> {
        
    public:
																DppNumTablesStatistic(const StochasticNode< std::vector<valueType> >* v);	//!< Default constructor
																DppNumTablesStatistic(const DppNumTablesStatistic& t);						//!< Copy constructor
        virtual													~DppNumTablesStatistic(void);												//!< Destructor
        
        DppNumTablesStatistic&									operator=(const DppNumTablesStatistic& t);
        
        // Basic utility functions
        DppNumTablesStatistic*									clone(void) const;															//!< Clone object
        void													update(void);																//!< Clone the function
		
    protected:
        void													swapParameterInternal(const DagNode *oldP, const DagNode *newP);			//!< Implementation of swaping parameters
        
    private:
		
		// members
        const StochasticNode< std::vector<valueType> >*			elementVals;
    };
	
}

using namespace RevBayesCore;

template<class valueType>
DppNumTablesStatistic<valueType>::DppNumTablesStatistic(const StochasticNode< std::vector<valueType> >* v) : TypedFunction<int>( new int(0) ), elementVals( v ) {
    // add the tree parameter as a parent
    addParameter( elementVals );
    update();
}

template<class valueType>
DppNumTablesStatistic<valueType>::DppNumTablesStatistic(const DppNumTablesStatistic &t) : TypedFunction<int>( t ), elementVals( t.elementVals ) {
    // no need to add parameters, happens automatically
}

template<class valueType>
DppNumTablesStatistic<valueType>::~DppNumTablesStatistic( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}

template<class valueType>
DppNumTablesStatistic<valueType>* DppNumTablesStatistic<valueType>::clone( void ) const {
    return new DppNumTablesStatistic<valueType>( *this );
}

template<class valueType>
void DppNumTablesStatistic<valueType>::update( void ) {
    
    const DirichletProcessPriorDistribution<valueType>& dist = static_cast<const DirichletProcessPriorDistribution<valueType> &>( elementVals->getDistribution() );
	*value = dist.getNumberOfCategories();
}

template<class valueType>
void DppNumTablesStatistic<valueType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {

    if (oldP == elementVals) {
        elementVals = static_cast< const StochasticNode< std::vector<valueType> >* >( newP );
    }
}



#endif

