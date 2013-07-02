//
//  AdmixtureNodeTimeSlideBeta.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__AdmixtureNodeTimeSlideBeta__
#define __revbayes_mlandis__AdmixtureNodeTimeSlideBeta__


#include <ostream>
#include <set>
#include <string>

#include "SimpleMove.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"

namespace RevBayesCore {
    
    class AdmixtureNodeTimeSlideBeta : public SimpleMove {
        
    public:
        AdmixtureNodeTimeSlideBeta( StochasticNode<AdmixtureTree> *n, int ni, double d, bool tuning, double weight);                                                         //!<  constructor
        
        // Basic utility functions
        AdmixtureNodeTimeSlideBeta*  clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            rejectSimpleMove(void);
        void                            acceptSimpleMove(void);
        void                            tune(void);
        
    private:
        
        int nodeIndex;
        // member variables
        StochasticNode<AdmixtureTree>*  variable;
        
        // stored objects to undo proposal
        bool                            failed;
        AdmixtureNode*                  storedNode;
        double                          storedAge;
        
        double                          delta;
    };
    
}

#endif /* defined(__revbayes_mlandis__AdmixtureNodeTimeSlideBeta__) */
