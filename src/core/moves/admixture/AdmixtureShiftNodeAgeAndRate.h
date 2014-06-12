//
//  AdmixtureShiftNodeAgeAndRate.h
//  rb_mlandis
//
//  Created by Michael Landis on 3/27/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureShiftNodeAgeAndRate__
#define __rb_mlandis__AdmixtureShiftNodeAgeAndRate__

#include <ostream>
#include <set>
#include <string>

#include "MoveOld.h"
#include "ContinuousStochasticNode.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"

namespace RevBayesCore {
    
    class AdmixtureShiftNodeAgeAndRate : public MoveOld {
        
    public:
        AdmixtureShiftNodeAgeAndRate( StochasticNode<AdmixtureTree> *n, std::vector<ContinuousStochasticNode*> br, int ni, double d, bool t, double weight);                                                         //!<  constructor
        
        // Basic utility functions
        AdmixtureShiftNodeAgeAndRate*  clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        
        void                            acceptMove(void);                                                                   //!< Accept the InferenceMoveSimple
        double                          performMove(double& probRatio);                                                     //!< Perform the InferenceMoveSimple
        void                            rejectMove(void);
        
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            rejectSimpleMove(void);
        void                            acceptSimpleMove(void); // non-pure virtual accept function
        void                            tune(void);


        
    private:
        
        // member variables
        StochasticNode<AdmixtureTree>*  variable;
        std::vector< ContinuousStochasticNode*> branchRates;
        
        // stored objects to undo proposal
        int                             nodeIndex;
        bool                            changed;
        bool                            failed;
        double                          delta;
        AdmixtureNode*                  storedNode;
        double                          storedAge;
        std::map<AdmixtureNode*,double> storedRates;
        
    };
    
}

#endif /* defined(__rb_mlandis__AdmixtureShiftNodeAgeAndRate__) */
