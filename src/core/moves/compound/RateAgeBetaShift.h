//
//  RateAgeBetaShift.h
//  rb_mlandis
//
//  Created by Michael Landis on 7/4/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__RateAgeBetaShift__
#define __rb_mlandis__RateAgeBetaShift__

#include <map>
#include <ostream>
#include <set>
#include <string>

#include "CompoundMove.h"
#include "StochasticNode.h"
#include "TimeTree.h"

namespace RevBayesCore {
    
    class RateAgeBetaShift : public CompoundMove {
        
    public:
        RateAgeBetaShift( std::vector<DagNode*> n, double delta, bool t, double weight);                                                         //!<  constructor
        
        // Basic utility functions
        RateAgeBetaShift*               clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performCompoundMove(void);                                                            //!< Perform move
        void                            printParameterSummary(std::ostream &o) const;
        void                            rejectCompoundMove(void);
        void                            tune(void);
        
    private:
        
        // member variables
        StochasticNode<TimeTree>*       tree;
        std::vector<StochasticNode<double>* > rates;
        double                          delta;
        
        // stored objects to undo proposal
        TopologyNode*                   storedNode;
        double                          storedAge;
        std::map<int,double>            storedRates;
        
    };
    
}

#endif /* defined(__rb_mlandis__RateAgeBetaShift__) */
