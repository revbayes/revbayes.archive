//
//  AdmixtureEdgeReweight.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__AdmixtureEdgeReweight__
#define __revbayes_mlandis__AdmixtureEdgeReweight__

#include <ostream>
#include <set>
#include <string>

#include "SimpleMove.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"

namespace RevBayesCore {
    
    class AdmixtureEdgeReweight : public SimpleMove {
        
    public:
        AdmixtureEdgeReweight( StochasticNode<AdmixtureTree> *n, int ag, double l, double weight);                                                          //!<  constructor
        
        // Basic utility functions
        AdmixtureEdgeReweight*          clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        bool                            isActive(unsigned long g) const;
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            rejectSimpleMove(void);
        
    private:
        
        // member variables
        StochasticNode<AdmixtureTree>*  variable;
        double                          lambda;
        
        // stored objects to undo proposal
        AdmixtureNode*                  storedAdmixtureChild;
        double                          storedWeight;

        bool failed;
        unsigned long activeGen;
        
    };
    
}



#endif /* defined(__revbayes_mlandis__AdmixtureEdgeReweight__) */
