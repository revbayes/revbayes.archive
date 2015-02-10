//
//  TreeConverter.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 7/17/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "BranchLengthTree.h"
#include "TimeTree.h"
#include "Topology.h"
#include "TreeUtilities.h"

#include <algorithm>
#include <iostream>

void RevBayesCore::TreeUtilities::constructTimeTreeRecursively(TopologyNode *tn, const TopologyNode &n, std::vector<TopologyNode*> &nodes, std::vector<double> &ages, double depth) {
    
    // set the name
    tn->setName( n.getName() );
    
    // remember the node
    nodes.push_back( tn );
    
    // set the age
    double a = depth - n.getBranchLength();
    if ( a < 1E-6 ) 
    {
        a = 0.0;
    }
    ages.push_back( a );
    
    // create children
    for (size_t i = 0; i < n.getNumberOfChildren(); ++i) {
        const TopologyNode& child = n.getChild( i );
        TopologyNode* newChild = new TopologyNode();
        
        // set parent child relationship
        newChild->setParent( tn );
        tn->addChild( newChild );
        
        // start recursive call
        constructTimeTreeRecursively(newChild, child, nodes, ages, a);
    }
}


RevBayesCore::TimeTree* RevBayesCore::TreeUtilities::convertTree(const Tree &t) {
    // create time tree object (topology + times)
    TimeTree *tt = new TimeTree();
    
    // the topology object
    Topology *tau = new Topology();
    
    // clock trees should always be rooted
    tau->setRooted( true );
    
    // get the root of the original tree
    const TopologyNode& bln = t.getRoot();
    
    TopologyNode* root = new TopologyNode();
    
    std::vector<double> ages;
    std::vector<TopologyNode*> nodes;
    
    double maxDepth = bln.getMaxDepth() + bln.getBranchLength();
    
    // recursive creation of the tree
    constructTimeTreeRecursively(root, bln, nodes, ages, maxDepth);
    
    // add the root which creates the topology
    tau->setRoot( root );
    
    // connect the topology to the tree
    tt->setTopology( tau, true );
    
    // set the ages
    for (size_t i = 0; i < nodes.size(); ++i) 
    {
        tt->setAge(nodes[i]->getIndex(), ages[i]);
    }
    
    return tt;
}

void RevBayesCore::TreeUtilities::getOldestTip(TimeTree* t, TopologyNode *n, double& oldest)
{
    
    // we only rescale internal nodes
    if ( !n->isTip() ) {
        
        // assertion that we have binary trees
#ifdef ASSERTIONS_TREE
        if ( n->getNumberOfChildren() != 2 ) {
            throw RbException("NNI is only implemented for binary trees!");
        }
#endif
        
        // rescale both children
        getOldestTip( t, &n->getChild(0), oldest);
        getOldestTip( t, &n->getChild(1), oldest);
    }
    else
    {
        if (n->getAge() > oldest)
        {
            oldest = n->getAge();
        }
    }
}

void RevBayesCore::TreeUtilities::rescaleSubtree(TimeTree *t, TopologyNode *n, double factor) {
    // we only rescale internal nodes
    if ( !n->isTip() ) {
        // rescale the age of the node
        double newAge = n->getAge() * factor;
        t->setAge(n->getIndex(), newAge);
        
        // assertion that we have binary trees
#ifdef ASSERTIONS_TREE
        if ( n->getNumberOfChildren() != 2 ) {
            throw RbException("NNI is only implemented for binary trees!");
        }
#endif
        
        // rescale both children
        rescaleSubtree( t, &n->getChild(0), factor);
        rescaleSubtree( t, &n->getChild(1), factor);
    }
}


void RevBayesCore::TreeUtilities::rescaleTree(TimeTree *t, TopologyNode *n, double factor) {
    // rescale the time of the node
    double newAge = n->getAge() * factor;
    t->setAge( n->getIndex(), newAge);
    
    // recursive call for internal nodes
    if ( !n->isTip() ) {
        
        // assertion that we have binary trees
#ifdef ASSERTIONS_TREE
        if ( n->getNumberOfChildren() != 2 ) {
            throw RbException("NNI is only implemented for binary trees!");
        }
#endif
        
        // rescale both children
        rescaleTree( t, &n->getChild(0), factor);
        rescaleTree( t, &n->getChild(1), factor);
    }
}



std::string RevBayesCore::TreeUtilities::uniqueNewickTopology(const Tree &t) 
{
    return uniqueNewickTopologyRecursive( t.getRoot() );
}


std::string RevBayesCore::TreeUtilities::uniqueNewickTopologyRecursive(const TopologyNode &n) 
{
    // check whether this is an internal node
    if ( n.isTip() ) 
    {
        return n.getName();
    } 
    else 
    {
        std::string newick = "(";
        std::vector<std::string> children;
        for (size_t i = 0; i < n.getNumberOfChildren(); ++i) 
        {
            children.push_back( uniqueNewickTopologyRecursive(n.getChild( i ) ) );
        }
        sort(children.begin(), children.end());
        for (std::vector<std::string>::iterator it = children.begin(); it != children.end(); ++it) 
        {
            if ( it != children.begin() ) 
            {
                newick += ",";
            }
            newick += *it;
        }
        newick += ")";
        newick += n.getName();
        
        return newick;
    }
    
}
