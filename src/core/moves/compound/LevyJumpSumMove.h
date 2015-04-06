//
//  LevyJumpSumMove.h
//  revbayes-proj
//
//  Created by Michael Landis on 4/4/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__LevyJumpSumMove__
#define __revbayes_proj__LevyJumpSumMove__


#include <map>
#include <ostream>
#include <set>
#include <string>

#include "CompoundMove.h"
#include "ContinuousStochasticNode.h"

namespace RevBayesCore {
    
    class LevyJumpSumMove : public CompoundMove {
        
    public:
        LevyJumpSumMove( std::vector<DagNode*> n, double sf, bool t, double w);                                                         //!<  constructor
        
        // Basic utility functions
        LevyJumpSumMove*                        clone(void) const;                                                                  //!< Clone object
        void                                    swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                                  performCompoundMove(void);                                                            //!< Perform move
        void                                    printParameterSummary(std::ostream &o) const;
        void                                    rejectCompoundMove(void);
        void                                    tune(void);
        
    private:
        
        // member variables
        ContinuousStochasticNode*               sliderVal1;
        ContinuousStochasticNode*               sliderVal2;
        double									slideFactor;
        
        // stored objects to undo proposal
		double									storedSV1;
		double									storedSV2;
        
    };
    
}


#endif /* defined(__revbayes_proj__LevyJumpSumMove__) */
