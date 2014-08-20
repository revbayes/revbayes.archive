//
//  MrcaIndexStatistic.cpp
//  RevBayesCore
//
//  Created by Michael Landis on 08/19/14.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "MrcaIndexStatistic.h"
#include "RbConstants.h"
#include "RbException.h"

using namespace RevBayesCore;

MrcaIndexStatistic::MrcaIndexStatistic(const TypedDagNode<TimeTree> *t, const Clade &c) : TypedFunction<int>( new int(-1) ), tree( t ), clade( c ), index( RbConstants::Size_t::nan ) {
    // add the tree parameter as a parent
    addParameter( tree );
    
    initialize();
    update();
}


MrcaIndexStatistic::~MrcaIndexStatistic( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
}



MrcaIndexStatistic* MrcaIndexStatistic::clone( void ) const {
    
    return new MrcaIndexStatistic( *this );
}


void MrcaIndexStatistic::initialize( void ) {
    
    taxaCount = clade.size();
    index = RbConstants::Size_t::nan;
    
}


void MrcaIndexStatistic::update( void ) {
    
    const std::vector<TopologyNode*> &n = tree->getValue().getNodes();
    size_t minCaldeSize = n.size() + 2;
    
    bool found = false;
    if ( index != RbConstants::Size_t::nan )
    {
        
        TopologyNode *node = n[index];
        std::vector<std::string> taxa;
        node->getTaxaStringVector( taxa );
        size_t cladeSize = taxa.size();
        if ( node->containsClade( clade, false ) && taxaCount == cladeSize )
        {
            found = true;
        }
        else
        {
            minCaldeSize = cladeSize;
        }
        
    }
    
    
    if ( found == false )
    {
        
        for (size_t i = tree->getValue().getNumberOfTips(); i < n.size(); ++i)
        {
            
            TopologyNode *node = n[i];
            std::vector<std::string> taxa;
            node->getTaxaStringVector( taxa );
            size_t cladeSize = taxa.size();
            if ( cladeSize < minCaldeSize && cladeSize >= taxaCount && node->containsClade( clade, false ) )
            {
                
                index = node->getIndex();
                minCaldeSize = cladeSize;
                if ( taxaCount == cladeSize )
                {
                    break;
                }
                
            }
            
        }
        
    }
    
    if ( index == RbConstants::Size_t::nan )
    {
        throw RbException("MrcaIndex-Statistics can only be applied if clade is present.");
    }
    
    int mrcaIndexIdx = index;
    *value = mrcaIndexIdx;
}



void MrcaIndexStatistic::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == tree)
    {
        tree = static_cast<const TypedDagNode<TimeTree>* >( newP );
        index = RbConstants::Size_t::nan;
    }
    
}


