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

#include "MoveOld.h"
#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"
#include "Tree.h"

namespace RevBayesCore {
    
    class AdmixtureEdgeAddResidualWeights : public MoveOld {
        
    public:
//        AdmixtureEdgeAddResidualWeights( StochasticNode<AdmixtureTree> *n, StochasticNode<double>* r, DeterministicNode< RbVector<double> >* res, ConstantNode<int>* dt, int me, bool asa, double weight);                                                          //!<  constructor
        AdmixtureEdgeAddResidualWeights( StochasticNode<AdmixtureTree> *n, StochasticNode<double>* r, StochasticNode<int>* ac, DeterministicNode< RbVector<double> >* res, double d, int ag, int me, bool asa, double weight);                                                          //!<  constructor
        
        // Basic utility functions
        AdmixtureEdgeAddResidualWeights*              clone(void) const;                                                                  //!< Clone object
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
        
        // member variables
        StochasticNode<AdmixtureTree>*  variable;
        StochasticNode<double>*         rate;
        StochasticNode<int>*            admixtureCount;
        DeterministicNode< RbVector<double> >* residuals;
        //ConstantNode<int>*              delayTimer;
        unsigned long                   activeGen;
        bool                            changed;
        bool                            failed;
        bool                            allowSisterAdmixture;
        size_t                          delay;
        size_t                          maxEvents;
        size_t                          numEvents;
        double                          delta;
        
        // stored objects to undo proposal
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
