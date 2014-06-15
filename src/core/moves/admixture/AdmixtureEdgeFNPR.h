//
//  AdmixtureEdgeFNPR.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/27/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureEdgeFNPR__
#define __rb_mlandis__AdmixtureEdgeFNPR__

#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>

#include "ContinuousStochasticNode.h"
#include "MoveOld.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"

namespace RevBayesCore {
    
    class AdmixtureEdgeFNPR : public MoveOld {
        
    public:
        AdmixtureEdgeFNPR( StochasticNode<AdmixtureTree> *n, std::vector<ContinuousStochasticNode*> br, int ag, bool asa, double l, double weight);                                               //!<  constructor
        
        // Basic utility functions
        AdmixtureEdgeFNPR*             clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        bool                            isActive(unsigned long g) const;
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            rejectSimpleMove(void);
        void                            acceptSimpleMove(void);
        
        void                            acceptMove(void);
        void                            rejectMove(void);
        double                          performMove(double&);
        
    private:
        
        void                            findNewBrothers(std::vector<AdmixtureNode *> &b, AdmixtureNode &p, AdmixtureNode *n);
        
        // member variables
        StochasticNode<AdmixtureTree>*  variable;
        std::vector<ContinuousStochasticNode*> branchRates;
        
        // tuning parameter
        double                          lambda;
        
        // stored objects to undo proposal
        AdmixtureNode*                  storedAdmixtureChildParent;
        AdmixtureNode*                  storedAdmixtureParentParent;
        AdmixtureNode*                  storedAdmixtureChildChild;
        AdmixtureNode*                  storedAdmixtureParentChild;
        AdmixtureNode*                  storedAdmixtureChild;
        AdmixtureNode*                  storedAdmixtureParent;
        
        AdmixtureNode* newAdmixtureChildChild;// = storedAdmixtureChildChild;
        AdmixtureNode* newAdmixtureParentChild;// = storedAdmixtureParentChild;
        AdmixtureNode* newAdmixtureChildParent;// = storedAdmixtureChildParent;
        AdmixtureNode* newAdmixtureParentParent;// = storedAdmixtureParentParent;
        
        double                          storedAge;
        double                          storedWeight;
        std::map<int,double>            storedBranchRates;
        

        bool changed;
        bool failed;
        bool allowSisterAdmixture;
        unsigned long activeGen;
    };
    
}


#endif /* defined(__rb_mlandis__AdmixtureEdgeFNPR__) */
