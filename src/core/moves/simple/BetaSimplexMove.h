/**
 * @file
 * This file contains the declaration of BetaSimplexMove, which changes
 * a simplex.
 *
 * @brief Declaration of BetaSimplexMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-11 14:54:35 +0200 (Fri, 11 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-10, version 1.0
 *
 * $Id: BetaSimplexMove.h 1535 2012-05-11 12:54:35Z hoehna $
 */

#ifndef BetaSimplexMove_H
#define BetaSimplexMove_H

#include <ostream>
#include <set>
#include <string>

#include "StochasticNode.h"
#include "SimpleMove.h"

namespace RevBayesCore {
    
    class BetaSimplexMove : public SimpleMove {
        
    public:
        BetaSimplexMove(StochasticNode<double>* node, double a, bool tuning, double weight);                           //!< Internal constructor
        
        // Basic utility functions
        BetaSimplexMove*                        clone(void) const;                                                                  //!< Clone object
        void                                    swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                                  performSimpleMove(void);                                                            //!< Perform move
        void                                    printParameterSummary(std::ostream &o) const;
        void                                    rejectSimpleMove(void);
        void                                    tune(void);
        
    private:
        
        StochasticNode<double>*                 variable;
        double                                  alpha;
        double                                  storedValue;
        
    };
    
}

#endif

