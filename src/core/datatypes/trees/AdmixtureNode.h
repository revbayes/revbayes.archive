//
//  AdmixtureNode.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/19/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__AdmixtureNode__
#define __revbayes_mlandis__AdmixtureNode__

#include "TopologyNode.h"

#include <vector>

namespace RevBayesCore {
    
    class AdmixtureTree;
    
    class AdmixtureNode : public TopologyNode  {
        
    public:
        AdmixtureNode(int indx=0);                                                       //!< Default constructor with optional index
        AdmixtureNode(const std::string& n, int indx=0);                                 //!< Constructor with name and optional index
        AdmixtureNode(const AdmixtureNode &n);                                                //!< Copy constructor
        virtual                            ~AdmixtureNode(void);                                                        //!< Destructor
        
        AdmixtureNode&                      operator=(const AdmixtureNode& n);
        
        // Basic utility functions
        AdmixtureNode*                      clone(void) const;                                                          //!< Clone object
        bool                                equals(const TopologyNode& node) const;                                     //!< Test whether this is the same node
        bool                                equals(const AdmixtureNode& node) const;                                    //!< Test whether this is the same node
        
        // AdmixtureNode functions
        
        void                                flagNewickRecomputation(void);                                              //!< Flag the newick RlString for recomputation
        
        double                              getAge(void) const;                                                         //!< Get the age (time ago from present) for this node
        double                              getBranchLength(void) const;                                                //!< Get the branch length leading towards this node
        const AdmixtureNode&                getChild(size_t i) const;                                                   //!< Returns the i-th child
        AdmixtureNode&                      getChild(size_t i);                                                         //!< Returns the i-th child (non-const to return non-const node)
        std::vector<int>                    getChildrenIndices(void) const;                                             //!< Return children indices
        size_t                              getNumberOfChildren(void) const;                                            //!< Returns the number of children
        AdmixtureNode&                      getParent(void);                                                            //!< Returns the node's parent
        const AdmixtureNode&                getParent(void) const;                                                      //!< Returns the node's parent
        double                              getTime(void) const;                                                        //!< Get the time of the node
        
        void                                addChild(TopologyNode* c);                                                  //!< Adds a child node
        void                                addChild(AdmixtureNode* c);                                                 //!< Adds a child node
        void                                setChild(TopologyNode* c, size_t idx);
        void                                setChild(AdmixtureNode* c, size_t idx);
        void                                removeAllChildren(void);                                                    //!< Removes all of the children of the node
        void                                removeChild(TopologyNode* p);                                               //!< Removes a specific child
        void                                removeChild(AdmixtureNode* p);                                              //!< Removes a specific child
        
        void                                setBranchLength(double a);
        void                                setAge(double a);                                                           //!< Set the age (time ago from present) for this node
        void                                setParent(TopologyNode* p);                                                 //!< Sets the node's parent
        void                                setParent(AdmixtureNode* p);                                                //!< Sets the node's parent
        void                                setTopology(AdmixtureTree *t);
        
        AdmixtureNode&                      getAdmixtureParent(void);
        const AdmixtureNode&                getAdmixtureParent(void) const;
        AdmixtureNode&                      getAdmixtureChild(void);
        const AdmixtureNode&                getAdmixtureChild(void) const;
        double                              getWeight(void);
        void                                setAdmixtureParent(AdmixtureNode* p);
        void                                setAdmixtureChild(AdmixtureNode* p);
        void                                setWeight(double w);
        
        AdmixtureNode&                      getTopologyParent(void);
        const AdmixtureNode&                getTopologyParent(void) const;
        AdmixtureNode&                      getTopologyChild(size_t i);
        const AdmixtureNode&                getTopologyChild(size_t i) const;
        void                                addTopologyChild(TopologyNode* c);
        void                                addTopologyChild(AdmixtureNode* c);
        void                                removeAllTopologyChildren(void);
        void                                removeTopologyChild(TopologyNode* p);
        void                                removeTopologyChild(AdmixtureNode* p);
        void                                setTopologyParent(TopologyNode* p);
        void                                setTopologyParent(AdmixtureNode* p);
        bool                                checkChildren(void);
        bool                                checkParent(void);

        
        bool                                isAdmixtureNode(void);
        bool                                isInteriorNode(void);
        bool operator<( const AdmixtureNode*& other) const;
        
    private:
        std::vector<AdmixtureNode*>         children;                                                                   //!< Vector holding the node's children.
        AdmixtureNode*                      parent;                                                                     //!< Pointer to the parent of the node.
        AdmixtureTree*                      topology;                                                                   //!< A pointer to the tree for convinience access
//        mutable double                      age;
//        mutable double                      time;
//        mutable double                      branchLength;
        
        bool                                sorted;
        bool                                admixtureNode;
        AdmixtureNode*                      admixtureParent;
        AdmixtureNode*                      admixtureChild;
        double                              weight;
    };
    
}

#endif /* defined(__revbayes_mlandis__AdmixtureNode__) */
