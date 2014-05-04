//
//  SamplePathHistoryCtmcMove.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/4/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__SamplePathHistoryCtmcMove__
#define __rb_mlandis__SamplePathHistoryCtmcMove__



#include <ostream>
#include <set>
#include <string>
#include <vector>

#include "AbstractTreeHistoryCtmc.h"
#include "BranchHistory.h"
#include "SimpleMove.h"
#include "StochasticNode.h"

namespace RevBayesCore {
    
    class SamplePathHistoryCtmcMove : public SimpleMove {
        
    public:
        SamplePathHistoryCtmcMove( StochasticNode<AbstractCharacterData> *n, double l, bool tuning, double w);                                    //!<  constructor
        
        // Basic utility functions
        SamplePathHistoryCtmcMove* clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        void                            acceptSimpleMove(void);
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            printParameterSummary(std::ostream &o) const;
        void                            rejectSimpleMove(void);
        void                            tune(void);
        
    private:
        
        StochasticNode<AbstractCharacterData>*  variable;
        BranchHistory                   storedValue;
        size_t                          storedIndex;
        
        // parameters
        double                          lambda;
        size_t                          numNodes;
        size_t                          numCharacters;
        size_t                          numStates;
        
        
    };
    
}

#endif /* defined(__rb_mlandis__SamplePathHistoryCtmcMove__) */
