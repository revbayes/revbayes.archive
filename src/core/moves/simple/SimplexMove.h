/**
 * @file
 * This file contains the declaration of SimplexMove, which changes
 * a simplex.
 *
 * @brief Declaration of SimplexMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-11 14:54:35 +0200 (Fri, 11 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-10, version 1.0
 *
 * $Id: SimplexMove.h 1535 2012-05-11 12:54:35Z hoehna $
 */

#ifndef SimplexMove_H
#define SimplexMove_H

#include <ostream>
#include <set>
#include <string>

#include "RbVector.h"
#include "StochasticNode.h"
#include "SimpleMove.h"

namespace RevBayesCore {
    
    class SimplexMove : public SimpleMove {

    public:
        SimplexMove(StochasticNode< RbVector<double> >* node, double a, size_t nc, double o, bool tuning, double weight, double k=0.0);                           //!< Internal constructor

        // Basic utility functions
        SimplexMove*                            clone(void) const;                                                                  //!< Clone object
        void                                    swapNode(DagNode *oldN, DagNode *newN);
    
    protected:
        const std::string&                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                                  performSimpleMove(void);                                                            //!< Perform move
        void                                    printParameterSummary(std::ostream &o) const;
        void                                    rejectSimpleMove(void);
        void                                    tune(void);

    private:

        StochasticNode< RbVector<double> >*     variable;
        double                                  alpha;
        size_t                                  nCategories;
        double                                  offset;
		double									kappa;
        std::vector<double>                     storedValue;

    };
    
}

#endif

