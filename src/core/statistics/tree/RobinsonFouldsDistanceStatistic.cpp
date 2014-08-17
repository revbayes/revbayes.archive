#include "RobinsonFouldsDistanceStatistic.h"
#include "TreeBipartitions.h"
#include <algorithm>

#include <boost/dynamic_bitset.hpp>

using namespace RevBayesCore;

//
//  RobinsonFouldsDistanceStatistic.cpp
//  RevBayesCore
//
//  Created by Bastien Boussau on 16/08/14.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

RobinsonFouldsDistanceStatistic::RobinsonFouldsDistanceStatistic(const TypedDagNode<TimeTree> *t1, const TypedDagNode<TimeTree> *t2) : TypedFunction< double >( new double(0.0) ),
tree1( t1 ), tree2(t2)
{
    // add the tree parameter as a parent
    addParameter( tree1 );
    addParameter( tree2 );
    
    update();
}


RobinsonFouldsDistanceStatistic::~RobinsonFouldsDistanceStatistic( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



RobinsonFouldsDistanceStatistic* RobinsonFouldsDistanceStatistic::clone( void ) const
{
    
    return new RobinsonFouldsDistanceStatistic( *this );
}


void RobinsonFouldsDistanceStatistic::update( void )
{
    
    //const TopologyNode& r = tree->getValue().getRoot();
    std::vector<boost::dynamic_bitset<> > bipartitions1 = (TreeBipartitions(tree1)).getValue();
    std::vector<boost::dynamic_bitset<> > bipartitions2 = (TreeBipartitions(tree2)).getValue();
    bool found;
    *value = 0.0;
    for (size_t i = 0; i< bipartitions1.size(); ++i) {
        found = false;
        for (size_t j = 0; j < bipartitions2.size(); ++j) {
            if (bipartitions1[i] == bipartitions2[j]) {
                found = true;
                break;
            }
        }
        if (!found) {
            *value += 1.0;
        }
    }
    for (size_t i = 0; i< bipartitions2.size(); ++i) {
        found = false;
        for (size_t j = 0; j < bipartitions1.size(); ++j) {
            if (bipartitions2[i] == bipartitions1[j]) {
                found = true;
                break;
            }
        }
        if (!found) {
            *value += 1.0;
        }
    }

}



void RobinsonFouldsDistanceStatistic::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == tree1)
    {
        tree1 = static_cast<const TypedDagNode<TimeTree>* >( newP );
    }
    if (oldP == tree2)
    {
        tree2 = static_cast<const TypedDagNode<TimeTree>* >( newP );
    }
    
}
