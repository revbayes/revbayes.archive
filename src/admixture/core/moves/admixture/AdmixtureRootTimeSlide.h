//
//  AdmixtureRootTimeSlide.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__AdmixtureRootTimeSlide__
#define __revbayes_mlandis__AdmixtureRootTimeSlide__


#include <ostream>
#include <set>
#include <string>

#include "SimpleMove.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"

namespace RevBayesCore {
    
    class AdmixtureRootTimeSlide : public SimpleMove {
        
    public:
        AdmixtureRootTimeSlide( StochasticNode<AdmixtureTree> *n, double d, bool tuning, double weight);                                                          //!<  constructor
        
        // Basic utility functions
        AdmixtureRootTimeSlide*                  clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            printParameterSummary(std::ostream &o) const;
        void                            rejectSimpleMove(void);
        void                            tune(void);
        
    private:
        void                            rescaleSubtree(AdmixtureNode *n, double factor);
        
        // member variables
        StochasticNode<AdmixtureTree>*  variable;
        double                          delta;
        
        // stored objects to undo proposal
        double                          storedRescale_0;
        double                          storedRescale_1;
        
    };
    
}


#endif /* defined(__revbayes_mlandis__AdmixtureRootTimeSlide__) */
