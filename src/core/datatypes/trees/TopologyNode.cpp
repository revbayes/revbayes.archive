/**
 * @file
 * This file contains the declaration of a TopologyNode.
 *
 * @brief Declaration of TopologyNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-17 10:31:20 +0200 (Tue, 17 Jul 2012) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id: TopologyNode.cpp 1682 2012-07-17 08:31:20Z hoehna $
 */

#include "Clade.h"
#include "RbException.h"
#include "RbOptions.h"
#include "RbSettings.h"
#include "RbUtil.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "Tree.h"

#include <algorithm>
#include <stdio.h>

using namespace RevBayesCore;

/** Default constructor (interior node, no name). Give the node an optional index ID */
TopologyNode::TopologyNode(int indx) :
    parent( NULL ),
    tree( NULL ),
    name(""), 
    index(indx),
    interiorNode( false ),
    rootNode( true ),
    tipNode( true ),
    newick(""), 
    newickNeedsRefreshing( true )
{
    
}


/** Constructor of node with name. Give the node an optional index ID */
TopologyNode::TopologyNode(const std::string& n, int indx) :
    parent( NULL ),
    tree( NULL ),
    name(n), 
    index(indx),
    interiorNode( false ),
    rootNode( true ),
    tipNode( true ),
    newick(""), 
    newickNeedsRefreshing( true )
{
    
}


/** Copy constructor. We use a shallow copy. */
TopologyNode::TopologyNode(const TopologyNode &n) : 
    tree( NULL ) 
{
    
    // copy the members
    name                    = n.name;
    index                   = n.index;
    interiorNode            = n.interiorNode;
    tipNode                 = n.tipNode;
    rootNode                = n.rootNode;
    newick                  = n.newick;
    newickNeedsRefreshing   = n.newickNeedsRefreshing;
    parent                  = n.parent;
    nodeComments            = n.nodeComments;
    branchComments          = n.branchComments;
    
    // copy the children
    for (std::vector<TopologyNode*>::const_iterator it = n.children.begin(); it != n.children.end(); it++) 
    {
        TopologyNode* theNode = *it;
        TopologyNode* theClone = theNode->clone();
        children.push_back( theClone );
        theClone->setParent(this);
    }

    
}


/** Destructor */
TopologyNode::~TopologyNode(void) 
{
    
    // we do not own the parent so we do not delete it
    
    // free memory of children
    removeAllChildren();
    
    // make sure that I was removed from my parent
    if (parent != NULL)
        parent->removeChild(this);
    
}


TopologyNode& TopologyNode::operator=(const TopologyNode &n) {
    
    if (this == &n) 
    {
        
        removeAllChildren();
        
        // copy the members
        name                    = n.name;
        index                   = n.index;
        interiorNode            = n.interiorNode;
        tipNode                 = n.tipNode;
        rootNode                = n.rootNode;
        newick                  = n.newick;
        newickNeedsRefreshing   = n.newickNeedsRefreshing;
        nodeComments            = n.nodeComments;
        branchComments          = n.branchComments;
        
        // copy the members
        parent          = n.parent;
        
        // copy the children
        for (std::vector<TopologyNode*>::const_iterator it = n.children.begin(); it != n.children.end(); it++) 
        {
            children.push_back( (*it)->clone() );
        }
        
        // add myself as a new child to the parent node
        parent->addChild(this);

    }
    
    return *this;
}



void TopologyNode::addBranchParameter(std::string const &n, const std::vector<double> &p, bool internalOnly) {
    
    if ( !internalOnly || !isTip()  ) 
    {
        std::stringstream o;
        o << n << "=" << p[index];
        std::string comment = o.str();
        branchComments.push_back( comment );
        
        newickNeedsRefreshing = true;
        
        for (std::vector<TopologyNode*>::iterator it = children.begin(); it != children.end(); ++it) 
        {
            (*it)->addBranchParameter(n, p, internalOnly);
        }
    }
}


/** Add a child node. We own it from here on. */
void TopologyNode::addChild(TopologyNode* c, bool forceNewickRecomp) {
    
    // add the child to our internal vector
    children.push_back(c);
    
    // mark for newick recomputation
    if ( forceNewickRecomp )
        flagNewickRecomputation();
    
    // fire tree change event
    if ( tree != NULL ) 
    {
        tree->getTreeChangeEventHandler().fire( *c );
    }
    
    tipNode = false;
    interiorNode = true;
}


void TopologyNode::addNodeParameter(std::string const &n, double p) 
{
    
    std::stringstream o;
    o << n << "=" << p;
    std::string comment = o.str();
    nodeComments.push_back( comment );
        
    newickNeedsRefreshing = true;
        
}


void TopologyNode::addParameter(std::string const &n, const std::vector<double> &p, bool internalOnly) {

    if ( !internalOnly || !isTip()  ) {
        std::stringstream o;
        o << n << "=" << p[index];
        std::string comment = o.str();
        nodeComments.push_back( comment );
        
        newickNeedsRefreshing = true;
        
        for (std::vector<TopologyNode*>::iterator it = children.begin(); it != children.end(); ++it) {
            (*it)->addParameter(n, p, internalOnly);
        }
    }
}


/* Build newick string */
std::string TopologyNode::buildNewickString( void ) {
    
    // create the newick string
    std::stringstream o;
    
    // test whether this is a internal or external node
    if (tipNode) 
    {
        // this is a tip so we just return the name of the node
        o << name;
        if ( nodeComments.size() > 0 || RbSettings::userSettings().getPrintNodeIndex() == true ) 
        {
            o << "[&";
            // first let us print the node index
            if ( RbSettings::userSettings().getPrintNodeIndex() == true )
            {
                o << "index=" << index;
                if (nodeComments.size() > 0) 
                {
                    o << ",";
                }
            }
            
            for (size_t i = 0; i < nodeComments.size(); ++i) 
            {
                if ( i > 0 ) 
                {
                    o << ",";
                }
                o << nodeComments[i];
            }
            o << "]";
        }
        
        if ( tree != NULL ) 
        {
            o << ":" << getBranchLength();
        }
        if ( branchComments.size() > 0 ) 
        {
            o << "[&";
            for (size_t i = 0; i < branchComments.size(); ++i) 
            {
                if ( i > 0 ) 
                {
                    o << ",";
                }
                o << branchComments[i];
            }
            o << "]";
        }
    }
    else {
        o << "(";
        for (size_t i=0; i<(getNumberOfChildren()-1); i++) 
        {
            o << getChild(i).computeNewick() << ",";
        }
        o << getChild(getNumberOfChildren()-1).computeNewick() << ")";
        if ( nodeComments.size() > 0 || RbSettings::userSettings().getPrintNodeIndex() == true ) 
        {
            o << "[&";
            // first let us print the node index
            if ( RbSettings::userSettings().getPrintNodeIndex() == true )
            {
                o << "index=" << index;
                if (nodeComments.size() > 0) 
                {
                    o << ",";
                }
            }
            for (size_t i = 0; i < nodeComments.size(); ++i) 
            {
                if ( i > 0 ) 
                {
                    o << ",";
                }
                o << nodeComments[i];
            }
            o << "]";
        }
        if ( tree != NULL ) 
        {
            o << ":" << getBranchLength();
        }
        if ( branchComments.size() > 0 ) 
        {
            o << "[&";
            for (size_t i = 0; i < branchComments.size(); ++i) 
            {
                if ( i > 0 ) 
                {
                    o << ",";
                }
                o << branchComments[i];
            }
            o << "]";
        }
    }
    
    return o.str();
}


void TopologyNode::clearBranchParameters( void ) {
    
    branchComments.clear();
    if ( !isTip()  ) 
    {
        
        for (std::vector<TopologyNode*>::iterator it = children.begin(); it != children.end(); ++it) 
        {
            (*it)->clearBranchParameters();
        }
    }
}


/** Clone function */
TopologyNode* TopologyNode::clone(void) const {
    
    return new TopologyNode(*this);
}



const std::string& TopologyNode::computeNewick(void) {
    
    // check if we need to recompute
    if ( newickNeedsRefreshing ) 
    {
        newick = buildNewickString();
        newickNeedsRefreshing = false;
    }
    
    return newick;
}


/* Build newick string */
std::string TopologyNode::computePlainNewick( void ) const {
    
    // test whether this is a internal or external node
    if (tipNode) 
    {
        // this is a tip so we just return the name of the node
        return name;
    }
    else {
        std::string left = getChild(0).computePlainNewick();
        std::string right = getChild(1).computePlainNewick();
        if ( left < right ) 
        {
            return "(" + left + "," + right + ")";
        } 
        else 
        {
            return "(" + right + "," + left + ")";
        }
    }
    
}


bool TopologyNode::containsClade(const TopologyNode *c, bool strict) const {
    
    std::vector<std::string> myTaxa   = getTaxaStringVector();
    std::vector<std::string> yourTaxa = c->getTaxaStringVector();
    
    if ( myTaxa.size() < yourTaxa.size() ) 
    {
        return false;
    }
    
    // check that every taxon of the clade is present in this subtree
    for (std::vector<std::string>::const_iterator y_it = yourTaxa.begin(); y_it != yourTaxa.end(); ++y_it) 
    {
        bool found = false;
        for (std::vector<std::string>::const_iterator it = myTaxa.begin(); it != myTaxa.end(); ++it) 
        {
            if ( *y_it == *it ) 
            {
                found = true;
                break;
            }
        }
        
        if (!found) 
        {
            return false;
        }
    }
    
    if ( !strict || myTaxa.size() == yourTaxa.size() ) 
    {
        return true;
    } 
    else 
    {
        bool contains = false;
        for (std::vector<TopologyNode*>::const_iterator it = children.begin(); it != children.end(); ++it) 
        {
            contains |= (*it)->containsClade(c,strict);
            if ( contains ) 
            {
                break;
            }
        }
        return contains;
    }
}


bool TopologyNode::containsClade(const Clade &c, bool strict) const 
{
    
    std::vector<std::string> myTaxa   = getTaxaStringVector();
    
    if ( myTaxa.size() < c.size() ) 
    {
        return false;
    }
    
    // check that every taxon of the clade is in this subtree
    for (std::vector<std::string>::const_iterator y_it = c.begin(); y_it != c.end(); ++y_it) 
    {
        bool found = false;
        for (std::vector<std::string>::const_iterator it = myTaxa.begin(); it != myTaxa.end(); ++it) 
        {
            if ( *y_it == *it ) 
            {
                found = true;
                break;
            }
        }
        
        if (!found) 
        {
            return false;
        }
    }
    
    if ( !strict || myTaxa.size() == c.size() ) 
    {
        return true;
    } 
    else 
    {
        bool contains = false;
        for (std::vector<TopologyNode*>::const_iterator it = children.begin(); it != children.end(); ++it) 
        {
            contains |= (*it)->containsClade(c,strict);
            if ( contains ) 
            {
                break;
            }
        }
        return contains;
    }
}



bool TopologyNode::equals(const TopologyNode& node) const {
    
    if (this == &node) 
    {
        return true;
    }
    
    // test if the name is the same
    if (name != node.name) 
    {
        return false;
    }
    
    // test if the index is the same
    if (index != node.index) 
    {
        return false;
    }
    
    // test if the parent is the same
    if (parent != node.parent) 
    {
        return false;
    }
    
    // test if the size of the children differs
    if (children.size() != node.children.size()) 
    {
        return false;
    }
    
    // test if all children are the same
    for (size_t i=0; i<children.size(); i++) 
    {
        if (children[i]->equals(*node.children[i])) 
        {
            return false;
        }
    }
    
    return true;
}


void TopologyNode::flagNewickRecomputation( void ) {
    
    if ( parent != NULL && !newickNeedsRefreshing ) 
    {
        parent->flagNewickRecomputation();
    }
    
    newickNeedsRefreshing = true;

}


/*
 * Get the Age.
 * We internally store the age so can return it. However, if we invalidated the age ( age = Inf ),
 * then we need to compute the age from the time.
 */
double TopologyNode::getAge( void ) const {
    return tree->getAge(index);
}


/*
 * Get the branch length.
 * We comput the difference of my time and my parents time.
 */
double TopologyNode::getBranchLength( void ) const {
    return tree->getBranchLength(index);
}


/** Get child at index i */
const TopologyNode& TopologyNode::getChild(size_t i) const {
    
    return *children[i];
}


/** Get child at index i */
TopologyNode& TopologyNode::getChild(size_t i) {
    
    return *children[i];
}


const std::vector<TopologyNode*>& TopologyNode::getChildren( void ) const {
    return children;
}


/** Loop over children and get their indices */
std::vector<int> TopologyNode::getChildrenIndices() const {
    
    std::vector<int> temp;
    
    for ( std::vector<TopologyNode* >::const_iterator i=children.begin(); i!=children.end(); i++ )
        temp.push_back( int( (*i)->getIndex() ) );
    
    return temp;
}


size_t TopologyNode::getIndex( void ) const {
    
    return index;
}


/**
 * Get the maximal depth starting from this node. 
 * The depth here mean the maximal path length along the branches until a terminal node (tip) is reached.
 * For ultrametric trees all path lengths are equivalent, but for serial sampled trees not.
 * Hence, we compute the maximal depths by recursively exploring each path along the branches to the children.
 *
 * \return    The maximal depth (path length) from this node to the most recent tip.
 */
double TopologyNode::getMaxDepth( void ) const
{
    
    // iterate over the childen
    double max = 0.0;
    for (std::vector<TopologyNode*>::const_iterator it = children.begin(); it != children.end(); ++it) 
    {
        double m = 0.0;
        TopologyNode& node = *(*it);
        if ( node.isTip() ) 
        {
            m = node.getBranchLength();
        }
        else
        {
            m = node.getBranchLength() + node.getMaxDepth();
        }
        
        if ( m > max ) 
        {
            max = m;
        }
    }
    
    return max;
}


const std::string& TopologyNode::getName( void ) const 
{
    
    return name;
}


size_t TopologyNode::getNumberOfChildren( void ) const {
    
    return children.size();
}


/**
 * Get the number of nodes contained in the subtree starting with this node as the root.
 * This either returns 1 if this is a tip node (or 0 if we do not count tipes) 
 * or computes recursively the number of nodes in both children plus one for this node.
 *
 * \param[in]   tips       Shall we count tips?
 * \return                 Subtree size.
 */
size_t TopologyNode::getNumberOfNodesInSubtree( bool countTips ) const
{
    
    if ( tipNode ) 
    {
        return (countTips ? 1 : 0);
    }
    else
    {
        return children[0]->getNumberOfNodesInSubtree(countTips) + children[1]->getNumberOfNodesInSubtree(countTips) + 1; 
    }
    
}

TopologyNode& TopologyNode::getParent(void) { 
    
    return *parent; 
    
}

const TopologyNode& TopologyNode::getParent(void) const { 
    
    return *parent; 
}


std::vector<std::string> TopologyNode::getTaxaStringVector( void ) const {
    
    if ( isTip() ) 
    {
        return std::vector<std::string>(1,name);
    }
    else 
    {
        std::vector<std::string> taxa;
        for ( std::vector<TopologyNode* >::const_iterator i=children.begin(); i!=children.end(); i++ ) 
        {
            std::vector<std::string> names = (*i)->getTaxaStringVector();
            taxa.insert( taxa.begin(), names.begin(), names.end() );
        }
        return taxa;
    }
}


double TopologyNode::getTime( void ) const {
    
    return tree->getTime( index );
}


double TopologyNode::getTmrca(const TopologyNode &n) const {
    
    std::vector<std::string> myTaxa   = getTaxaStringVector();
    std::vector<std::string> yourTaxa = n.getTaxaStringVector();
    
    if ( myTaxa.size() < yourTaxa.size() ) 
    {
        return -1;
    }
    
    for (std::vector<std::string>::const_iterator y_it = yourTaxa.begin(); y_it != yourTaxa.end(); ++y_it) 
    {
        bool found = false;
        for (std::vector<std::string>::const_iterator it = myTaxa.begin(); it != myTaxa.end(); ++it) 
        {
            if ( *y_it == *it ) 
            {
                found = true;
                break;
            }
        }
        
        if (!found) 
        {
            return -1;
        }
    }
    
    if ( myTaxa.size() == yourTaxa.size() ) 
    {
        return getAge();
    } 
    else 
    {
        double tmrca = -1;
        bool contains = false;
        for (std::vector<TopologyNode*>::const_iterator it = children.begin(); it != children.end(); ++it) 
        {
            tmrca = (*it)->getTmrca(n);
            contains |= ( tmrca >= 0 );
            if ( contains ) 
            {
                break;
            }
        }
        return tmrca;
    }
}


void TopologyNode::initiateFlaggingForNewickRecomputation( void ) {
    
    for (size_t i = 0; i < getNumberOfChildren(); ++i) 
    {
        getChild( i ).flagNewickRecomputation();
    }

}


bool TopologyNode::isRoot( void ) const {
    
    return rootNode;
}


bool TopologyNode::isTip( void ) const {
    
    return tipNode;
}


/** Remove all children. We need to call intelligently the destructor here. */
void TopologyNode::removeAllChildren(void) {
    
    size_t nChildren = children.size();
    // empty the children vector
    while (children.size() > 0) 
    {
        TopologyNode* theNode = children[0];
        // free the memory
        delete theNode;
        nChildren = children.size();
    }
    
    name = "";
    
    // mark for newick recomputation
    flagNewickRecomputation();
    
    tipNode = true;
    interiorNode = false;
}




/** Remove a child from the vector of children */
void TopologyNode::removeChild(TopologyNode* c, bool forceNewickRecomp) {
    
    std::vector<TopologyNode* >::iterator it = find(children.begin(), children.end(), c);
    if ( it != children.end() ) 
    {
        children.erase(it);
        //        delete p;
    }
    else 
        throw(RbException("Cannot find node in list of children nodes"));
    
    tipNode = children.size() == 0;
    interiorNode = children.size() > 0;
    
    // fire tree change event
    if ( tree != NULL ) 
    {
        tree->getTreeChangeEventHandler().fire( *c );
        tree->getTreeChangeEventHandler().fire( *this );
    }
    
    // mark for newick recomputation
    if (forceNewickRecomp)
        flagNewickRecomputation();
}

void TopologyNode::setIndex( size_t idx) {
    
    index = idx;
    
}

void TopologyNode::setName(std::string const &n) {
    
    name = n;
    
    // mark for newick recomputation
    flagNewickRecomputation();

}


void TopologyNode::setParent(TopologyNode* p, bool forceNewickRecomp) {
    
    // we only do something if this isn't already our parent
    if (p != parent) 
    {
        // we do not own the parent so we do not have to delete it
        parent = p;
        
        // mark for newick recomputation
        if (forceNewickRecomp)
            flagNewickRecomputation();
        
        // fire tree change event
        if ( tree != NULL ) 
        {
            tree->getTreeChangeEventHandler().fire( *this );
        }
        
    }
    
    rootNode = false;
}


void TopologyNode::setTree(Tree *t) {
    
    tree = t;
    for (std::vector<TopologyNode *>::iterator i = children.begin(); i != children.end(); ++i) 
    {
        (*i)->setTree( t );
    }
    
}

