//
//  AdmixtureEdgeAdd.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__AdmixtureEdgeAdd__
#define __revbayes_mlandis__AdmixtureEdgeAdd__

#include <ostream>
#include <set>
#include <string>

#include "MoveOld.h"
#include "ConstantNode.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"
#include "Tree.h"

namespace RevBayesCore {
    
    class AdmixtureEdgeAdd : public MoveOld {
        
    public:
        AdmixtureEdgeAdd( StochasticNode<AdmixtureTree> *n, StochasticNode<double>* r, ConstantNode<int>* dt, double weight);                                            //!<  constructor
        
        // Basic utility functions
        AdmixtureEdgeAdd*              clone(void) const;                                                                  //!< Clone object
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
        
    };
    
}

#endif /* defined(__revbayes_mlandis__AdmixtureEdgeAdd__) */
