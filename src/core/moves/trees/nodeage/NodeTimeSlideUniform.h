/**
 * @file
 * This file contains the declaration of NodeTimeSlideUniform, 
 * which randomly draws a node in the tree and draws a random new time for the node
 * between its parents time and the children time.
 *
 * @brief Declaration of SubtreeScale
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-12 16:14:14 +0200 (Thu, 12 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-12, version 1.0
 *
 * $Id: ScaleMove.h 1677 2012-07-12 14:14:14Z hoehna $
 */

#ifndef NodeTimeSlideUniform_H
#define NodeTimeSlideUniform_H

#include <ostream>
#include <set>
#include <string>

#include "SimpleMove.h"
#include "StochasticNode.h"
#include "TimeTree.h"

namespace RevBayesCore {
    
    class NodeTimeSlideUniform : public SimpleMove {
        
    public:
        NodeTimeSlideUniform( StochasticNode<TimeTree> *n, double weight);                                                         //!<  constructor
        
        // Basic utility functions
        NodeTimeSlideUniform*           clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            rejectSimpleMove(void);
        
    private:   
        
        // member variables
        StochasticNode<TimeTree>*       variable;
        
        // stored objects to undo proposal
        TopologyNode*                   storedNode;
        double                          storedAge;
        
    };
    
}

#endif

