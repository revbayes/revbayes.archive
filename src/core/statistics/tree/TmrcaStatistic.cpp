//
//  TmrcaStatistic.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 11/29/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TmrcaStatistic.h"
#include "RbException.h"

using namespace RevBayesCore;

TmrcaStatistic::TmrcaStatistic(const TypedDagNode<TimeTree> *t, const Clade &c) : TypedFunction<double>( new double(0.0) ), tree( t ), clade( c ), index( -1 ) {
    // add the tree parameter as a parent
    addParameter( tree );
    
    update();
}


TmrcaStatistic::TmrcaStatistic(const TmrcaStatistic &n) : TypedFunction<double>( n ), tree( n.tree ), clade( n.clade ), index( -1 ) {
    // no need to add parameters, happens automatically

}


TmrcaStatistic::~TmrcaStatistic( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!

}



TmrcaStatistic* TmrcaStatistic::clone( void ) const {
    
    return new TmrcaStatistic( *this );
}


void TmrcaStatistic::update( void ) {
    
    
    const std::vector<TopologyNode*> &n = tree->getValue().getNodes();
    size_t minCaldeSize = n.size() + 2;
    for (size_t i = tree->getValue().getNumberOfTips(); i < n.size(); ++i) 
    {
        TopologyNode *node = n[i];
        size_t cladeSize = node->getTaxaStringVector().size();
        if ( cladeSize < minCaldeSize && node->containsClade( clade, false ) ) 
        {
            index = int( node->getIndex() );
            minCaldeSize = cladeSize;
//            break;
        }
    }
    if ( index < 0 ) 
    {
        throw RbException("TMRCA-Statistics can only be applied if clade is present.");
    }
    
    double tmrca = tree->getValue().getAge(index);
    *value = tmrca;
}



void TmrcaStatistic::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == tree) 
    {
        tree = static_cast<const TypedDagNode<TimeTree>* >( newP );
        index = -1;
    }
    
}


