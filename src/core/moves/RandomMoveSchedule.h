/**
 * @file
 * This file contains the declaration of Move, which is the abstract
 * base class for moves used in the mcmc inference.
 *
 * @brief Declaration of the base class Move
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-08-17, version 1.0
 *
 * $Id$
 */

#ifndef RandomMoveSchedule_H
#define RandomMoveSchedule_H

#include "Move.h"
#include "MoveSchedule.h"

#include <vector>

namespace RevBayesCore {
    
    class RandomMoveSchedule : public MoveSchedule  {
        
    public:
        RandomMoveSchedule(const std::vector<Move*> &m);                                                                                                                                         //!< Default constructor
        virtual                                        ~RandomMoveSchedule(void);                                                                             //!< Destructor
        
        // pure virtual public methods
        RandomMoveSchedule*                             clone(void) const;
        double                                          getNumberMovesPerIteration(void) const;
        Move*                                           nextMove(unsigned long g);

    private:
        
        // Hidden member variables
        double                                          movesPerIteration;
        std::vector<double>                             weights;
    };
    
}

#endif
