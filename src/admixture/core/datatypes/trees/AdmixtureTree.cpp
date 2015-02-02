//
//  AdmixtureTree.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//


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
 * $Id: AdmixtureTree.cpp 1651 2012-07-05 14:47:08Z hoehna $
 */



#include "AdmixtureTree.h"
#include "TopologyNode.h"
#include "RbException.h"
#include "RbOptions.h"
#include <algorithm>
#include <iostream>
#include <string>

using namespace RevBayesCore;


/* Default constructor */
AdmixtureTree::AdmixtureTree(void) : Tree(), root( NULL ), numTips( 0 ), numNodes( 0 ) {
    
}


AdmixtureTree::AdmixtureTree(const std::vector<std::string>& n) : Tree(), root( NULL ), numTips( 0 ), numNodes( 0 ), names(n) {
    
}


/* Copy constructor */
//AdmixtureTree::AdmixtureTree(const AdmixtureTree& t) : Tree( t ), root( NULL ) {
AdmixtureTree::AdmixtureTree(const AdmixtureTree& t) : root( NULL ) {
    
    // set the parameters
    binary      = t.binary;
    numTips     = t.numTips;
    numNodes    = t.numNodes;
    names = t.names;
    
    // need to perform a deep copy of the AdmixtureTree nodes
    if (t.root != NULL) {
        root = t.getRoot().clone();
        
        setRoot(root);
        
        // set this topology for the nodes
        root->setTopology( this );
        
        fillNodesByIndex();
        
        updateTipOrderByNames(names);
    }
    
}

/* Destructor */
AdmixtureTree::~AdmixtureTree(void) {
    
    nodes.clear();
    nodesByIndex.clear();
    
    delete root;
}


AdmixtureTree& AdmixtureTree::operator=(const AdmixtureTree &t) {
    
    
    ;
    
    if (this != &t) {
        //Tree::operator=(t);
        
        
        nodes.clear();
        nodesByIndex.clear();
        
        //delete root;
       // root = NULL;
        
        binary      = t.binary;
        numTips     = t.numTips;
        numNodes    = t.numNodes;
        nodesByIndex = t.nodesByIndex;
        names = t.names;
        
        TopologyNode* newRoot = t.root->clone();
        
        // set the root. This will also set the nodes vector.
        setRoot(newRoot);
        
        // set this topology for the nodes
        root->setTopology( this );
        
        updateTipOrderByNames(names);
        
    }
    
    return *this;
}


/* Clone function */
AdmixtureTree* AdmixtureTree::clone(void) const {
    ;
    return new AdmixtureTree(*this);
}


void AdmixtureTree::equalizeBranchLengths( void ) {
    
    equalizeBranchLengths( *root );
}



void AdmixtureTree::equalizeBranchLengths( AdmixtureNode &node ) {
    
    if ( !node.isTip() ) {
        equalizeBranchLengths( node.getChild(0) );
        equalizeBranchLengths( node.getChild(1) );
        
        double left = node.getChild(0).getAge() + node.getChild( 0 ).getBranchLength();
        double right = node.getChild(1).getAge() + node.getChild( 1 ).getBranchLength();
        std::cout << node.getAge() << " = " << ((left + right) / 2.0) << std::endl;
        node.setAge( (left + right) / 2.0 );
    }
    
}





void AdmixtureTree::fillNodeTimes( void ) {
    
    fillNodeTimes( *root );
}


void AdmixtureTree::fillNodeTimes( AdmixtureNode &node ) {
    
    if ( !node.isTip() ) {
        fillNodeTimes( node.getChild(0) );
        fillNodeTimes( node.getChild(1) );
    }
    
    node.getAge();
    node.getBranchLength();
    node.getTime();
    
}

bool AdmixtureTree::updateTopologicalSort(void)
{
    nodesByTopologicalSort.clear();
    markTopologicalSortGlobal.clear();
    std::map<AdmixtureNode*,bool> markTopologicalSortLocal;
    
    return fillNodesByTopologicalSort(root, markTopologicalSortLocal);
}

bool AdmixtureTree::fillNodesByTopologicalSort(AdmixtureNode* node, std::map<AdmixtureNode*,bool> markTopologicalSortLocal)
{
    
    // if local topological sort recursion finds a repeated node, then graph contains a cycle
    if (markTopologicalSortLocal.find(node) != markTopologicalSortLocal.end())
        return false;
    
    // if global sort has not visited node, proceed
    if (markTopologicalSortGlobal.find(node) == markTopologicalSortGlobal.end())
    {

        // flag global and local visits
        markTopologicalSortLocal[node] = true;
        markTopologicalSortGlobal[node] = true;
        
        // recurse over tree children
        size_t nCh = node->getNumberOfChildren();
        for (size_t i = 0; i < nCh; i++)
            if (fillNodesByTopologicalSort(&node->getChild(i), markTopologicalSortLocal) == false)
                return false;
        
        // recure over admixture children
        if (&node->getAdmixtureChild() != NULL)
            if (fillNodesByTopologicalSort(&node->getAdmixtureChild(), markTopologicalSortLocal) == false)
                return false;
        
        nodesByTopologicalSort.push_back(node);
        
    }
    
    // if no cycle was found, then the admixture graph is a DAG
    return true;
    
}


/* fill the nodes vector by a preorder traversal recursively starting with this node. */
void AdmixtureTree::fillNodesByPreorderTraversal(AdmixtureNode* node) {
    
    // this is preorder so add yourself first
    nodes.push_back(node);
    
    // now call this function recursively for all your children
    for (size_t i=0; i<node->getNumberOfChildren(); i++) {
        fillNodesByPreorderTraversal(&node->getChild(i));
    }
}

/* fill the nodes vector by a phylogenetic traversal recursively starting with this node.
 * The tips fill the slots 0,...,n-1 followed by the internal nodes and then the root.
 */
void AdmixtureTree::fillNodesByPhylogeneticTraversal(AdmixtureNode* node) {
    
    // now call this function recursively for all your children
    for (size_t i=0; i<node->getNumberOfChildren(); i++) {
        fillNodesByPhylogeneticTraversal(&node->getChild(i));
    }
    
    if (node->isTip()) {
        // all the tips go to the beginning
        nodes.insert(nodes.begin(), node);
    }
    else {
        // this is phylogenetic ordering so the internal nodes come last
        nodes.push_back(node);
    }
}


std::vector<TopologyNode *> AdmixtureTree::getNodes( void ) const {
    std::vector<TopologyNode *> tmp( nodes.size() );
    for (size_t i = 0; i < nodes.size(); ++i) {
        tmp[i] = nodes[i];
    }
    return tmp;
}


AdmixtureNode& AdmixtureTree::getNode(size_t idx) {
    return *nodes[idx];
}


const AdmixtureNode& AdmixtureTree::getNode(size_t idx) const {
    return *nodes[idx];
}

std::vector<TopologyNode *> AdmixtureTree::getNodesByIndex( void ) const {
    std::vector<TopologyNode *> tmp( nodesByIndex.size() );
    for (size_t i = 0; i < nodesByIndex.size(); ++i) {
        tmp[i] = nodesByIndex[i];
    }
    return tmp;
}

void AdmixtureTree::fillNodesByIndex(void)
{
    size_t n = nodes.size();
    nodesByIndex = std::vector<AdmixtureNode*>(n);
    
    for (size_t i = 0; i < n; i++)
        nodesByIndex[i] = nodes[i];
}



double AdmixtureTree::getAge(size_t idx) const {
    return ages[idx];
}



double AdmixtureTree::getBranchLength(size_t idx) const {
    
    size_t parentIdx = nodesByIndex[idx]->getParent().getIndex();
    return ages[parentIdx] - ages[idx];
}

/** Calculate the number of interior nodes in the AdmixtureTree by deducing the number of
 tips from number of nodes, and then subtract 1 more if the AdmixtureTree is rooted. */
size_t AdmixtureTree::getNumberOfInteriorNodes( void ) const {
    
    size_t n = 0;
    for (size_t i=0; i<nodes.size(); i++) {
        if (nodes[i]->getNumberOfChildren() == 2)
            n++;
    }
    
    if ( isRooted() )
        return n - 1;
    else
        return n;

}


size_t AdmixtureTree::getNumberOfNodes( void ) const {
    return nodes.size();
}

size_t AdmixtureTree::getNumberOfAdmixtureParents() const {
    
    size_t n = 0;
    for (size_t i=0; i<nodes.size(); i++) {
        if (&nodes[i]->getAdmixtureChild() != NULL)
            n++;
    }
    return n;
}

std::vector<AdmixtureNode*> AdmixtureTree::getAdmixtureParents() const {
    
    std::vector<AdmixtureNode *> tmp;
    for (size_t i = 0; i < nodes.size(); i++) {
        if (&nodes[i]->getAdmixtureChild() != NULL)
            tmp.push_back(nodes[i]);
    }
    return tmp;
}

std::vector<AdmixtureNode*> AdmixtureTree::getFirstAdmixtureParentPerLineage(void) const {
    
    std::vector<AdmixtureNode *> tmp;
    for (size_t i = 0; i < nodes.size(); i++)
    {
        AdmixtureNode* p = nodes[i];
        
        // if admixture edge, and both admixture nodes' parents are divergence nodes
        if (&p->getAdmixtureChild() != NULL && p->getParent().getNumberOfChildren() == 2 && p->getAdmixtureChild().getParent().getNumberOfChildren() == 2)
        {
            //std::cout << "gfappl " << i << "\n";
            tmp.push_back(p);
        }
    }
    
    for (size_t i = 0; i < tmp.size(); i++)
    {
        ;//  std::cout << tmp[i] << "  " << tmp[i]->getParent().getNumberOfChildren() << "\n";
    }
    return tmp;
}

size_t AdmixtureTree::getNumberOfAdmixtureChildren() const {
    
    size_t n = 0;
    for (size_t i=0; i<nodes.size(); i++) {
        if (&nodes[i]->getAdmixtureParent() != NULL)
            n++;
    }
    //std::cout << n << "\t" << nodes.size() << "\n";
    return n;
}


double AdmixtureTree::getTime(size_t idx) const {
    return times[idx];
}

void AdmixtureTree::eraseAdmixtureNode(AdmixtureNode *p)
{
    std::vector<AdmixtureNode*>::iterator it;

    //@Michael: This doesn't compile on Windows using GCC 4.2
    //MJL -> MysteriousStranger: I added #include <algorithm> and std::find, hopefully fixing your problem.
    
    it = std::find(nodes.begin(), nodes.end(), p);
    nodes.erase(it);

}

void AdmixtureTree::checkAllEdges(void)
{
    for (size_t i = 0; i < nodes.size(); i++)
    {
        std::cout << &nodes[i];
        
        if (!nodes[i]->isRoot())
            std::cout << "\tcP=" << (nodes[i]->checkParent() ? "T" : "F");
    
        if (!nodes[i]->isTip())
            std::cout << "\tcC=" << (nodes[i]->checkChildren() ? "T" : "F");
        
        std::cout << "\n";
    }
}

void AdmixtureTree::checkAllEdgesRecursively(AdmixtureNode* p) const
{
    std::cout << "N " << p << "    ";
    
    std::cout << "idx      " << p->getIndex() << "    ";
    
    if (p->getAge() == 0.0)
        std::cout << "age 0.0        ";
    else
        std::cout << "age " << p->getAge() << "   ";

    if (!p->isRoot())
        std::cout << "P " << &p->getParent() << "    ";
    else
        std::cout << "P null           ";

    size_t nCh = p->getNumberOfChildren();
    if (nCh == 0)
    {
        std::cout << "C0 null           ";
        std::cout << "C1 null           ";
    }
    else if (nCh == 1)
    {
        std::cout << "C0 " << &p->getChild(0) << "    ";
        std::cout << "C1 null           ";
    }
    else if (nCh == 2)
    {
        std::cout << "C0 " << &p->getChild(0) << "    ";
        std::cout << "C1 " << &p->getChild(1) << "    ";
    }
    
    if (&p->getAdmixtureParent() != NULL)
        std::cout << "AP " << &p->getAdmixtureParent() << "    ";
    else
        std::cout << "AP null          ";

    if (&p->getAdmixtureChild() != NULL)
        std::cout << "AC " << &p->getAdmixtureChild() << "    ";
    else
        std::cout << "AC null           ";
    
    if (!p->isRoot())
        std::cout << "\tcP=" << (p->checkParent() ? "T" : "F");
    
    if (!p->isTip())
        std::cout << "\tcC=" << (p->checkChildren() ? "T" : "F");
    
    if (p->getNumberOfChildren() == 1 && p->isAdmixtureNode() && &p->getAdmixtureChild() != NULL && &p->getAdmixtureParent() == NULL)
        std::cout << "\tAP";
    
    else if (p->getNumberOfChildren() == 1 && p->isAdmixtureNode() && &p->getAdmixtureChild() == NULL && &p->getAdmixtureParent() != NULL)
        std::cout << "\tAC";
    
    else if (p->getNumberOfChildren() == 0 && p->isTip() && (&p->getAdmixtureChild() == NULL || &p->getAdmixtureParent() == NULL))
        std::cout << "\tTN";
    
    else if (p->getNumberOfChildren() == 2 && p->isInteriorNode() && (&p->getAdmixtureChild() == NULL || &p->getAdmixtureParent() == NULL))
        std::cout << "\tIN";
    
    else
        std::cout << "\t??";
        
    std::cout << "\n";

    for (size_t i = 0; i < nCh; i++)
        checkAllEdgesRecursively(&p->getChild(i));
}

void AdmixtureTree::pushAdmixtureNode(AdmixtureNode *p)
{
    nodes.push_back(p);
}

const std::string& AdmixtureTree::getNewickRepresentation( void ) const {
    root->setNewickNeedsRefreshing(true);
    return root->computeNewick();
}

/** Calculate and return the number of tips on the AdmixtureTree by going through the vector
 of nodes, querying each about its tip status. */
size_t AdmixtureTree::getNumberOfTips( void ) const {
    
    size_t n = 0;
    for (size_t i=0; i<nodes.size(); i++) {
        if (nodes[i]->isTip() == true)
            n++;
    }
    return n;
}


/** We provide this function to allow a caller to randomly pick one of the interior nodes.
 This version assumes that the root is always the last and the tips the first in the nodes vector. */
const AdmixtureNode& AdmixtureTree::getInteriorNode( int indx ) const {
    
    // \TODO: Bound checking, maybe draw from downpass array instead
    return *nodesByIndex[ indx + getNumberOfTips() ];
}


AdmixtureNode& AdmixtureTree::getRoot( void ) {
    
    return *root;
}


const AdmixtureNode& AdmixtureTree::getRoot( void ) const {
    
    return *root;
}


/** We provide this function to allow a caller to randomly pick one of the interior nodes.
 This version assumes that the tips are first in the nodes vector. */
AdmixtureNode& AdmixtureTree::getTipNode( size_t indx ) {
    
    return *nodes[ indx ];
}


/** We provide this function to allow a caller to randomly pick one of the interior nodes.
 This version assumes that the tips are first in the nodes vector. */
const AdmixtureNode& AdmixtureTree::getTipNode( size_t indx ) const {
    
    // \TODO: Bound checking
    return *nodes[ indx ];
}

double AdmixtureTree::getTreeLength(void) const {
    double treeLength = 0;
    for (size_t i = 0; i < nodes.size(); i++)
        treeLength += nodes[i]->getBranchLength();
    return treeLength;
}

double AdmixtureTree::getTreeHeight(void) const {
    return root->getAge();
}

double AdmixtureTree::getUnitTreeLength(void) const {
    
    return getTreeLength() / root->getAge();
}

bool AdmixtureTree::isBinary( void ) const {
    return binary;
}


bool AdmixtureTree::isRooted( void ) const {
    return true;
}


void AdmixtureTree::resizeElementVectors(size_t n) {
    // remove all elements
    ages.clear();
    times.clear();
    
    // resize to new dimension
    ages.resize( n );
    times.resize( n );
}


void AdmixtureTree::setAge(size_t idx, double a) {
    ages[idx] = a;
}


void AdmixtureTree::setRooted(bool tf) {
    // time tree are always rooted!!!
}



void AdmixtureTree::setRoot( TopologyNode* r) {
    
    AdmixtureNode *tn = static_cast<AdmixtureNode*>( r );
    setRoot( tn );
    
}

void AdmixtureTree::setRoot( AdmixtureNode* r) {
    
    // set the root
    root = r;
    
    root->setTopology( this );
    
    nodes.clear();
    
    // bootstrap all nodes from the root and add the in a pre-order traversal
    // fillNodesByPreorderTraversal(r);
    fillNodesByPhylogeneticTraversal(r);
    
    //size_t numTips = 0;
    //size_t tipIdx = 0;
    //size_t intIdx =0 ;
    
    for (size_t i = 0; i < nodes.size(); i++)
        nodes[i]->setIndex(i+1);
    
  //  for (size_t i = 0; i < nodes.size(); ++i) {
  ///      if (nodes[i]->isTip() == false)
//            nodes[i]->setIndex(i+1);
  //      else
  //          numTips++;
        //std::cout << nodes[i]->getName() << " " << nodes[i]->getIndex() << " " << nodes[i]->getAge() << "\n";
    //}
    
    
    /*
    std::vector<AdmixtureNode*> n2(numTips);
    for (size_t i = 0; i < numTips; i++)
        n2[nodes[i]->getIndex()] = nodes[i];
    
    for (size_t i = 0; i < numTips; i++)
        nodes[i] = n2[i];
    */
    
    
    //if (nodesByIndex.size() == 0)
    //    fillNodesByIndex();
}

void AdmixtureTree::setAgesFromBrlens(const std::vector<double>& brlens)
{
    recurseSetAgeFromBrlen(root, brlens);
}

void AdmixtureTree::recurseSetAgeFromBrlen(AdmixtureNode* p, const std::vector<double>& brlens)
{
    for (size_t i = 0; i < p->getNumberOfChildren(); i++)
        recurseSetAgeFromBrlen(&p->getChild(i), brlens);
    p->getParent().setAge(brlens[p->getIndex()] - p->getAge());
}

void AdmixtureTree::updateTipOrderByNames(std::vector<std::string> n)
{
    //std::cout << "updateTipOrderByNames()  " << n.size() << "\n";
    
    std::vector<AdmixtureNode*> tmp_nodes = nodes;
    
    for (size_t i = 0; i < n.size(); i++)
    {
        for (size_t j = 0; j < n.size(); j++)
        {
            if (n[i] == nodes[j]->getName())
            {
                AdmixtureNode* p = nodes[i];
                nodes[i] = nodes[j];
                nodes[j] = p;
                nodes[i]->setIndex(i+1);
//                tmp_nodes[j] = nodes[i];
  //              tmp_nodes[j]->setIndex(j+1);
            }
        }
    }
  
//    for (size_t i = 0; i < n.size(); i++)
//        std::cout << "UPDATE " << n[i] << " " << nodes[i]->getName() << "\n";
//        std::cout << "UPDATE " << n[i] << " " << tmp_nodes[i]->getName() << "\n";
    
//    nodes = tmp_nodes;
}

const std::vector<std::string>& AdmixtureTree::getNames(void)
{
    return names;
}

void AdmixtureTree::setNames(const std::vector<std::string>& n)
{
    names = n;
}

void AdmixtureTree::addAdmixtureEdge(AdmixtureNode* p, AdmixtureNode* c, AdmixtureNode* pc, AdmixtureNode* cc, bool enforceNewickRecomp)
{
    
    AdmixtureNode* pp = &pc->getParent();
    AdmixtureNode* cp = &cc->getParent();
    
    // check age
    if (p->getAge() > pp->getAge() || p->getAge() > cp->getAge())
    {
        std::stringstream ss_err;
        ss_err << "Cannot regraft nd.age > nd.pa.age :" << p->getAge() << " > " << pp->getAge() << "\n";
        throw(RbException(ss_err.str()));
    }
    else if (p->getAge() < pc->getAge() || p->getAge() < cc->getAge())
    {
        std::stringstream ss_err;
        ss_err << "Cannot regraft nd.age < nd.ch.age :" << p->getAge() << " > " << pc->getAge() << "\n";
        throw(RbException(ss_err.str()));
    }
    
    // set admixture edge relationship
    p->setAdmixtureChild(c);
    c->setAdmixtureParent(p);
    
    // add admixtureParent
    p->addChild(pc,false);
    pc->setParent(p,false);
    pp->removeChild(pc,false);
    pp->addChild(p,false);
    p->setParent(pp,false);
    p->setOutgroup(pc->isOutgroup());
    
    // add admixtureChild
    c->addChild(cc,false);
    cc->setParent(c,false);
    cp->removeChild(cc,false);
    cp->addChild(c,false);
    c->setParent(cp,false);
    c->setOutgroup(cc->isOutgroup());
    
    if (enforceNewickRecomp)
        root->flagNewickRecomputation();
}

void AdmixtureTree::addAdmixtureEdge(AdmixtureNode* p, AdmixtureNode* c, AdmixtureNode* pc, AdmixtureNode* cc, double t, double w, bool enforceNewickRecomp)
{
    p->setAge(t);
    c->setAge(t);
    c->setWeight(w);
    
    addAdmixtureEdge(p, c, pc, cc, enforceNewickRecomp);
}

void AdmixtureTree::removeAdmixtureEdge(AdmixtureNode* p, bool enforceNewickRecomp)
{
    AdmixtureNode* n = NULL;
    
    if (&p->getAdmixtureParent() != NULL)
        p = &p->getAdmixtureParent();
    
    // remove admixtureParent
    AdmixtureNode* pp = &p->getParent();
    AdmixtureNode* pc = &p->getChild(0);
    
    p->setParent(n);
    p->removeChild(pc, false);
    pc->setParent(pp, false);
    pp->removeChild(p, false);
    pp->addChild(pc, false);

    // remove admixtureChild
    AdmixtureNode* c = &p->getAdmixtureChild();
    AdmixtureNode* cp = &c->getParent();
    AdmixtureNode* cc = &c->getChild(0);
    
    c->setParent(n);
    c->removeChild(cc, false);
    cc->setParent(cp, false);
    cp->removeChild(c, false);
    cp->addChild(cc, false);
    
    if (enforceNewickRecomp)
        root->flagNewickRecomputation();
}

void AdmixtureTree::subtreePruneRegraft(AdmixtureNode* p, AdmixtureNode* old_pc, AdmixtureNode* new_pc, bool enforceNewickRecomp)
{
    
    AdmixtureNode* old_pp = &p->getParent();
    AdmixtureNode* new_pp = &new_pc->getParent();
    
    // check age
    if (p->getAge() > new_pp->getAge())
    {
        std::stringstream ss_err;
        ss_err << "Cannot regraft nd.age > nd.pa.age :" << p->getAge() << " > " << new_pp->getAge() << "\n";
        throw(RbException(ss_err.str()));
    }
    else if (p->getAge() < new_pc->getAge())
    {
        std::stringstream ss_err;
        ss_err << "Cannot regraft nd.age < nd.ch.age :" << p->getAge() << " > " << new_pc->getAge() << "\n";
        throw(RbException(ss_err.str()));
    }

    // old_pc_bro remains at original divergence site
    AdmixtureNode* old_pc_bro = &old_pp->getChild(0);
    if (old_pc_bro == old_pc)
        old_pc_bro = &old_pp->getChild(1);
    
    // prune p and old_pc clade
    old_pp->removeChild(p, false);
    old_pp->addChild(old_pc_bro, false);
    old_pc_bro->setParent(old_pp, false);
    p->removeChild(old_pc_bro, false);
    
    // regraft p and old_pc clade
    new_pp->removeChild(new_pc, false);
    p->setParent(new_pp, false);
    new_pp->addChild(p, false);
    new_pc->setParent(p, false);
    p->addChild(new_pc, false);
    
    // recompute newick str at end
    if (enforceNewickRecomp)
        root->flagNewickRecomputation();
}

void AdmixtureTree::findDescendantTips(std::set<AdmixtureNode*>& s, AdmixtureNode* p)
{
    if (p->getNumberOfChildren() == 0)
    {
        //std::cout << p << "\t" << p->getIndex() << "\n";
        s.insert(p);
    }
    else
    {
        for (size_t i = 0; i < p->getNumberOfChildren(); i++)
            findDescendantTips(s, &p->getChild(i));
    }
}
std::string AdmixtureTree::getAdmixtureEdgeStr(AdmixtureNode* p, AdmixtureNode* c)
{
    std::stringstream ss;
    std::set<AdmixtureNode*> ps,cs;
    findDescendantTips(ps, p);
    findDescendantTips(cs, c);
    std::set<AdmixtureNode*>::iterator its;
    for (its = ps.begin(); its != ps.end(); its++)
    {
        if (its != ps.begin()) ss << ",";
        ss << (*its)->getName();
    }
    ss << " -> ";
    for (its = cs.begin(); its != cs.end(); its++)
    {
        if (its != cs.begin()) ss << ",";
        ss << (*its)->getName();
    }
    ss << "\ta=" << c->getAge();
    ss << "\tw=" << c->getWeight();
    
    return ss.str();
}

void AdmixtureTree::subtreePruneRegraft(AdmixtureNode* p, AdmixtureNode* old_pc, AdmixtureNode* new_pc, double t, bool enforceNewickRecomp)
{
    p->setAge(t);
    subtreePruneRegraft(p, old_pc, new_pc, enforceNewickRecomp);
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const AdmixtureTree& x) {
    
    o << x.getNewickRepresentation();
    
    return o;
}


