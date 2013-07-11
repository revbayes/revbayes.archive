//
//  AdmixtureSubtreePruneRegraft.h
//  rb_mlandis
//
//  Created by Michael Landis on 7/7/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureSubtreePruneRegraft__
#define __rb_mlandis__AdmixtureSubtreePruneRegraft__


#include <ostream>
#include <set>
#include <string>

#include "SimpleMove.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"
#include "Tree.h"

namespace RevBayesCore {
    
    class AdmixtureSubtreePruneRegraft : public SimpleMove {
        
    public:
        AdmixtureSubtreePruneRegraft( StochasticNode<AdmixtureTree> *n, double d, double weight);                                            //!<  constructor
        
        // Basic utility functions
        AdmixtureSubtreePruneRegraft*     clone(void) const;                                                                  //!< Clone object
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
        double                              storedNodeAge;
        double                              delta;
        
        AdmixtureNode* storedChild;
        AdmixtureNode* storedParent;
        AdmixtureNode* storedNode;
        AdmixtureNode* newParent;
        AdmixtureNode* newChild;
        
    };
    
}

#endif /* defined(__rb_mlandis__AdmixtureSubtreePruneRegraft__) */
