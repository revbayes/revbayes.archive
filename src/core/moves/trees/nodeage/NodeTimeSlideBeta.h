/**
 * @file
 * This file contains the declaration of NodeTimeSlideBeta, 
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

#ifndef NodeTimeSlideBeta_H
#define NodeTimeSlideBeta_H

#include <ostream>
#include <set>
#include <string>

#include "SimpleMove.h"
#include "StochasticNode.h"
#include "TimeTree.h"

namespace RevBayesCore {
    
    class NodeTimeSlideBeta : public SimpleMove {
        
    public:
        NodeTimeSlideBeta( StochasticNode<TimeTree> *n, double delta, double o, bool t, double weight);                                                         //!<  constructor
        
        // Basic utility functions
        NodeTimeSlideBeta*              clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            printParameterSummary(std::ostream &o) const;
        void                            rejectSimpleMove(void);
        void                            tune(void);

    private:   
        
        // member variables
        StochasticNode<TimeTree>*       variable;
        double                          delta;
        double                          offset;
        
        // stored objects to undo proposal
        TopologyNode*                   storedNode;
        double                          storedAge;
        
    };
    
}

#endif

