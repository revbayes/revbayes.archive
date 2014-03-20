#ifndef RandomIntegerWalkMove_H
#define RandomIntegerWalkMove_H

#include <ostream>
#include <set>
#include <string>

#include "SimpleMove.h"
#include "StochasticNode.h"

namespace RevBayesCore {
    
    /**
     * The random walk on integers move. 
     *
     *
     * This is a very simple move on integer numbers that proposes with probability p = 0.5
     * to increase the current value by 1 and with probability p = 0.5 to decrease the 
     * current value by 1. Thus, it is a random walk but guided by the acceptance ratio.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2009-09-08, version 1.0
     *
     */
    class RandomIntegerWalkMove : public SimpleMove {
        
    public:
        RandomIntegerWalkMove( StochasticNode<int> *n, double w);                                                           //!<  constructor
        
        // Basic utility functions
        RandomIntegerWalkMove*          clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the DAG nodes on which the move is working on
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            rejectSimpleMove(void);                                                             //!< Reject the move
        
    private:
        
        StochasticNode<int>*            variable;                                                                           //!< The variable the move is working on
        int                             storedValue;                                                                        //!< The stored value of the move used for rejections.
        
    };
    
}

#endif

