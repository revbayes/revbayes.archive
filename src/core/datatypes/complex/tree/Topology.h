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

        Topology&                                   operator=(const Topology& t);
    
        // Basic utility functions
        Topology*                                   clone(void) const;                                                      //!< Clone object
        const VectorString&                         getClass(void) const;                                                   //!< Get class vector 
        const TypeSpec&                             getTypeSpec(void) const;                                                //!< Get language type of the object  
        void                                        printValue(std::ostream& o) const;                                      //!< Print value for user

        // Member variable rules
        const MemberRules&                          getMemberRules(void) const;                                             //!< Get member rules

        // Member method inits
        const MethodTable&                          getMethods(void) const;                                                 //!< Get methods
        
        // Topology functions
        bool                                        getIsBinary(void) const { return isBinary; }                            //!< Is the tree rooted
        bool                                        getIsRooted(void) const { return isRooted; }                            //!< Is the tree rooted
        const std::vector<const TopologyNode*>&     getNodes(void) const { return nodes; }                                  //!< Get a pointer to the nodes in the tree
        size_t                                      getNumberOfInteriorNodes(void) const;                                   //!< Get the number of nodes in the tree
        size_t                                      getNumberOfNodes(void) const { return nodes.size(); }                   //!< Get the number of nodes in the tree
        size_t                                      getNumberOfTips(void) const;                                            //!< Get the number of tip nodes in the tree
        const TopologyNode&                         getInteriorNode(int indx) const;                                        //!< Get a pointer to interior node i
        const TopologyNode&                         getRoot(void) const;                                                    //!< Get a pointer to the root node of the tree
        const TopologyNode&                         getTipNode(size_t indx) const;                                          //!< Get a pointer to tip node i
        void                                        setIsRooted(bool tf) { isRooted = tf; }                                 //!< Set the rootedness of the tree
        void                                        setRoot(TopologyNode* r);                                               //!< Set the root and bootstrap the tree from it

    protected:
        RbLanguageObject*                           executeOperationSimple(const std::string& name, Environment& args);     //!< Execute method
    
    private:
        static const TypeSpec                       typeSpec;
        void                                        fillNodesByPreorderTraversal(const TopologyNode* node);                 //!< fill the nodes vector by a preorder traversal recursively starting with this node.
        void                                        fillNodesByPhylogeneticTraversal(const TopologyNode* node);             //!< fill the nodes vector by a preorder traversal recursively starting with this node.
    
        std::vector<const TopologyNode*>            nodes;                                                                  //!< Vector of pointers to all nodes
        TopologyNode*                               root;                                                                   //!< Pointer to the root node
        bool                                        isRooted;                                                               //!< Is the topology rooted?
        bool                                        isBinary;                                                               //!< Is the topology binary?
};

#endif

