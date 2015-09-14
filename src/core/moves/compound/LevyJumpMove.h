//
//  LevyJumpMove.h
//  revbayes-proj
//
//  Created by Michael Landis on 4/4/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__LevyJumpMove__
#define __revbayes_proj__LevyJumpMove__

#include <ostream>
#include <set>
#include <string>

#include "ContinuousStochasticNode.h"
#include "SimpleMove.h"

namespace RevBayesCore {
    
    class LevyJumpMove : public SimpleMove {
        
    public:
        LevyJumpMove(ContinuousStochasticNode* node, double d, bool tuning, double weight);                           //!< Internal constructor
        
        // Basic utility functions
        LevyJumpMove*                           clone(void) const;                                                                  //!< Clone object
        void                                    swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                                  performSimpleMove(void);                                                            //!< Perform move
        void                                    printParameterSummary(std::ostream &o) const;
        void                                    rejectSimpleMove(void);
        void                                    tune(void);
        
    private:
        
        ContinuousStochasticNode*               variable;
        double                                  delta;
        double                                  storedValue;
        
    };
    
}



#endif /* defined(__revbayes_proj__LevyJumpMove__) */
