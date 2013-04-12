//
//  AdmixtureEdgeRemoveResidualWeights.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__AdmixtureEdgeRemoveResidualWeights__
#define __revbayes_mlandis__AdmixtureEdgeRemoveResidualWeights__

#include <ostream>
#include <set>
#include <string>

#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "Move.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"
#include "Tree.h"

namespace RevBayesCore {
    
    class AdmixtureEdgeRemoveResidualWeights : public Move {
        
    public:
        AdmixtureEdgeRemoveResidualWeights( StochasticNode<AdmixtureTree> *n, StochasticNode<double>* r, DeterministicNode<std::vector<double> >* res, ConstantNode<int>* dt, double weight);                                            //!<  constructor
        
        // Basic utility functions
        AdmixtureEdgeRemoveResidualWeights*            clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            rejectSimpleMove(void);
        void                            acceptSimpleMove(void);
        
        void                            acceptMove(void);                                                                   //!< Accept the InferenceMoveSimple
        double                          performMove(double& probRatio);                                                     //!< Perform the InferenceMoveSimple
        void                            rejectMove(void);
        
        
    private:
        
        // member variables
        StochasticNode<AdmixtureTree>*  variable;
        StochasticNode<double>*         rate;
        DeterministicNode<std::vector<double> >* residuals;
        ConstantNode<int>*              delayTimer;
        bool changed;
        int delay;
        
        // stored objects to undo proposal
        bool                            failed;
        AdmixtureNode*                  storedAdmixtureChild;
        AdmixtureNode*                  storedAdmixtureParent;
        AdmixtureNode*                  storedAdmixtureChildChild;
        AdmixtureNode*                  storedAdmixtureParentChild;
        AdmixtureNode*                  storedAdmixtureChildParent;
        AdmixtureNode*                  storedAdmixtureParentParent;
        
    };
    
}
#endif /* defined(__revbayes_mlandis__AdmixtureEdgeRemoveResidualWeights__) */
