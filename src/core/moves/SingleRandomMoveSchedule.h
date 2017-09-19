//
//  SingleRandomMoveSchedule.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 2/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__SingleRandomMoveSchedule__
#define __revbayes_mlandis__SingleRandomMoveSchedule__


#include "Move.h"
#include "MoveSchedule.h"
#include "RbVector.h"

#include <vector>

namespace RevBayesCore {
    
    class SingleRandomMoveSchedule : public MoveSchedule  {
        
    public:
        SingleRandomMoveSchedule(RbVector<Move> *m);                                                                                                                                         //!< Default constructor
        virtual                                        ~SingleRandomMoveSchedule(void);                                                                             //!< Destructor
        
        // pure virtual public methods
        SingleRandomMoveSchedule*                       clone(void) const;
        double                                          getNumberMovesPerIteration(void) const;
        void                                            setNumberMovesPerIteration(double);
        Move&                                           nextMove(unsigned long g);
        
    private:

        // Hidden member variables
        double                                          movesPerIteration;
    };
    
}

#endif /* defined(__revbayes_mlandis__SingleRandomMoveSchedule__) */
