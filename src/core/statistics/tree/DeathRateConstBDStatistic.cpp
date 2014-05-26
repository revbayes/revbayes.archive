/**
 * @file
 * This file contains the implmentation of the statistic for calculating the death rate given the diversification and turnover 
 *
 * @brief Implmentation of the DeathRateConstBDStatistic
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id: DeathRateConstBDStatistic.cpp tracyh $
 */
 
#include "DeathRateConstBDStatistic.h"
#include "StochasticNode.h"

#include <vector>
#include <string>
#include <ostream>

using namespace RevBayesCore;

DeathRateConstBDStatistic::DeathRateConstBDStatistic(const TypedDagNode< double >* d, const TypedDagNode< double >* t) : TypedFunction<double>( new double(0.0) ), diversification( d ), turnover( t ) {
    // add the tree parameter as a parent
    addParameter( diversification );
    addParameter( turnover );
    update();
}

DeathRateConstBDStatistic::DeathRateConstBDStatistic(const DeathRateConstBDStatistic &t) : TypedFunction<double>( t ), diversification(t.diversification), turnover(t.turnover) {
    // no need to add parameters, happens automatically
}

DeathRateConstBDStatistic::~DeathRateConstBDStatistic( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}

DeathRateConstBDStatistic* DeathRateConstBDStatistic::clone( void ) const {
    return new DeathRateConstBDStatistic( *this );
}

void DeathRateConstBDStatistic::update( void ) {
    
	double d = diversification->getValue();
	double r = turnover->getValue();
	double dr = (r * d) / (1.0 - r);
	*value = dr;
}

void DeathRateConstBDStatistic::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
	
    if (oldP == diversification) {
        diversification = static_cast< const TypedDagNode<double>* >( newP );
    }
    else if (oldP == turnover) {
        turnover = static_cast< const TypedDagNode<double>* >( newP );
    }
}

