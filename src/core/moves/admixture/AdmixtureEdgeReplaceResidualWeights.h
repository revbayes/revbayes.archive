//
//  AdmixtureEdgeReplaceResidualWeights.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/3/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureEdgeReplaceResidualWeights__
#define __rb_mlandis__AdmixtureEdgeReplaceResidualWeights__


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
    
    class AdmixtureEdgeReplaceResidualWeights : public Move {
        
    public:
        AdmixtureEdgeReplaceResidualWeights( StochasticNode<AdmixtureTree> *n, StochasticNode<double>* r, DeterministicNode<std::vector<double> >* res, ConstantNode<int>* dt, bool asa, double weight);                                                          //!<  constructor
        
        // Basic utility functions
        AdmixtureEdgeReplaceResidualWeights*              clone(void) const;                                                                  //!< Clone object
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
        
        void                            findDescendantTips(std::set<AdmixtureNode*>& s, AdmixtureNode* p);
        
        // member variables
        StochasticNode<AdmixtureTree>*  variable;
        StochasticNode<double>*         rate;
        DeterministicNode<std::vector<double> >* residuals;
        ConstantNode<int>*              delayTimer;
        bool                            changed;
        bool                            failed;
        bool                            failedAdd;
        bool                            allowSisterAdmixture;
        int                             delay;
        
        // stored objects to undo proposal
        AdmixtureNode*                  storedAdmixtureParent;
        AdmixtureNode*                  storedAdmixtureChild;
        AdmixtureNode*                  storedAdmixtureParentChild;
        AdmixtureNode*                  storedAdmixtureChildChild;
        AdmixtureNode*                  storedAdmixtureParentParent;
        AdmixtureNode*                  storedAdmixtureChildParent;
        AdmixtureNode*                  newAdmixtureChildChild;
        AdmixtureNode*                  newAdmixtureParentChild;
        AdmixtureNode*                  newAdmixtureChildParent;
        AdmixtureNode*                  newAdmixtureParentParent;
        
        double                          storedAdmixtureAge;
        double                          storedAdmixtureWeight;

        std::vector<double>             storedResiduals;
    };
    
}

#endif /* defined(__rb_mlandis__AdmixtureEdgeReplaceResidualWeights__) */
