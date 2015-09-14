//
//  AdmixtureEdgeDivergenceMerge.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/9/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureEdgeDivergenceMerge__
#define __rb_mlandis__AdmixtureEdgeDivergenceMerge__

#include <ostream>
#include <set>
#include <string>

#include "MoveOld.h"
#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "ContinuousStochasticNode.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"
#include "Tree.h"

namespace RevBayesCore {
    
    class AdmixtureEdgeDivergenceMerge : public MoveOld {
        
    public:
       
        //AdmixtureEdgeDivergenceMerge( StochasticNode<AdmixtureTree> *n, StochasticNode<double>* r, StochasticNode<int>* ac, DeterministicNode< RbVector<double> >* res, int ag, bool asa, double weight);   //!<  constructor
        AdmixtureEdgeDivergenceMerge( StochasticNode<AdmixtureTree> *n, StochasticNode<double>* r, std::vector<ContinuousStochasticNode*> br, StochasticNode<int>* ac, DeterministicNode< RbVector<double> >* res, int ag, bool asa, double weight);   //!<  constructor
        
        
        // Basic utility functions
        AdmixtureEdgeDivergenceMerge*   clone(void) const;                                                                  //!< Clone object
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
        StochasticNode<double>*         admixtureRate;
        StochasticNode<int>*            admixtureCount;
        std::vector<ContinuousStochasticNode*> branchRates;
        DeterministicNode< RbVector<double> >* residuals;
        //ConstantNode<int>*              delayTimer;
        bool                            changed;
        bool                            failed;
        bool                            allowSisterAdmixture;
        int                             delay;
        unsigned long                   activeGen;
        int numEvents;
        
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
        
        AdmixtureNode*                  storedAdmixtureChildGrandparent;
        AdmixtureNode*                  storedAdmixtureChildBrother;
        AdmixtureNode*                  storedAdmixtureChildBrotherChild;
        
        double                          storedAdmixtureAge;
        double                          storedDivergenceAge;
        double                          storedAdmixtureWeight;
        
        std::vector<double>             storedResiduals;
        std::map<int,double>            storedBranchRates;
    };
    
}

#endif /* defined(__rb_mlandis__AdmixtureEdgeDivergenceMerge__) */
