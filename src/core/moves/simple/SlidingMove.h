/**
 * @file
 * This file contains the declaration of SlidingMove, which moves
 * a real value uniformly within a sliding window.
 *
 * @brief Declaration of SlidingMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#ifndef SlidingMove_H
#define SlidingMove_H

#include <ostream>
#include <set>
#include <string>

#include "SimpleMove.h"
#include "ContinuousStochasticNode.h"

namespace RevBayesCore {

    class SlidingMove : public SimpleMove {

    public:
                                    SlidingMove( ContinuousStochasticNode *n, double d, bool tuning, double w );                     //!<  constructor

        // Basic utility functions
        SlidingMove*                clone(void) const;                                                                  //!< Clone object
        void                        swapNode(DagNode *oldN, DagNode *newN);
    
	protected:
        const std::string&          getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                      performSimpleMove(void);                                                            //!< Perform move
        void                        printParameterSummary(std::ostream &o) const;
        void                        rejectSimpleMove(void);
        void                        tune(void);

    private:

        // parameters
        double                      delta;
        ContinuousStochasticNode*   variable;
        double                      storedValue;
};
    
}

#endif

