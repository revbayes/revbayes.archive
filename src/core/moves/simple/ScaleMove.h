/**
 * @file
 * This file contains the declaration of ScaleMove, which scales
 * a real value with a factor drawn from an appropriate distribution.
 *
 * @brief Declaration of ScaleMove
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

#ifndef ScaleMove_H
#define ScaleMove_H

#include <ostream>
#include <set>
#include <string>

#include "SimpleMove.h"
#include "StochasticNode.h"

namespace RevBayesCore {
    
    class ScaleMove : public SimpleMove {

    public:
        ScaleMove( StochasticNode<double> *n, double l, bool tuning, double w);                                    //!<  constructor

        // Basic utility functions
        ScaleMove*                      clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
    
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            printParameterSummary(std::ostream &o) const;
        void                            rejectSimpleMove(void);
        void                            tune(void);

    private:
        // parameters
        double                          lambda;
    
        StochasticNode<double>*         variable;
        double                          storedValue;

    };
    
}

#endif

