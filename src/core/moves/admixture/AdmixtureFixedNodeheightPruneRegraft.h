//
//  AdmixtureFixedNodeheightPruneRegraft.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__AdmixtureFixedNodeheightPruneRegraft__
#define __revbayes_mlandis__AdmixtureFixedNodeheightPruneRegraft__


#include <ostream>
#include <set>
#include <string>

#include "SimpleMove.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"
#include "Tree.h"

namespace RevBayesCore {
    
    class AdmixtureFixedNodeheightPruneRegraft : public SimpleMove {
        
    public:
        AdmixtureFixedNodeheightPruneRegraft( StochasticNode<AdmixtureTree> *n, double weight);                                            //!<  constructor
        
        // Basic utility functions
        AdmixtureFixedNodeheightPruneRegraft*    clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            acceptSimpleMove(void);
        void                            rejectSimpleMove(void);
        
    private:
        void                            findNewBrothers(std::vector<AdmixtureNode*> &b, AdmixtureNode &p, AdmixtureNode *n);
        
        // member variables
        StochasticNode<AdmixtureTree>*       variable;
        
        // stored objects to undo proposal
        bool failed;
        AdmixtureNode*                       storedBrother;
        AdmixtureNode*                       storedNewBrother;
        
    };
    
}


#endif /* defined(__revbayes_mlandis__AdmixtureFixedNodeheightPruneRegraft__) */
