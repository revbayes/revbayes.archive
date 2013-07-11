//
//  AdmixtureSubtreePruneRegraftAndRateShiftAndRateShift.h
//  rb_mlandis
//
//  Created by Michael Landis on 7/7/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureSubtreePruneRegraftAndRateShiftAndRateShift__
#define __rb_mlandis__AdmixtureSubtreePruneRegraftAndRateShiftAndRateShift__


#include <ostream>
#include <set>
#include <string>

#include "CompoundMove.h"
#include "StochasticNode.h"
#include "ContinuousStochasticNode.h"
#include "AdmixtureTree.h"
#include "Tree.h"

namespace RevBayesCore {
    
    class AdmixtureSubtreePruneRegraftAndRateShift : public CompoundMove {
        
    public:
        AdmixtureSubtreePruneRegraftAndRateShift( std::vector<DagNode*> v, int ni, double d, double weight);                                            //!<  constructor
        
        // Basic utility functions
        AdmixtureSubtreePruneRegraftAndRateShift*     clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performCompoundMove(void);                                                            //!< Perform move
        void                            rejectCompoundMove(void);
        void                            acceptCompoundMove(void);
        
    private:
        double                          proposeAge(AdmixtureNode* p);
        
        // member variables
        StochasticNode<AdmixtureTree>*      tree;
        ContinuousStochasticNode*           rate;
        
        // stored objects to undo proposal
        bool failed;
        int nodeIdx;
        double                              storedNodeAge;
        double                              storedRate;
        double                              delta;
        
        AdmixtureNode* storedChild;
        AdmixtureNode* storedParent;
        AdmixtureNode* storedNode;
        AdmixtureNode* newParent;
        AdmixtureNode* newChild;
        
    };
    
}

#endif /* defined(__rb_mlandis__AdmixtureSubtreePruneRegraftAndRateShiftAndRateShift__) */
