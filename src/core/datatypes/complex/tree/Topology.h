/**
 * @file
 * This file contains the declaration of a Topology a light-weight class that holds the topology of a tree as a set of nodes.
 *
 * @brief Declaration of the Topology
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id$
 */

#ifndef Topology_H
#define Topology_H

#include <set>
#include <string>

class TopologyNode;


class Topology {
    
    public:
                                                    Topology(void);                                                         //!< Default constructor
                                                    Topology(const Topology& t);                                            //!< Copy constructor
                                                   ~Topology(void);                                                         //!< Destructor

        Topology&                                   operator=(const Topology& t);
    
        // Basic utility functions
        Topology*                                   clone(void) const;                                                      //!< Clone object
        
        // Topology functions
        bool                                        getIsBinary(void) const;                            //!< Is the tree rooted
        bool                                        getIsRooted(void) const;                            //!< Is the tree rooted
        const std::vector<const TopologyNode*>&     getNodes(void) const;                                  //!< Get a pointer to the nodes in the tree
        size_t                                      getNumberOfInteriorNodes(void) const;                                   //!< Get the number of nodes in the tree
        size_t                                      getNumberOfNodes(void) const;                   //!< Get the number of nodes in the tree
        size_t                                      getNumberOfTips(void) const;                                            //!< Get the number of tip nodes in the tree
        const TopologyNode&                         getInteriorNode(int indx) const;                                        //!< Get a pointer to interior node i
        const TopologyNode&                         getRoot(void) const;                                                    //!< Get a pointer to the root node of the tree
        const TopologyNode&                         getTipNode(size_t indx) const;                                          //!< Get a pointer to tip node i
        void                                        setIsRooted(bool tf) { isRooted = tf; }                                 //!< Set the rootedness of the tree
        void                                        setRoot(TopologyNode* r);                                               //!< Set the root and bootstrap the tree from it
    
    private:
        void                                        fillNodesByPreorderTraversal(const TopologyNode* node);                 //!< fill the nodes vector by a preorder traversal recursively starting with this node.
        void                                        fillNodesByPhylogeneticTraversal(const TopologyNode* node);             //!< fill the nodes vector by a preorder traversal recursively starting with this node.
    
        std::vector<const TopologyNode*>            nodes;                                                                  //!< Vector of pointers to all nodes
        TopologyNode*                               root;                                                                   //!< Pointer to the root node
        bool                                        isRooted;                                                               //!< Is the topology rooted?
        bool                                        isBinary;                                                               //!< Is the topology binary?
        size_t                                      numTips;
        size_t                                      numNodes;
};

#endif

