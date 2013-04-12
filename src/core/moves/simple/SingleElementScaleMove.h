/**
 * @file
 * This file contains the declaration of SingleElementScaleMove, which scales
 * a real value with a factor drawn from an appropriate distribution.
 *
 * @brief Declaration of SingleElementScaleMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-09-10 11:02:28 +0200 (Mon, 10 Sep 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id: SingleElementScaleMove.h 1817 2012-09-10 09:02:28Z hoehna $
 */

#ifndef SingleElementScaleMove_H
#define SingleElementScaleMove_H

#include <ostream>
#include <vector>
#include <string>

#include "SimpleMove.h"
#include "StochasticNode.h"

namespace RevBayesCore {
    
    class SingleElementScaleMove : public SimpleMove {
        
    public:
        SingleElementScaleMove( StochasticNode< std::vector<double> > *n, double l, bool t, double w);                                    //!<  constructor
        
        // Basic utility functions
        SingleElementScaleMove*                     clone(void) const;                                                                  //!< Clone object
        void                                        swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&                          getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                                      performSimpleMove(void);                                                            //!< Perform move
        void                                        printParameterSummary(std::ostream &o) const;
        void                                        rejectSimpleMove(void);
        void                                        tune(void);
        
    private:
        // parameters
        double                                      lambda;
        
        StochasticNode< std::vector<double> >*      variable;
        size_t                                      index;
        double                                      storedValue;
        
    };
    
}

#endif

