/*
 *  TreeUnrooted.cpp
 *  revbayes_xcode
 *
 *  Created by John Huelsenbeck on 8/24/10.
 *  Copyright 2010 University of California, Berkeley. All rights reserved.
 *
 */

#include "MemberFunction.h"
#include "RandomNumberFactory.h"
#include "RbNames.h"
#include "ReferenceRule.h"
#include "Tree.h"
#include "TreeUnrooted.h"
#include "VectorString.h"



TreeUnrooted::TreeUnrooted(void) : Tree( getMemberRules() ) {
    
}

TreeUnrooted::TreeUnrooted(const TreeUnrooted& ut) : Tree( getMemberRules() ) {
    
}

TreeUnrooted::TreeUnrooted(std::vector<std::string> names) : Tree( getMemberRules() ) {
    
    // initialize the number of tips in the tree
    setNumberOfTips( int(names.size()) );
    
    // allocate the nodes
    int nNodes = 2 * getNumberOfTips() - 2;
    allocateNodes(nNodes);
    
    // randomly link up the nodes
    buildRandomTree(nNodes, GLOBAL_RNG);
}

TreeUnrooted::TreeUnrooted(std::string newickStr) : Tree( getMemberRules() ) {
    
}

TreeUnrooted::TreeUnrooted(int nTips) : Tree( getMemberRules() ) {
    
}

TreeUnrooted::~TreeUnrooted(void) {
    
    deleteNodes();
}

/* Clone this tree */
TreeUnrooted* TreeUnrooted::clone(void) const {
    return new TreeUnrooted(*this);
}

/* Equals comparison */
bool TreeUnrooted::equals(const RbObject* x) const {
    return x == this;
}

const VectorString& TreeUnrooted::getClass(void) const {
    
    static VectorString rbClass = VectorString(TreeUnrooted_name) + Tree::getClass();
    return rbClass;
}

void TreeUnrooted::printValue(std::ostream& o) const {
    o << newick << std::endl;
}

const MemberRules& TreeUnrooted::getMemberRules(void) const {
    
    static MemberRules memberRules;
    static bool        rulesSet = false;
    
    if (!rulesSet) 
    {
        memberRules.push_back(new ReferenceRule("brlen", RealPos_name));
        //memberRules.push_back(new ArgumentRule("time", RealPos_name));
        
        
        /* Inherit members from Tree, put it at back */
        const MemberRules& inheritedRules = Tree::getMemberRules();
        memberRules.insert(memberRules.end(), inheritedRules.begin(), inheritedRules.end()); 
        
        rulesSet = true;
    }
    
    return memberRules;
}

/** Map calls to member methods */
DAGNode* TreeUnrooted::executeOperation(const std::string& name, ArgumentFrame& args) {
    
    if (false) 
    {
        return NULL;
    }
    else
        return Tree::executeOperation(name, args);
    
}

/** Do not allow the user to set the index */
void TreeUnrooted::setValue(const std::string& name, RbObject* val) {
    
    //    if (name == "index")
    //        throw RbException("Cannot change index of a tree node");
    Tree::setValue(name, val);
}

/** Do not allow the user to set the index */
void TreeUnrooted::setVariable(const std::string& name, DAGNode* var) {
    
    //    if (name == "index")
    //        throw RbException("Cannot change index of a tree node");
    Tree::setVariable(name, var);
}

/** Get method specifications */
const MethodTable& TreeUnrooted::getMethods(void) const {
    
    static MethodTable      methodInits;
    static ArgumentRules    isTipRules;
    static bool             initsSet = false;
    
    if (!initsSet) 
    {
        const MethodTable& inheritedMethods = Tree::getMethods();
        methodInits = MethodTable(inheritedMethods);
        
//        methodInits.insert(methodInits.end(), inheritedMethods.begin(), inheritedMethods.end()); 
        
        initsSet = true;
    }
    
    return methodInits;
}

void TreeUnrooted::buildRandomTree(int nTips, RandomNumberGenerator* rng) {
    
}


