//
//  AdmixtureEdgeAddResidualWeights.h
//  rb_mlandis
//
//  Created by Michael Landis on 2/28/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureEdgeAddResidualWeights__
#define __rb_mlandis__AdmixtureEdgeAddResidualWeights__

#include <ostream>
#include <set>
#include <string>

#include "Move.h"
#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"
#include "Tree.h"

namespace RevBayesCore {
    
    class AdmixtureEdgeAddResidualWeights : public Move {
        
    public:
        AdmixtureEdgeAddResidualWeights( StochasticNode<AdmixtureTree> *n, StochasticNode<double>* r, DeterministicNode<std::vector<double> >* res, ConstantNode<int>* dt, double weight);                                                          //!<  constructor
        
        // Basic utility functions
        AdmixtureEdgeAddResidualWeights*              clone(void) const;                                                                  //!< Clone object
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
        
        void                            findNewBrothers(std::vector<AdmixtureNode*> &b, AdmixtureNode &p, AdmixtureNode *n);
        
        // member variables
        StochasticNode<AdmixtureTree>*  variable;
        StochasticNode<double>*         rate;
        DeterministicNode<std::vector<double> >* residuals;
        ConstantNode<int>*              delayTimer;
        bool                            changed;
        bool                            failed;
        int                             delay;
        
        // stored objects to undo proposal
        double                          storedAdmixtureAge;
        AdmixtureNode*                  storedAdmixtureParent;
        AdmixtureNode*                  storedAdmixtureChild;
        AdmixtureNode*                  storedAdmixtureParentChild;
        AdmixtureNode*                  storedAdmixtureChildChild;
        AdmixtureNode*                  storedAdmixtureParentParent;
        AdmixtureNode*                  storedAdmixtureChildParent;
        std::vector<double>             storedResiduals;
    };
    
}


#endif /* defined(__rb_mlandis__AdmixtureEdgeAddResidualWeights__) */
