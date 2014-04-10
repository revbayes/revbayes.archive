/**
 * @file
 * This file contains the declaration of a path-resample move, which changes
 * a the path of a random walk (Brownian motion).
 *
 * @brief Declaration of SimplexMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-11 14:54:35 +0200 (Fri, 11 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-08-14, version 1.0
 *
 * $Id: SimplexMove.h 1535 2012-05-11 12:54:35Z hoehna $
 */

#ifndef PathResampleMove_H
#define PathResampleMove_H

#include <ostream>
#include <set>
#include <string>

#include "UnivariateFunction.h"
#include "StochasticNode.h"
#include "SimpleMove.h"

namespace RevBayesCore {
    
    class PathResampleMove : public SimpleMove {
        
    public:
        PathResampleMove(StochasticNode<UnivariateFunction>* node, int nb, double lambda, bool tuning, double weight);                                        //!< Internal constructor
        
        // Basic utility functions
        PathResampleMove*                       clone(void) const;                                                                  //!< Clone object
        void                                    swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                                  performSimpleMove(void);                                                            //!< Perform move
        void                                    printParameterSummary(std::ostream &o) const;
        void                                    rejectSimpleMove(void);
        void                                    tune(void);
        
    private:
        double                                  computeProposalProbability(const std::vector<double> &v, int b, int e);
        void                                    propose(std::vector<double> &v, int b, int e);
        
        
        StochasticNode<UnivariateFunction>*     variable;
        int                                     index;
        int                                     nBlocks;
        double                                  lambda;
        std::vector<double>                     storedValue;
        
    };
    
}

#endif

