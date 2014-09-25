//
//  AdmixtureNode.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/19/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//



/**
 * @file
 * This file contains the declaration of a AdmixtureNode.
 *
 * @brief Declaration of AdmixtureNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-11-14 02:23:29 -0800 (Wed, 14 Nov 2012) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id: AdmixtureNode.cpp 1832 2012-11-14 10:23:29Z hoehna $
 */


#include "AdmixtureNode.h"
#include "AdmixtureTree.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbOptions.h"
#include "RbSettings.h"
#include "Tree.h"
#include "TreeChangeEventHandler.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

/** Default constructor (interior node, no name). Give the node an optional index ID */
AdmixtureNode::AdmixtureNode(int indx) : TopologyNode( indx ), parent( NULL ), topology( NULL ), age(0.0), admixtureNode(false), outgroup(false), admixtureParent(NULL), admixtureChild(NULL), weight(1.0) {
    
}


/** Constructor of node with name. Give the node an optional index ID */
AdmixtureNode::AdmixtureNode(const std::string& n, int indx) : TopologyNode( n, indx ), parent( NULL ), topology( NULL ), age(0.0), admixtureNode(false), outgroup(false), admixtureParent(NULL), admixtureChild(NULL), weight(1.0) {
    
}


/** Copy constructor. We use a shallow copy. */
AdmixtureNode::AdmixtureNode(const AdmixtureNode &n) : TopologyNode( n ), topology( NULL ) {
    
    // copy the members
    parent          = n.parent;
    
    // copy the children
    for (std::vector<AdmixtureNode*>::const_iterator it = n.children.begin(); it != n.children.end(); it++) {
        AdmixtureNode* theNode = *it;
        AdmixtureNode* theClone = theNode->clone();
        children.push_back( theClone );
        theClone->setParent(this);
    }
    
    index = n.index;
    age = n.age;
    weight = n.weight;
    admixtureParent = n.admixtureParent;
    admixtureChild = n.admixtureChild;
    admixtureNode = n.admixtureNode;
    outgroup = n.outgroup;
    
//    if (admixtureChild != NULL)
//        admixtureChild->setAdmixtureParent(&n);
    
}


/** Destructor */
AdmixtureNode::~AdmixtureNode(void) {
    
    // we do not own the parent so we do not delete it
    
    // free memory of children
    removeAllChildren();
    
    // make sure that I was removed from my parent
    if (parent != NULL)
        parent->removeChild(this);
    
}


AdmixtureNode& AdmixtureNode::operator=(const AdmixtureNode &n) {
    
    if (this == &n) {
        // delegate to base class first
        TopologyNode::operator=( n );
        
        removeAllChildren();
        
        // copy the members
        parent          = n.parent;
        
        // copy the children
        for (std::vector<AdmixtureNode*>::const_iterator it = n.children.begin(); it != n.children.end(); it++) {
            children.push_back( (*it)->clone() );
        }
        
        age = n.age;
        weight = n.weight;
        admixtureParent = n.admixtureParent;
        admixtureChild = n.admixtureChild;
        admixtureNode = n.admixtureNode;
        outgroup = n.outgroup;
        
        // add myself as a new child to the parent node
        parent->addChild(this);
        
        // topology
        topology = NULL;
        
        // add as admixtureChild if applicable
        //if (admixtureParent != NULL)
        //    admixtureParent->setAdmixtureChild(this);
            
    }
    
    return *this;
}


/** Add a child node. We own it from here on. */
void AdmixtureNode::addChild(TopologyNode* c, bool enforceNewickRecomp) {
    
    //std::cout << "addChild ( TopologyNode* c )\n";
    AdmixtureNode *tn = dynamic_cast<AdmixtureNode*>( c );
    if ( tn != NULL ) {
        addChild( tn, enforceNewickRecomp );
    }
    else {
        throw RbException("Cannot add a child of type non-AdmixtureNode to a AdmixtureNode.");
    }
    
}


/** Add a child node. We own it from here on. */
void AdmixtureNode::addChild(AdmixtureNode* c, bool enforceNewickRecomp) {

    //std::cout << "addChild ( AdmixtureNode* c )\n";
    
    // add the child to our internal vector
    children.push_back(c);
    
    // mark for newick recomputation
    if (enforceNewickRecomp)
        flagNewickRecomputation();
    
    // fire tree change event
    if ( topology != NULL ) {
    //    topology->getTreeChangeEventHandler().fire( *this );
    //    topology->getTreeChangeEventHandler().fire( *c );
    }
    
    tipNode = false;
    admixtureNode = children.size() == 1;
    interiorNode = children.size() > 1;

}

/** Add a child node. We own it from here on. */
void AdmixtureNode::setChild(TopologyNode* c, size_t idx) {
    
    AdmixtureNode *tn = dynamic_cast<AdmixtureNode*>( c );
    if ( tn != NULL ) {
        setChild( tn, idx );
    }
    else {
        throw RbException("Cannot add a child of type non-AdmixtureNode to a AdmixtureNode.");
    }
    
}


/** Add a child node. We own it from here on. */
void AdmixtureNode::setChild(AdmixtureNode* c, size_t idx) {
    
    // add the child to our internal vector
    children[idx] = c;
    
    // mark for newick recomputation
    flagNewickRecomputation();
    
    // fire tree change event
    if ( topology != NULL ) {
     //   topology->getTreeChangeEventHandler().fire( *this );
     //   topology->getTreeChangeEventHandler().fire( *c );
    }
    
    tipNode = false;
    admixtureNode = children.size() == 1;
    interiorNode = children.size() > 1;
    
}


/** Clone function */
AdmixtureNode* AdmixtureNode::clone(void) const {
    
    return new AdmixtureNode(*this);
}


bool AdmixtureNode::equals(const TopologyNode& node) const {
    
    const AdmixtureNode *tn = dynamic_cast<const AdmixtureNode*>( &node );
    if ( tn != NULL ) {
        return equals( *tn );
    }
    
    return false;
}


bool AdmixtureNode::equals(const AdmixtureNode& node) const {
    
    if (this == &node) {
        return true;
    }
    // test if the name is the same
    if (taxon != node.taxon) {
        return false;
    }
    
    // test if the index is the same
    if (index != node.index) {
        return false;
    }
    
    // test if the parent is the same
    if (parent != node.parent) {
        return false;
    }
    
    // test if the size of the children differs
    if (children.size() != node.children.size()) {
        return false;
    }
    
    // test if all children are the same
    for (size_t i=0; i<children.size(); i++) {
        if (children[i]->equals(*node.children[i])) {
            return false;
        }
    }
    
    // rest of properties
    //...
    
    return true;
}


void AdmixtureNode::flagNewickRecomputation( void ) {
    
    if ( parent != NULL) {
        parent->flagNewickRecomputation();
    }
    
    newickNeedsRefreshing = true;
}


/*
 * Get the Age.
 * We internally store the age so can return it. However, if we invalidated the age ( age = Inf ),
 * then we need to compute the age from the time.
 */
double AdmixtureNode::getAge( void ) const {
    //return topology->getAge( index );
    return age;
}


/*
 * Get the branch length.
 * We comput the difference of my time and my parents time.
 */
double AdmixtureNode::getBranchLength( void ) const {
    if ( parent == NULL ) {
        return 0.0;
    }
    else {
        return parent->getAge() - getAge();
    }
}

double AdmixtureNode::getTopologyBranchLength(void) const {
    if (parent == NULL) {
        return 0.0;
    }
    else
        return getTopologyParent().getAge() - getAge();
}

/** Get child at index i */
const AdmixtureNode& AdmixtureNode::getChild(size_t i) const {
    
    return *children[i];
}


/** Get child at index i */
AdmixtureNode& AdmixtureNode::getChild(size_t i) {
    
    return *children[i];
}


/** Loop over children and get their indices */
std::vector<int> AdmixtureNode::getChildrenIndices() const {
    
    std::vector<int> temp;
    
    for ( std::vector<AdmixtureNode* >::const_iterator i=children.begin(); i!=children.end(); i++ )
        temp.push_back( int( (*i)->getIndex() ) );
    
    return temp;
}


size_t AdmixtureNode::getNumberOfChildren( void ) const {
    return children.size();
}

AdmixtureNode& AdmixtureNode::getParent(void) {
    return *parent;
    
}

const AdmixtureNode& AdmixtureNode::getParent(void) const {
    return *parent;
    
}

AdmixtureNode& AdmixtureNode::getTopologyParent(void) {
    
    AdmixtureNode* p = parent;
    
    while (p->isAdmixtureNode())
    {
        p = &p->getParent();
    }
    return *p;
    
}

const AdmixtureNode& AdmixtureNode::getTopologyParent(void) const {

    AdmixtureNode* p = parent;
    
    while (p->isAdmixtureNode())
    {
        p = &p->getParent();
    }
    return *p;
    
}

AdmixtureNode& AdmixtureNode::getTopologyChild(size_t i) {
    
    
    AdmixtureNode* p = children[i];
   
    while (p->getNumberOfChildren() == 1)
    {
        p = &p->getChild(0);
    }
    return *p;
    
}

const AdmixtureNode& AdmixtureNode::getTopologyChild(size_t i) const {
    
    AdmixtureNode* p = children[i];

    
    // should skip over admixtureNode==true
    while (p->getNumberOfChildren() == 1)
    {
        p = &p->getChild(0);
    }
    return *p;
    
}

/** Add a child node. We own it from here on. */
void AdmixtureNode::addTopologyChild(TopologyNode* c) {
    
    AdmixtureNode *tn = dynamic_cast<AdmixtureNode*>( c );
    if ( tn != NULL ) {
        addTopologyChild( tn );
    }
    else {
        throw RbException("Cannot add a child of type non-AdmixtureNode to a AdmixtureNode.");
    }
    
}


/** Add a child node. We own it from here on. */
void AdmixtureNode::addTopologyChild(AdmixtureNode* c) {
    
    // add the child to our internal vector
    children.push_back(c);
    
    // mark for newick recomputation
    flagNewickRecomputation();
    
    // fire tree change event
    if ( topology != NULL ) {
    //    topology->getTreeChangeEventHandler().fire( *this );
     //   topology->getTreeChangeEventHandler().fire( *c );
    }
    
    tipNode = false;
    admixtureNode = children.size() == 1;
    interiorNode = children.size() > 1;
    
}

/** Remove all children. We need to call intelligently the destructor here. */
void AdmixtureNode::removeAllTopologyChildren(void) {
    
    /*
     size_t nChildren = children.size();
     // empty the children vector
     while (children.size() > 0) {
     AdmixtureNode* theNode = children[0];
     // free the memory
     delete theNode;
     nChildren = children.size();
     }
     */
    children.clear();
    
    taxon = Taxon();
    
    // mark for newick recomputation
    flagNewickRecomputation();
    
    tipNode = true;
    admixtureNode = false;
    interiorNode = false;
}

/** Remove a child from the vector of children */
void AdmixtureNode::removeTopologyChild(TopologyNode* c) {
    
    AdmixtureNode *tn = dynamic_cast<AdmixtureNode*>( c );
    
    if ( tn != NULL ) {
        removeChild( tn );
    }
    else {
        throw RbException("Cannot remove a child of type non-AdmixtureNode to a AdmixtureNode.");
    }
}


/** Remove a child from the vector of children */
void AdmixtureNode::removeTopologyChild(AdmixtureNode* c) {
    
    std::vector<AdmixtureNode* >::iterator it = find(children.begin(), children.end(), c);
    if ( it != children.end() ) {
        children.erase(it);
        //        delete p;
    }
    else
        throw(RbException("Cannot find node in list of children nodes"));
    
    tipNode = children.size() == 0;
    admixtureNode = children.size() == 1;
    interiorNode = children.size() > 1;
    
    // fire tree change event
    if ( topology != NULL ) {
     //   topology->getTreeChangeEventHandler().fire( *c );
     //   topology->getTreeChangeEventHandler().fire( *this );
    }
    
    // mark for newick recomputation
    flagNewickRecomputation();
}

void AdmixtureNode::setTopologyParent(TopologyNode* p) {
    
    AdmixtureNode *tn = dynamic_cast<AdmixtureNode*>( p );
    
    if ( tn != NULL ) {
        setTopologyParent( tn );
    }
    else {
        throw RbException("Cannot set the parent of type non-AdmixtureNode to a AdmixtureNode.");
    }
    
}



void AdmixtureNode::setTopologyParent(AdmixtureNode* p) {
    
    // we only do something if this isn't already our parent
    if (p != parent) {
    
        // find the correct node
        AdmixtureNode* q = parent;
        while (p->getAge() > q->getAge())
        {
            q = &q->getParent();
        }
        
        q->setParent(p);
        
        // mark for newick recomputation
        flagNewickRecomputation();
        
        // fire tree change event
        if ( topology != NULL ) {
         //   topology->getTreeChangeEventHandler().fire( *p );
          //  topology->getTreeChangeEventHandler().fire( *this );
        }
    }
    
    rootNode = false;
}

bool AdmixtureNode::isOutgroup(void)
{
    return outgroup;
}

bool AdmixtureNode::setOutgroup(bool tf)
{
    outgroup = tf;
    return tf;
}

bool AdmixtureNode::isAdmixtureNode(void)
{
    return admixtureNode;
}

bool AdmixtureNode::isInteriorNode(void)
{
    return interiorNode;
}

bool AdmixtureNode::checkChildren(void)
{
    size_t nChild = children.size();
    for (size_t i = 0; i < nChild; i++)
        if (this == &children[i]->getParent())
            return true;
    return false;
}

bool AdmixtureNode::checkParent(void)
{
    size_t nChild = parent->getNumberOfChildren();
    for (size_t i = 0; i < nChild; i++)
        if (this == &parent->getChild(i))
            return true;
    return false;
}



double AdmixtureNode::getTime( void ) const {
    
    if (parent == NULL ) {
        return 0.0;
    }
    else {
        return parent->getTime() + parent->getAge() - getAge();
    }
}

AdmixtureNode& AdmixtureNode::getAdmixtureParent(void)
{
    return *admixtureParent;
}

const AdmixtureNode& AdmixtureNode::getAdmixtureParent(void) const
{
    return *admixtureParent;
}

AdmixtureNode& AdmixtureNode::getAdmixtureChild(void)
{
    return *admixtureChild;
}

const AdmixtureNode& AdmixtureNode::getAdmixtureChild(void) const
{
    return *admixtureChild;
}

double AdmixtureNode::getWeight(void)
{
    return weight;
}

void AdmixtureNode::setAdmixtureParent(AdmixtureNode* p)
{
    admixtureParent = p;
    admixtureNode = admixtureParent != NULL;
}

void AdmixtureNode::setAdmixtureChild(AdmixtureNode* p)
{
    admixtureChild = p;
    admixtureNode = admixtureChild != NULL;
}

void AdmixtureNode::setWeight(double w)
{
    weight = w;
}


/** Remove all children. We need to call intelligently the destructor here. */
void AdmixtureNode::removeAllChildren(void) {
    
    /*
    size_t nChildren = children.size();
    // empty the children vector
    while (children.size() > 0) {
        AdmixtureNode* theNode = children[0];
        // free the memory
        delete theNode;
        nChildren = children.size();
    }
    */
    children.clear();
    
    taxon = Taxon();
    
    // mark for newick recomputation
    flagNewickRecomputation();
    
    tipNode = true;
    admixtureNode = false;
    interiorNode = false;
}

/** Remove a child from the vector of children */
void AdmixtureNode::removeChild(TopologyNode* c, bool enforceNewickRecomp) {
    
    AdmixtureNode *tn = dynamic_cast<AdmixtureNode*>( c );
    
    if ( tn != NULL ) {
        removeChild( tn, enforceNewickRecomp );
    }
    else {
        throw RbException("Cannot remove a child of type non-AdmixtureNode to a AdmixtureNode.");
    }
}


/** Remove a child from the vector of children */
void AdmixtureNode::removeChild(AdmixtureNode* c, bool enforceNewickRecomp) {
    
    std::vector<AdmixtureNode* >::iterator it = find(children.begin(), children.end(), c);
    if ( it != children.end() ) {
        children.erase(it);
        //        delete p;
    }
    else
    {
        std::stringstream ss_err;
        ss_err << "Cannot find node " << c << " in list of children nodes for " << this;
        std::cout << ss_err.str() << "\n";
       // throw(RbException(ss_err.str()));
//        throw(RbException("Cannot find node in list of children nodes"));
    }
    
    tipNode = children.size() == 0;
    admixtureNode = children.size() == 1;
    interiorNode = children.size() > 1;
    
    // fire tree change event
    if ( topology != NULL ) {
       // topology->getTreeChangeEventHandler().fire( *c );
       // topology->getTreeChangeEventHandler().fire( *this );
    }
    
    // mark for newick recomputation
    if (enforceNewickRecomp)
        flagNewickRecomputation();
}


void AdmixtureNode::setAge( double a ) {
    
#ifdef ASSERTIONS_TREE
    if ( a < 0.0 ) {
        throw RbException("Cannot set age to a negative value.");
    }
#endif
    
    // we delegate the call to the tree
    //topology->setAge(index, a);
    age = a;
    
    // mark for newick recomputation
    initiateFlaggingForNewickRecomputation();
    
    // fire tree change event
    if ( topology != NULL ) {
        if ( isTip() ) {
       //     topology->getTreeChangeEventHandler().fire( *this );
        }
        else {
       //     topology->getTreeChangeEventHandler().fire( getChild(0) );
        //    topology->getTreeChangeEventHandler().fire( getChild(1) );
        }
    }
    
}




void AdmixtureNode::setParent(TopologyNode* p, bool enforceNewickRecomp) {
    
    AdmixtureNode *tn = dynamic_cast<AdmixtureNode*>( p );
    
    if ( tn != NULL ) {
        setParent( tn, enforceNewickRecomp );
    }
    else {
        throw RbException("Cannot set the parent of type non-AdmixtureNode to a AdmixtureNode.");
    }
    
}



void AdmixtureNode::setParent(AdmixtureNode* p, bool enforceNewickRecomp) {
    
    // we only do something if this isn't already our parent
    if (p != parent) {
        // we do not own the parent so we do not have to delete it
        parent = p;
        
        // mark for newick recomputation
        if (enforceNewickRecomp)
            flagNewickRecomputation();
        
        // fire tree change event
        if ( topology != NULL ) {
        //    topology->getTreeChangeEventHandler().fire( *p );
         //   topology->getTreeChangeEventHandler().fire( *this );
        }
        
    }
    
    rootNode = false;
}


void AdmixtureNode::setTopology(AdmixtureTree *t) {
    topology = t;
    for (std::vector<AdmixtureNode *>::iterator i = children.begin(); i != children.end(); ++i) {
        (*i)->setTopology( t );
    }
}

bool AdmixtureNode::operator<( const AdmixtureNode*& other) const
{
    if ( topology->getAge(index) < other->getAge() )
        return true;
    else
        return false;
}


const std::string& AdmixtureNode::computeNewick(void)
{
    // check if we need to recompute
    if ( newickNeedsRefreshing ) {
        newick = buildNewickString();
        newickNeedsRefreshing = false;
    }
    
    return newick;
}

std::string AdmixtureNode::buildNewickString(void)
{
    // create the newick string
    std::stringstream o;
    
    // test whether this is a internal or external node
    if (tipNode) {
        // this is a tip so we just return the name of the node
        o << taxon.getName();
        if ( nodeComments.size() > 0 ) {
            o << "[&";
            for (size_t i = 0; i < nodeComments.size(); ++i) {
                if ( i > 0 ) {
                    o << ",";
                }
                o << nodeComments[i];
            }
            o << "]";
        }
        
        if ( topology != NULL ) {
            o << ":" << getTopologyBranchLength();
        }
        if ( branchComments.size() > 0 ) {
            o << "[&";
            for (size_t i = 0; i < branchComments.size(); ++i) {
                if ( i > 0 ) {
                    o << ",";
                }
                o << branchComments[i];
            }
            o << "]";
        }
    }
    else {
        o << "(";
        for (size_t i=0; i<(getNumberOfChildren()-1); i++) {
            o << getTopologyChild(i).computeNewick() << ",";
        }
        o << getTopologyChild(getNumberOfChildren()-1).computeNewick() << ")";
        if ( nodeComments.size() > 0 ) {
            o << "[&";
            for (size_t i = 0; i < nodeComments.size(); ++i) {
                if ( i > 0 ) {
                    o << ",";
                }
                o << nodeComments[i];
            }
            o << "]";
        }
        if ( topology != NULL ) {
            o << ":" << getTopologyBranchLength();
        }
        if ( branchComments.size() > 0 ) {
            o << "[&";
            for (size_t i = 0; i < branchComments.size(); ++i) {
                if ( i > 0 ) {
                    o << ",";
                }
                o << branchComments[i];
            }
            o << "]";
        }
    }
    std::cout << "TEST " << o.str() << "\n";
    return o.str();
}

void AdmixtureNode::setNewickNeedsRefreshing(bool tf)
{
    newickNeedsRefreshing = tf;
}

