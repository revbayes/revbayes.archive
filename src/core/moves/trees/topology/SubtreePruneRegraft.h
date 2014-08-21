/**
 * @file
 * This file contains the declaration of Subtree-Prune-and-Regraft, 
 * which randomly draws a node in the tree and exchanges its two neighbors.
 *
 * @brief Declaration of SubtreePruneRegraft
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

#ifndef SubtreePruneRegraft_H
#define SubtreePruneRegraft_H

#include <ostream>
#include <set>
#include <string>

#include "SimpleMove.h"
#include "StochasticNode.h"
#include "Topology.h"

namespace RevBayesCore {
    
    class SubtreePruneRegraft : public SimpleMove {
        
    public:
        SubtreePruneRegraft( StochasticNode<Topology> *n, double weight);                                            //!<  constructor
        
        // Basic utility functions
        SubtreePruneRegraft*                        clone(void) const;                                                                  //!< Clone object
        void                                        swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&                          getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                                      performSimpleMove(void);                                                            //!< Perform move
        void                                        rejectSimpleMove(void);
        
    private:
        // private methods
        bool                                        isDescendant(const TopologyNode &n, const TopologyNode &p);
        
        // member variables
        StochasticNode<Topology>*                   variable;
        
        // stored objects to undo proposal
        TopologyNode*                               storedChoosenNode;
        TopologyNode*                               storedBrother;
        
    };
    
}

#endif

