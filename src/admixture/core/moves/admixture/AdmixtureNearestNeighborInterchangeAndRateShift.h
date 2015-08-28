//
//  AdmixtureNearestNeighborInterchangeAndRateShift.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/12/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureNearestNeighborInterchangeAndRateShift__
#define __rb_mlandis__AdmixtureNearestNeighborInterchangeAndRateShift__


#include <ostream>
#include <set>
#include <string>

#include "MoveOld.h"
#include "ContinuousStochasticNode.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"

namespace RevBayesCore {
    
    class AdmixtureNearestNeighborInterchangeAndRateShift : public MoveOld {
        
    public:
        AdmixtureNearestNeighborInterchangeAndRateShift( StochasticNode<AdmixtureTree> *n, std::vector<ContinuousStochasticNode*> br, double d, bool tuning, double weight);                                                         //!<  constructor
        virtual ~AdmixtureNearestNeighborInterchangeAndRateShift(void);
        
        // Basic utility functions
        AdmixtureNearestNeighborInterchangeAndRateShift*  clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        
        void                            acceptMove(void);                                                                   //!< Accept the InferenceMoveSimple
        double                          performMove(double& probRatio);                                                     //!< Perform the InferenceMoveSimple
        void                            rejectMove(void);
        
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            rejectSimpleMove(void);
        void                            acceptSimpleMove(void);
        void                            tune(void);
        
    private:
        
        // member variables
        StochasticNode<AdmixtureTree>*  variable;
        std::vector<ContinuousStochasticNode*> branchRates;
        
        // stored objects to undo proposal
        bool                            failed;
        bool                            changed;
        
        AdmixtureNode*                  storedNode;
        AdmixtureNode*                  storedBrother;
        AdmixtureNode*                  storedBrotherParent;
        AdmixtureNode*                  storedNodeParent;
        AdmixtureNode*                  storedNodeChildFix;
        AdmixtureNode*                  storedNodeChildMove;
        AdmixtureNode*                  newNodeParent;
        AdmixtureNode*                  newNodeChild;

        double                          storedAge;
        double                          storedBroAge;
        
        int                             storedChildRateIndex;
        int                             storedNodeRateIndex;
        int                             storedBrotherRateIndex;
        double                          storedChildRate;
        double                          storedNodeRate;
        double                          storedBrotherRate;
        
        double                          delta;
    };
}


#endif /* defined(__rb_mlandis__AdmixtureNearestNeighborInterchangeAndRateShift__) */
