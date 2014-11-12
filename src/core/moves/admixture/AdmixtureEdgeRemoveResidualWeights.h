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
#include "MoveOld.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"
#include "Tree.h"

namespace RevBayesCore {
    
    class AdmixtureEdgeRemoveResidualWeights : public MoveOld {
        
    public:
        //AdmixtureEdgeRemoveResidualWeights( StochasticNode<AdmixtureTree> *n, StochasticNode<double>* r, DeterministicNode< RbVector<double> >* res, ConstantNode<int>* dt, double weight);                                            //!<  constructor
        AdmixtureEdgeRemoveResidualWeights( StochasticNode<AdmixtureTree> *n, StochasticNode<double>* r, StochasticNode<int>* ac, DeterministicNode< RbVector<double> >* res, double d, int ag, double weight);                                            //!<  constructor
        
        
        // Basic utility functions
        AdmixtureEdgeRemoveResidualWeights*            clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        bool                            isActive(unsigned long g) const;
        
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
        StochasticNode<int>*            admixtureCount;
        DeterministicNode< RbVector<double> >* residuals;
        //ConstantNode<int>*              delayTimer;
        bool changed;
        bool failed;
        unsigned long activeGen;
        int numEvents;
        double delta;
        
        // stored objects to undo proposal
      
        AdmixtureNode*                  storedAdmixtureChild;
        AdmixtureNode*                  storedAdmixtureParent;
        AdmixtureNode*                  storedAdmixtureChildChild;
        AdmixtureNode*                  storedAdmixtureParentChild;
        AdmixtureNode*                  storedAdmixtureChildParent;
        AdmixtureNode*                  storedAdmixtureParentParent;
        std::vector<double>             storedResiduals;
        
    };
    
}
#endif /* defined(__revbayes_mlandis__AdmixtureEdgeRemoveResidualWeights__) */
