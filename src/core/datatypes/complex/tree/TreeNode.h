/**
 * @file
 * This file contains the declaration of a TreeNode. Tree nodes are member objects and therefore can hold
 * all the variables one might to associate to the tree. The tree nodes are used to create the structure
 * of a tree. They provide access to their parent and children.
 *
 * The usage of tree nodes is to create and give easy access to parts of the dag, namely variables hold
 * by the nodes which need access to ancestral variables.
 *
 * We do not distinguish between branch parameter and node parameters. A branch parameter is simply set as
 * a node parameter of the descending node.
 *
 * A tree node can have a distribution associated with it, for instance in the situation when we condition
 * on a group of taxa being monophyletic.
 *
 *
 * NOTE: This class might be a temporary solution being the unified solution for all tree nodes. In the future
 * we might make this class abstract and implement at least the two types of tree nodes: bifurcating tree nodes
 * which are restricted two have exactly two descendants and multifurcating tree nodes which can have any number
 * of tree nodes. Perhaps there might be also tip nodes as a derived class and a root node, which is the 
 * correct OO design approach but comes with a lot of overhead. A fast conversion method would be needed.
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
 * $Id:$
 */

#ifndef TreeNode_H
#define TreeNode_H

#include "MemberObject.h"
#include <vector>

class TreeNode : public MemberObject {
    
public:
    TreeNode(void);                                                                                             //!< Default constructor
    TreeNode(const std::string& n);                                                                             //!< constructor with name
    
    // Basic utility functions
    TreeNode*                       clone(void) const;                                                          //!< Clone object
    bool                            equals(const RbObject* x) const;                                            //!< Equals comparison
    const VectorString&             getClass(void) const;                                                       //!< Get class vector
    void                            printValue(std::ostream& o) const;                                          //!< Print value for user
    std::string                     toString(void) const;                                                       //!< Complete info
    
    // Member variable functions
    const MemberRules&              getMemberRules(void) const;                                                 //!< Get member rules
    const MethodTable&              getMethodInits(void) const;                                                 //!< Get method specifications
    void                            setValue(const std::string& name, RbObject* val);                           //!< Disallow the user for setting index
    void                            setVariable(const std::string& name, DAGNode* var);                         //!< Disallow the user for setting index
    
    // TreeNode-specific functions
    void                            addChild(TreeNode* p) { children.push_back(p); }                            //!< Adds a child node
    TreeNode*                       getChild(int i) { return children[i]; }                                     //!< Returns the i-th child
    int                             getNumberOfChildren(void) { return int(children.size()); }                  //!< Returns the number of children
    TreeNode*                       getParent(void) { return parent; }                                          //!< Returns the node's parent
    bool                            isTip(void) { return children.size() == 0; }
    bool                            isRoot(void) { return parent == NULL; }
    void                            setParent(TreeNode* p) { parent = p; }                                      //!< Sets the node's parent
    void                            removeAllChildren(void) { children.clear(); }                               //!< Removes all of the children of the node
    void                            removeChild(TreeNode* p);                                                   //!< Removes a specific child
    
protected:
    DAGNode*                        executeOperation(const std::string& name, std::vector<DAGNode*>& args);     //!< Execute method
    
private: 
    std::vector<TreeNode*>          children;                                                                   //!< Vector holding the node's children
    TreeNode*                       parent;                                                                     //!< Pointer to the parent of the node
    std::string                     name;                                                                       //!< name of the node, i.e. identifier/taxon name
};

#endif


