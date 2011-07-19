/**
 * @file
 * This file contains the declaration of a TopologyNode. Tree nodes are member objects and therefore can hold
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
 * $Id$
 */

#ifndef TopologyNode_H
#define TopologyNode_H

#include "MemberObject.h"
#include <vector>



class TopologyNode : public MemberObject {
    
    public:
                                        TopologyNode(int indx=0);                                                   //!< Default constructor with optional index
                                        TopologyNode(const std::string& n, int indx=0);                             //!< Constructor with name and optional index
        // Basic utility functions
        TopologyNode*                   clone(void) const;                                                          //!< Clone object
        const VectorString&             getClass(void) const;                                                       //!< Get class vector
        void                            printValue(std::ostream& o) const;                                          //!< Print value for user
        std::string                     richInfo(void) const;                                                       //!< Complete info

        // Member variable rules
        const MemberRules&              getMemberRules(void) const;                                                 //!< Get member rules

        // Member method inits
        DAGNode*                        executeOperation(const std::string& name, ArgumentFrame& args);             //!< Execute method
        const MethodTable&              getMethods(void) const;                                                     //!< Get methods
        
        // TopologyNode functions
        void                            addChild(TopologyNode* p) { children.push_back(p); }                        //!< Adds a child node
        TopologyNode*                   getChild(int i) const { return children[i]; }                               //!< Returns the i-th child
        std::vector<int>                getChildrenIndices(void) const;                                             //!< Return children indices
        int                             getIndex(void) const { return index; }                                      //!< Get index of node
        std::string                     getName(void) const { return name; }                                        //!< Get name of node
        size_t                          getNumberOfChildren(void) const { return children.size(); }                 //!< Returns the number of children
        TopologyNode*                   getParent(void) const { return parent; }                                    //!< Returns the node's parent
        int                             getParentIndex(void) const { return parent->getIndex(); }                   //!< Return parent index
        bool                            isTip(void) { return children.size() == 0; }                                //!< Is node tip?
        bool                            isRoot(void) { return parent == NULL; }                                     //!< Is node root?
        void                            setParent(TopologyNode* p) { parent = p; }                                  //!< Sets the node's parent
        void                            removeAllChildren(void) { children.clear(); }                               //!< Removes all of the children of the node
        void                            removeChild(TopologyNode* p);                                               //!< Removes a specific child
        
    private: 
        std::vector<TopologyNode*>      children;                                                                   //!< Vector holding the node's children
        TopologyNode*                   parent;                                                                     //!< Pointer to the parent of the node
        std::string                     name;                                                                       //!< Name of the node, i.e. identifier/taxon name
        int                             index;                                                                      //!< Node index
};

#endif
