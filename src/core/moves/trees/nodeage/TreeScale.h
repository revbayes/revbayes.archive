/**
 * @file
 * This file contains the declaration of TreeScale, which rescales the whole tree.
 *
 * @brief Declaration of TreeScale
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-12 16:14:14 +0200 (Thu, 12 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-08-29, version 1.0
 *
 * $Id: ScaleMove.h 1677 2012-07-12 14:14:14Z hoehna $
 */

#ifndef TreeScale_H
#define TreeScale_H

#include <ostream>
#include <set>
#include <string>

#include "CompoundMove.h"
#include "StochasticNode.h"
#include "TimeTree.h"

namespace RevBayesCore {
    
    class TreeScale : public CompoundMove {
        
    public:
        TreeScale( StochasticNode<TimeTree> *t, StochasticNode<double> *r, double d, bool tuning, double w);                                                          //!<  constructor
        
        // Basic utility functions
        TreeScale*                      clone(void) const;                                                                  //!< Clone object
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
        StochasticNode<double>*         rootAge;
        
        // parameters
        double                          delta;
        
        // stored objects to undo proposal
        double                          storedAge;
        
    };
    
}

#endif

