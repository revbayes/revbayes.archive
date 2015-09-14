//
//  AdmixtureNodeTimeSlideUniform.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__AdmixtureNodeTimeSlideUniform__
#define __revbayes_mlandis__AdmixtureNodeTimeSlideUniform__


#include <ostream>
#include <set>
#include <string>

#include "SimpleMove.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"

namespace RevBayesCore {
    
    class AdmixtureNodeTimeSlideUniform : public SimpleMove {
        
    public:
        AdmixtureNodeTimeSlideUniform( StochasticNode<AdmixtureTree> *n, double weight);                                                         //!<  constructor
        
        // Basic utility functions
        AdmixtureNodeTimeSlideUniform*  clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            rejectSimpleMove(void);
        
    private:
        
        // member variables
        StochasticNode<AdmixtureTree>*  variable;
        
        // stored objects to undo proposal
        bool                            failed;
        AdmixtureNode*                  storedNode;
        double                          storedAge;
        
    };
    
}

#endif /* defined(__revbayes_mlandis__AdmixtureNodeTimeSlideUniform__) */
