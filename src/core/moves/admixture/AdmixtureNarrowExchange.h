//
//  AdmixtureNarrowExchange.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__AdmixtureNarrowExchange__
#define __revbayes_mlandis__AdmixtureNarrowExchange__

#include <ostream>
#include <set>
#include <string>

#include "SimpleMove.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"
#include "Tree.h"

namespace RevBayesCore {
    
    class AdmixtureNarrowExchange : public SimpleMove {
        
    public:
        AdmixtureNarrowExchange( StochasticNode<AdmixtureTree> *n, double d, double weight);                                            //!<  constructor
        
        // Basic utility functions
        AdmixtureNarrowExchange*     clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            rejectSimpleMove(void);
        void                            acceptSimpleMove(void);
        
    private:
        double                          proposeAge(AdmixtureNode* p);
        
        // member variables
        StochasticNode<AdmixtureTree>*       variable;
        
        // stored objects to undo proposal
        bool failed;
        AdmixtureNode*                       storedChosenNode;
        AdmixtureNode*                       storedUncle;
        double                              storedParentAge;
        double                              storedGrandparentAge;
        double                              delta;
        
        AdmixtureNode*                      storedChosenNodeParent;
        AdmixtureNode*                      storedUncleParent;
        AdmixtureNode*                      storedParent;
        AdmixtureNode*                      storedGrandparent;
        
    };
    
}

#endif /* defined(__revbayes_mlandis__AdmixtureNarrowExchange__) */
