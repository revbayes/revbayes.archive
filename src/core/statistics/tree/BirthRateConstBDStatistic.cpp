/**
 * @file
 * This file contains the implmentation of the statistic for calculating the birth rate given div and turnover,
 *
 * @brief Implmentation of the BirthRateConstBDStatistic
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id: BirthRateConstBDStatistic.cpp tracyh $
 */
 
#include "BirthRateConstBDStatistic.h"
#include "StochasticNode.h"

#include <vector>
#include <string>
#include <ostream>

using namespace RevBayesCore;

BirthRateConstBDStatistic::BirthRateConstBDStatistic(const TypedDagNode< double >* d, const TypedDagNode< double >* t) : TypedFunction<double>( new double(0.0) ), diversification( d ), turnover( t ) {
    // add the tree parameter as a parent
    addParameter( diversification );
    addParameter( turnover );
    update();
}

BirthRateConstBDStatistic::BirthRateConstBDStatistic(const BirthRateConstBDStatistic &t) : TypedFunction<double>( t ), diversification(t.diversification), turnover(t.turnover) {
    // no need to add parameters, happens automatically
}

BirthRateConstBDStatistic::~BirthRateConstBDStatistic( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}

BirthRateConstBDStatistic* BirthRateConstBDStatistic::clone( void ) const {
    return new BirthRateConstBDStatistic( *this );
}

void BirthRateConstBDStatistic::update( void ) {
    
	double d = diversification->getValue();
	double r = turnover->getValue();
	double br = d / (1.0 - r);
	*value = br;
}

void BirthRateConstBDStatistic::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
	
    if (oldP == diversification) {
        diversification = static_cast< const TypedDagNode<double>* >( newP );
    }
    else if (oldP == turnover) {
        turnover = static_cast< const TypedDagNode<double>* >( newP );
    }
}

