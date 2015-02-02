/**
 * @file
 * This file contains the implementation of a time-calibrated tree. This version of a tree
 * contains nodes with times.
 *
 * @brief Implementation of a time calibrated tree
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-17, version 1.0
 *
 * $Id: TimeTree.cpp 1651 2012-07-05 14:47:08Z hoehna $
 */

#include "TimeTree.h"
#include "RbException.h"
#include "RbOptions.h"
#include "TreeChangeEventListener.h"
#include "Topology.h"
#include "TopologyNode.h"

using namespace RevBayesCore;

/* Default constructor */
TimeTree::TimeTree(void) : Tree() {
    
}


/* Copy constructor */
TimeTree::TimeTree(const TimeTree& t) : Tree( t ) {
    
//    times = t.times;
    ages  = t.ages;
    
}


/* Destructor */
TimeTree::~TimeTree(void) {
    
}


TimeTree& TimeTree::operator=(const TimeTree &t) {
    
    if (this != &t) {
        Tree::operator=(t);
        
        ages      = t.ages;
//        times     = t.times;
        
    }
    
    return *this;
}


/* Clone function */
TimeTree* TimeTree::clone(void) const
{
    
    return new TimeTree(*this);
}


double TimeTree::getAge(size_t idx) const
{
    return ages[idx];
}



double TimeTree::getBranchLength(size_t idx) const
{
    
    const TopologyNode &n = topology->getNode( idx );
    if ( n.isRoot() )
    {
        return 0.0;
    }
    else
    {
        size_t parentIdx = n.getParent().getIndex();
        return ages[parentIdx] - ages[idx];
    }
}



double TimeTree::getTime(size_t idx) const
{
    
    const TopologyNode &n = topology->getNode( idx );
    if ( n.isRoot() )
    {
        return 0.0;
    }
    else
    {
        size_t parentIdx = n.getParent().getIndex();
        return ages[parentIdx] - ages[idx] + getTime(parentIdx);
    }
}


double TimeTree::getTreeLength(void) const {
    double treeLength = 0;
    const std::vector<TopologyNode*> &nodes = topology->getNodes();
    for (size_t i = 0; i < nodes.size(); i++)
        treeLength += nodes[i]->getBranchLength();
    return treeLength;
}


void TimeTree::resizeElementVectors(size_t n) {
    // remove all elements
    ages.clear();
//    times.clear();
    
    // resize to new dimension
    ages.resize( n );
//    times.resize( n );
}


void TimeTree::setAge(size_t idx, double a)
{
    
    // fire a tree change event
    const std::set<TreeChangeEventListener*> &listeners = changeEventHandler.getListeners();
    const TopologyNode &n = topology->getNode(idx);
    for (std::set<TreeChangeEventListener*>::iterator it = listeners.begin(); it != listeners.end(); ++it)
    {

        for (size_t i = 0; i < n.getNumberOfChildren(); ++i)
        {
            (*it)->fireTreeChangeEvent(n.getChild(i));
        }
    }
    
    // flag the newick string as invalid
    const std::vector<TopologyNode*> &children = n.getChildren();
    for (std::vector<TopologyNode*>::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        (*it)->flagNewickRecomputation();
    }
    
    ages[idx] = a;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const TimeTree& x) {
    o << x.getNewickRepresentation();
    
    return o;
}
