//
//  AdmixtureNearestNeighborInterchange.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__AdmixtureNearestNeighborInterchange__
#define __revbayes_mlandis__AdmixtureNearestNeighborInterchange__

#include <ostream>
#include <set>
#include <string>

#include "SimpleMove.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"
#include "Tree.h"

namespace RevBayesCore {
    
    class AdmixtureNearestNeighborInterchange : public SimpleMove {
        
    public:
        AdmixtureNearestNeighborInterchange( StochasticNode<AdmixtureTree> *n, double weight);                                            //!<  constructor
        
        // Basic utility functions
        AdmixtureNearestNeighborInterchange*     clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            rejectSimpleMove(void);
        void                            acceptSimpleMove(void);
        
    private:
        
        // member variables
        StochasticNode<AdmixtureTree>*       variable;
        
        // stored objects to undo proposal
        bool failed;
        AdmixtureNode*                       storedChosenNode;
        AdmixtureNode*                       storedUncle;
        double                          storedChosenAge;
        double                          storedUnclesAge;
        
        AdmixtureNode*                      storedChosenNodeParent;
        AdmixtureNode*                      storedUncleParent;
        
    };
    
}

#endif /* defined(__revbayes_mlandis__AdmixtureNearestNeighborInterchange__) */
