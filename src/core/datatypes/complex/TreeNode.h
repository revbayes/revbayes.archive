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

#ifndef TreeNode_H
#define TreeNode_H

#include "MemberObject.h"
#include <vector>

class TreeNode : public MemberObject {
    
public:
    TreeNode(void);                                                          //!< Default constructor
    
    // Basic utility functions
    TreeNode*                     clone(void) const;                                                       //!< Clone object
    bool                          equals(const RbObject* x) const;                                         //!< Equals comparison
    const VectorString&           getClass(void) const;                                                    //!< Get class vector
    void                          printValue(std::ostream& o) const;                                       //!< Print value for user
    std::string                   toString(void) const;                                                    //!< Complete info
    
    // Member variable functions
    const MemberRules&            getMemberRules(void) const;                                              //!< Get member rules
    const MethodTable&            getMethodInits(void) const;                                              //!< Get method specifications
    void                          setValue(const std::string& name, RbObject* val);                        //!< Disallow the user for setting index
    void                          setVariable(const std::string& name, DAGNode* var);                      //!< Disallow the user for setting index
    
    // TreeNode-specific functions
    void                          addChild(TreeNode* p) { children.push_back(p); }                         //!< Adds a child node
    TreeNode*                     getChild(int i) { return children[i]; }                                  //!< Returns the i-th child
    int                           getNumberOfChildren(void) { return int(children.size()); }                    //!< Returns the number of children
    TreeNode*                     getParent(void) { return parent; }                                       //!< Returns the node's parent
    bool                          isTip(void) { return children.size() == 0; }
    bool                          isRoot(void) { return parent == NULL; }
    void                          setParent(TreeNode* p) { parent = p; }                                   //!< Sets the node's parent
    void                          removeAllChildren(void) { children.clear(); }                            //!< Removes all of the children of the node
    void                          removeChild(TreeNode* p);                                                //!< Removes a specific child
    
protected:
    DAGNode*                      executeOperation(const std::string& name, std::vector<DAGNode*>& args);  //!< Execute method
    
private: 
    std::vector<TreeNode*>        children;                                                                //!< Vector holding the node's children
    TreeNode*                     parent;                                                                  //!< Pointer to the parent of the node
};

#endif


