/*
 *  Tree.cpp
 *  revbayes_xcode
 *
 *  Created by John Huelsenbeck on 8/24/10.
 *  Copyright 2010 University of California, Berkeley. All rights reserved.
 *
 */

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "DAGNodePlate.h"
#include "Integer.h"
#include "MemberFunction.h"
#include "RbException.h"
#include "RbNames.h"
#include "Tree.h"
#include "TreeNode.h"
#include "VectorString.h"

/** Default constructor */
Tree::Tree(void) : MemberObject( getMemberRules(), getMethodInits() ) {

}

/** constructor */
Tree::Tree(const MemberRules& memberRules, const MethodTable& methodInits) : MemberObject( memberRules, methodInits ) {

}


void Tree::allocateNodes(int nNodes) {

    for (int i=0; i<nNodes; i++)
        nodes.push_back( new TreeNode );
}

void Tree::deleteNodes(void) {

    for (std::vector<TreeNode*>::iterator it=nodes.begin(); it != nodes.end(); it++)
        delete (*it);
    nodes.clear();
}

const VectorString& Tree::getClass(void) const {
    
    static VectorString rbClass = VectorString(Tree_name) + MemberObject::getClass();
    return rbClass;
}


/** Do not allow the user to set the index */
void Tree::setValue(const std::string& name, RbObject* val) {

    MemberObject::setValue(name, val);
}

/** Do not allow the user to set the index */
void Tree::setVariable(const std::string& name, DAGNode* var) {

    MemberObject::setVariable(name, var);
}

/** Map calls to member methods */
DAGNode* Tree::executeOperation(const std::string& name, std::vector<DAGNode*>& args) {

    if (name == "getNodes") 
        {
        DAGNodePlate* plate = new DAGNodePlate(nodes.size(), TreeNode_name);
        for (size_t i=0; i<nodes.size(); i++) 
            {
            plate[i].setValue(nodes[i]);
            }
        return plate;
        }
    else if (name == "getNumberOfNodes") 
        {
        return new ConstantNode( new Integer((int)nodes.size()));
        }
    else if (name == "getRoot") 
        {
        return new ConstantNode( root );
        }
    else if (name == "getTips") 
        {
        DAGNodePlate* plate = new DAGNodePlate( numberOfTips, TreeNode_name);
        int j = 0;
        for (size_t i=0; i<nodes.size(); i++) 
            {
            if ( nodes[i]->isTip() ) 
                {
                plate[j].setValue(nodes[i]);
                j++;
                }
            }
        return plate;
        }
    else
        throw RbException("No member method called '" + name + "'");

    return NULL;
}

/** Get method specifications */
const MethodTable& Tree::getMethodInits(void) const {

    static MethodTable      methodInits;
    static ArgumentRules    isTipRules;
    static bool             initsSet = false;

    if (!initsSet) 
        {
        
        // add a function returning all nodes of the tree
        methodInits.addFunction("getNodes", new MemberFunction(TreeNode_name, 1, ArgumentRules()));
        
        // add a function returning the number of nodes
        methodInits.addFunction("getNumberOfNodes", new MemberFunction(Boolean_name, ArgumentRules()));
        
        // add a function returning the root of the tree
        methodInits.addFunction("getRoot", new MemberFunction(TreeNode_name, ArgumentRules()));
        
        // add a function returning all the tips
        methodInits.addFunction("getTips", new MemberFunction(Boolean_name, 1, ArgumentRules()));
        
        
//        methodInits.addFunction("getNumberOfNodes", new MemberFunction(Boolean_name, ArgumentRules()));

        initsSet = true;
        }

    return methodInits;
}

const MemberRules& Tree::getMemberRules(void) const {
    
    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) 
		{

        rulesSet = true;
		}

    return memberRules;
}