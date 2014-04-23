/**
 * @file
 * This file contains the implmentation of the average value statistic for DPP-distributed continuous variables, 
 *
 * @brief Implmentation of the DppNumTablesStatistic
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id: DppMeanContinuousValStatistic.cpp tracyh $
 */
 
#include "DppMeanContinuousValStatistic.h"
#include "DirichletProcessPriorDistribution.h"
#include "StochasticNode.h"

#include <vector>
#include <string>

using namespace RevBayesCore;

DppMeanContinuousValStatistic::DppMeanContinuousValStatistic(const StochasticNode< std::vector<double> >* v) : TypedFunction<double>( new double(0.0) ), elementVals( v ) {
    // add the tree parameter as a parent
    addParameter( elementVals );
    update();
}

DppMeanContinuousValStatistic::DppMeanContinuousValStatistic(const DppMeanContinuousValStatistic &t) : TypedFunction<double>( t ), elementVals( t.elementVals ) {
    // no need to add parameters, happens automatically
}

DppMeanContinuousValStatistic::~DppMeanContinuousValStatistic( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}

DppMeanContinuousValStatistic* DppMeanContinuousValStatistic::clone( void ) const {
    return new DppMeanContinuousValStatistic( *this );
}

void DppMeanContinuousValStatistic::update( void ) {
    
	double sum = 0.0;
	const std::vector<double>& evs = elementVals->getValue();
	size_t numElem = evs.size();
	for(int i=0; i<evs.size(); i++){
		sum += evs[i];
	}
	*value = sum / ((double)numElem);
}

void DppMeanContinuousValStatistic::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
	
    if (oldP == elementVals) {
        elementVals = static_cast< const StochasticNode< std::vector<double> >* >( newP );
    }
}

