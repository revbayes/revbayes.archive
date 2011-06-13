/**
 * @file
 * This file contains the declaration of a TreeNode.
 *
 * @brief Declaration of TreeNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "Boolean.h"
#include "ConstantNode.h"
#include "MemberFunction.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbNames.h"
#include "VectorString.h"
#include "TreeNode.h"

#include <algorithm>



/** Default constructor */
TreeNode::TreeNode(void) : MemberObject( getMemberRules() ) {
    
}

/** Clone function */
TreeNode* TreeNode::clone(void) const {
    
    return new TreeNode(*this);
}

/** Pointer-based equals comparison */
bool TreeNode::equals(const RbObject* obj) const {
    
    // Note: This function is on its way out
    return false;
}

/** Map calls to member methods */
DAGNode* TreeNode::executeOperation(const std::string& name, std::vector<DAGNode*>& args) {
    
    if (name == "isTip") 
    {
        if (getNumberOfChildren() == 0)
            return new ConstantNode(new Boolean(true));
        return new ConstantNode(new Boolean(false));
    }
    else if (name == "isRoot") 
    {
        if (getParent() == NULL)
            return new ConstantNode(new Boolean(true));
        return new ConstantNode(new Boolean(false));
    }
    else if (name == "isInterior") 
    {
        if (getParent() != NULL && getNumberOfChildren() != 0)
            return new ConstantNode(new Boolean(true));
        return new ConstantNode(new Boolean(false));
    }
    else
        throw RbException("No member method called '" + name + "'");
    
    return NULL;
}

/** Get class vector describing type of object */
const VectorString& TreeNode::getClass() const {
    
    static VectorString rbClass = VectorString(TreeNode_name) + MemberObject::getClass();
    return rbClass;
}

/** Get method specifications */
const MethodTable& TreeNode::getMethodInits(void) const {
    
    static MethodTable      methodInits;
    static ArgumentRules    isTipRules;
    static bool             initsSet = false;
    
    if (!initsSet) 
    {
        methodInits.addFunction("isTip", new MemberFunction(Boolean_name, ArgumentRules()));
        methodInits.addFunction("isRoot", new MemberFunction(Boolean_name, ArgumentRules()));
        methodInits.addFunction("isInterior", new MemberFunction(Boolean_name, ArgumentRules()));
        
        initsSet = true;
    }
    
    return methodInits;
}

/** Get member variable rules */
const MemberRules& TreeNode::getMemberRules(void) const {
    
    static MemberRules memberRules;
    static bool        rulesSet = false;
    
    if (!rulesSet) 
    {
        //        memberRules.push_back(new ArgumentRule("index", Integer_name));
        //        memberRules.push_back(new ArgumentRule("brlen", RealPos_name));
        //        memberRules.push_back(new ArgumentRule("time", RealPos_name));
        
        rulesSet = true;
    }
    
    return memberRules;
}

/** Print value for user */
void TreeNode::printValue(std::ostream& o) const {
    
}

/** Remove a child from the vector of children */
void TreeNode::removeChild(TreeNode* p) {
    
    std::vector<TreeNode*>::iterator it = find(children.begin(), children.end(), p);
    if ( it != children.end() )
        children.erase(it);
    else 
        throw(RbException("Cannot find node in list of children nodes"));
}

/** Do not allow the user to set the index */
void TreeNode::setValue(const std::string& name, RbObject* val) {
    
    if (name == "index")
        throw RbException("Cannot change index of a tree node");
    MemberObject::setValue(name, val);
}

/** Do not allow the user to set the index */
void TreeNode::setVariable(const std::string& name, DAGNode* var) {
    
    if (name == "index")
        throw RbException("Cannot change index of a tree node");
    MemberObject::setVariable(name, var);
}

/** Complete info about object */
std::string TreeNode::toString(void) const {
    
    std::ostringstream o;
    o <<  "Tree node: value = ";
    printValue(o);
    
    return o.str();
}

