/*
 *  TreeBinaryUnrooted.cpp
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
#include "TreeBinaryUnrooted.h"
#include "VectorString.h"



TreeBinaryUnrooted::TreeBinaryUnrooted(void) : Tree( getMemberRules() ) {
    
}

TreeBinaryUnrooted::TreeBinaryUnrooted(const TreeBinaryUnrooted& ut) : Tree( getMemberRules() ) {
    
}

TreeBinaryUnrooted::TreeBinaryUnrooted(std::vector<std::string> names) : Tree( getMemberRules() ) {
    
    // allocate the nodes
    int nNodes = 2 * getNumberOfTips() - 2;
    
    // randomly link up the nodes
    buildRandomTree(nNodes, GLOBAL_RNG);
}

TreeBinaryUnrooted::TreeBinaryUnrooted(std::string newickStr) : Tree( getMemberRules() ) {
    
}

TreeBinaryUnrooted::TreeBinaryUnrooted(int nTips) : Tree( getMemberRules() ) {
    
}

TreeBinaryUnrooted::~TreeBinaryUnrooted(void) {
    
}

/* Clone this tree */
TreeBinaryUnrooted* TreeBinaryUnrooted::clone(void) const {
    return new TreeBinaryUnrooted(*this);
}

/* Equals comparison */
bool TreeBinaryUnrooted::equals(const RbObject* x) const {
    return x == this;
}

const VectorString& TreeBinaryUnrooted::getClass(void) const {
    
    static VectorString rbClass = VectorString(TreeBinaryUnrooted_name) + Tree::getClass();
    return rbClass;
}

void TreeBinaryUnrooted::printValue(std::ostream& o) const {
    o << newick << std::endl;
}

const MemberRules& TreeBinaryUnrooted::getMemberRules(void) const {
    
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
DAGNode* TreeBinaryUnrooted::executeOperation(const std::string& name, ArgumentFrame& args) {
    
    if (false) 
    {
        return NULL;
    }
    else
        return Tree::executeOperation(name, args);
    
}

/** Do not allow the user to set the index */
void TreeBinaryUnrooted::setValue(const std::string& name, RbObject* val) {
    
    //    if (name == "index")
    //        throw RbException("Cannot change index of a tree node");
    Tree::setValue(name, val);
}

/** Do not allow the user to set the index */
void TreeBinaryUnrooted::setVariable(const std::string& name, DAGNode* var) {
    
    //    if (name == "index")
    //        throw RbException("Cannot change index of a tree node");
    Tree::setVariable(name, var);
}

/** Get method specifications */
const MethodTable& TreeBinaryUnrooted::getMethods(void) const {
    
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

void TreeBinaryUnrooted::buildRandomTree(int nTips, RandomNumberGenerator* rng) {
    
}


