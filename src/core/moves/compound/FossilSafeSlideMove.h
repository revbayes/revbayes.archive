//
//  FossilSafeSlideMove.h
//  revbayes-proj
//
//  Created by Michael Landis on 2/9/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef FossilSafeSlideMove_H
#define FossilSafeSlideMove_H

#include <map>
#include <ostream>
#include <set>
#include <string>

#include "CompoundMove.h"
#include "TimeTree.h"

namespace RevBayesCore {
    
    class FossilSafeSlideMove : public CompoundMove {
        
    public:
        FossilSafeSlideMove( std::vector<DagNode*> n, double l, bool t, double w);                                                  //!<  constructor
        
        // Basic utility functions
        FossilSafeSlideMove*                    clone(void) const;                                                                  //!< Clone object
        void                                    swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                                  performCompoundMove(void);                                                          //!< Perform move
        void                                    printParameterSummary(std::ostream &o) const;
        void                                    rejectCompoundMove(void);
        void                                    tune(void);
        
    private:
        
        // member variables
        StochasticNode<TimeTree>*               tree;
        ContinuousStochasticNode*               scaler;

        // tuning parameter
        double                                  lambda;
        
        // stored objects to undo proposal
		double									storedValue;
        
    };
    
}

#endif /* defined(FossilSafeSlideMove_H) */
