/**
 * @file
 * This file contains the declaration of a tree topology, which is a very lightweight container of topology nodes.
 *
 * @brief Declaration of the Topology
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2013-03-04, version 1.0
 *
 * $Id: Tree.h 1651 2012-07-05 14:47:08Z hoehna $
 */

#ifndef Topology_H
#define Topology_H

#include <set>
#include <string>
#include <vector>
#include "Taxon.h"
#include "Tree.h"

namespace RevBayesCore {
    
    class TopologyNode;
    
    class Topology {
        
    public:       
                                                            Topology(void);                                    //!< Default constructor required by revlanguage use
                                                            Topology(const Topology &t);
                                                            Topology(TopologyNode* Root, std::vector<TopologyNode*> Nodes); //SK

        virtual                                            ~Topology(void);                                                                                                        //!< Destructor
          
        Topology&                                           operator=(const Topology &t);
        // virtual asic utility functions
        virtual Topology*                                   clone(void) const;                                                      //!< Clone object
        
        
        void                                                addTree(Tree *t);                                                       //!< Add a tree user
        std::vector<std::string>                            getTipNames() const;
        std::vector<Taxon>                                  getTaxa() const;                                                        //!< Get all the taxa in the tree
        std::vector<std::string>                            getSpeciesNames() const;                                                     //!< Get all the species represented in the tree
        TopologyNode&                                       getNode(size_t idx);                                                    //!< Get the node at index
        const TopologyNode&                                 getNode(size_t idx) const;                                              //!< Get the node at index
        std::vector<TopologyNode*>                          getNodes(void) const;                                                   //!< Get a pointer to the nodes in the BranchLengthTree
//        TopologyNode&                                       getNodeByIndex(size_t idt);                                           //!< Get the node with member index == idx
//        const TopologyNode&                                 getNodeByIndex(size_t idt) const;                                     //!< Get the node with member index == idx
//        std::vector<TopologyNode*>                          getNodesByIndex(void) const;                                            //!< Get a vector of nodes ordered by index
        size_t                                              getNumberOfInteriorNodes(void) const;                                   //!< Get the number of nodes in the BranchLengthTree
        size_t                                              getNumberOfNodes(void) const;                                           //!< Get the number of nodes in the BranchLengthTree
        size_t                                              getNumberOfTips(void) const;                                            //!< Get the number of tip nodes in the BranchLengthTree
        const TopologyNode&                                 getInteriorNode(size_t indx) const;                                     //!< Get a pointer to interior node i
        std::string                                         getPlainNewickRepresentation() const;                                   //!< Get the newick representation of this BranchLengthTree
        const std::string&                                  getNewickRepresentation() const;                                        //!< Get the newick representation of this BranchLengthTree
        TopologyNode&                                       getRoot(void);                                                          //!< Get a pointer to the root node of the BranchLengthTree
        const TopologyNode&                                 getRoot(void) const;                                                    //!< Get a pointer to the root node of the BranchLengthTree
        TopologyNode&                                       getTipNode(size_t indx);                                                //!< Get a pointer to tip node i
        const TopologyNode&                                 getTipNode(size_t indx) const;                                          //!< Get a pointer to tip node i
        bool                                                isBinary(void) const;                                                   //!< Is the BranchLengthTree rooted
        bool                                                isRooted(void) const;                                                   //!< Get a pointer to the root node of the Tree
        void                                                removeTree(Tree *t);                                                    //!< Remove a tree user
        void                                                setRoot(TopologyNode* r);                                               //!< Set the root and bootstrap the Tree from it
		void                                                setRootNoReIndexing(TopologyNode* r);
        void                                                setRooted(bool tf);
        void                                                orderNodesByIndex();
		void                                                reindexNodes();
		
    private:
//        void                                                fillNodesByPreorderTraversal(TopologyNode* node);                   //!< fill the nodes vector by a preorder traversal recursively starting with this node.
        void                                                fillNodesByPhylogeneticTraversal(TopologyNode* node);               //!< fill the nodes vector by a preorder traversal recursively starting with this node.
//        void                                                fillNodesByIndex(void);

        // private members
        TopologyNode*                                       root;
        std::vector<TopologyNode*>                          nodes;                                                                  //!< Vector of pointers to all nodes
//        std::vector<TopologyNode*>                          nodesByIndex;                                                           //!< Vector of pointers to all nodes
        bool                                                binary;                                                                 //!< Is the BranchLengthTree binary?
        bool                                                rooted;
        size_t                                              numTips;
        size_t                                              numNodes;
        std::set<Tree*>                                     treesUsingThisTopology;                                                 //!< Trees using us
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const Topology& x);                                         //!< Overloaded output operator
    
}

#endif

