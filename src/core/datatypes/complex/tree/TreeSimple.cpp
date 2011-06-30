/*
 *  TreeSimple.cpp
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
#include "TreeSimple.h"
#include "VectorString.h"



TreeSimple::TreeSimple(void) : Tree( getMemberRules() ) {
    
}

TreeSimple::TreeSimple(const TreeSimple& ut) : Tree( getMemberRules() ) {
    
}

TreeSimple::TreeSimple(TreeNode *r) : Tree( getMemberRules() ) {
    root = r;
    
}

TreeSimple::TreeSimple(std::vector<std::string> names) : Tree( getMemberRules() ) {
    
    // allocate the nodes
    int nNodes = 2 * getNumberOfTips() - 2;
    
    // randomly link up the nodes
    buildRandomTree(nNodes, GLOBAL_RNG);
}

TreeSimple::TreeSimple(std::string newickStr) : Tree( getMemberRules() ) {
    
}

TreeSimple::TreeSimple(int nTips) : Tree( getMemberRules() ) {
    
}

TreeSimple::~TreeSimple(void) {
    
}

/* Clone this tree */
TreeSimple* TreeSimple::clone(void) const {
    return new TreeSimple(*this);
}

/* Equals comparison */
bool TreeSimple::equals(const RbObject* x) const {
    return x == this;
}

const VectorString& TreeSimple::getClass(void) const {
    
    static VectorString rbClass = VectorString(TreeSimple_name) + Tree::getClass();
    return rbClass;
}

void TreeSimple::printValue(std::ostream& o) const {
    o << newick << std::endl;
}

const MemberRules& TreeSimple::getMemberRules(void) const {
    
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
DAGNode* TreeSimple::executeOperation(const std::string& name, ArgumentFrame& args) {
    
    if (false) 
    {
        return NULL;
    }
    else
        return Tree::executeOperation(name, args);
    
}

/** Do not allow the user to set the index */
void TreeSimple::setValue(const std::string& name, RbObject* val) {
    
    //    if (name == "index")
    //        throw RbException("Cannot change index of a tree node");
    Tree::setValue(name, val);
}

/** Do not allow the user to set the index */
void TreeSimple::setVariable(const std::string& name, DAGNode* var) {
    
    //    if (name == "index")
    //        throw RbException("Cannot change index of a tree node");
    Tree::setVariable(name, var);
}

/** Get method specifications */
const MethodTable& TreeSimple::getMethods(void) const {
    
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

void TreeSimple::buildRandomTree(int nTips, RandomNumberGenerator* rng) {
    
}


