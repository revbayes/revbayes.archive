/**
 * @file
 * This file contains the declaration of a Topology a light-weight class that holds the topology of a tree as a set of nodes.
 *
 * @brief Declaration of the Topology
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id$
 */

#ifndef Topology_H
#define Topology_H

#include "ConstantMemberObject.h"
#include <set>
#include <string>

class ArgumentEnvironment;
class ArgumentRule;
class DAGNode;
class MethodTable;
class TopologyNode;
class VectorString;


const std::string Topology_name = "Tree Topology";

class Topology: public ConstantMemberObject {
    
    public:
                                            Topology(void);                                                         //!< Default constructor
                                            Topology(const Topology& t);                                            //!< Copy constructor
                                           ~Topology(void);                                                         //!< Destructor

        // Basic utility functions
        Topology*                           clone(void) const;                                                      //!< Clone object
        const VectorString&                 getClass(void) const;                                                   //!< Get class vector 
        const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object  
        void                                printValue(std::ostream& o) const;                                      //!< Print value for user
        std::string                         richInfo(void) const;                                                   //!< Complete info

        // Member variable rules
        RbPtr<const MemberRules>            getMemberRules(void) const;                                             //!< Get member rules

        // Member method inits
        RbPtr<RbObject>                     executeOperation(const std::string& name, const RbPtr<Environment>& args); //!< Execute method
        RbPtr<const MethodTable>            getMethods(void) const;                                                 //!< Get methods
        
        // Topology functions
        RbPtr<TopologyNode>                 cloneTree(RbPtr<const TopologyNode> parent);                                  //!< Deep copy of the nodes
        bool                                getIsBinary(void) const { return isBinary; }                            //!< Is the tree rooted
        bool                                getIsRooted(void) const { return isRooted; }                            //!< Is the tree rooted
        std::vector<RbPtr<TopologyNode> >   getNodes(void) const { return nodes; }                                  //!< Get a pointer to the nodes in the tree
        size_t                              getNumberOfInteriorNodes(void) const;                                   //!< Get the number of nodes in the tree
        size_t                              getNumberOfNodes(void) const { return nodes.size(); }                   //!< Get the number of nodes in the tree
        size_t                              getNumberOfTips(void) const;                                            //!< Get the number of tip nodes in the tree
        RbPtr<const TopologyNode>           getInteriorNode(int indx) const;                                        //!< Get a pointer to interior node i
        RbPtr<const TopologyNode>           getRoot(void) const;                                                    //!< Get a pointer to the root node of the tree
        RbPtr<const TopologyNode>           getTipNode(size_t indx) const;                                          //!< Get a pointer to tip node i
        void                                setIsRooted(bool tf) { isRooted = tf; }                                 //!< Set the rootedness of the tree
        void                                setRoot(RbPtr<TopologyNode> r);                                         //!< Set the root and bootstrap the tree from it
        
private:
        static const TypeSpec               typeSpec;
        void                                fillNodesByPreorderTraversal(RbPtr<TopologyNode> node);                 //!< fill the nodes vector by a preorder traversal recursively starting with this node.
    
        std::vector<RbPtr<TopologyNode> >   nodes;                                                                  //!< Vector of pointers to all nodes
        RbPtr<TopologyNode>                 root;                                                                   //!< Pointer to the root node
        bool                                isRooted;                                                               //!< Is the topology rooted?
        bool                                isBinary;                                                               //!< Is the topology binary?
};

#endif

