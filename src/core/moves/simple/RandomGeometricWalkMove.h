#ifndef RandomGeometricWalkMove_H
#define RandomGeometricWalkMove_H

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
     * to increase the current value and with probability p = 0.5 to decrease the 
     * current value. The number of steps taken by this move (value by which the current value is increased or decrease)
     * is geometrically distributed. 
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2009-09-08, version 1.0
     *
     */
    class RandomGeometricWalkMove : public SimpleMove {
        
    public:
        RandomGeometricWalkMove( StochasticNode<int> *n, double q, bool tuning, double w);                                                           //!<  constructor
        
        // Basic utility functions
        RandomGeometricWalkMove*        clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the DAG nodes on which the move is working on
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                            rejectSimpleMove(void);                                                             //!< Reject the move
        void                            tune(void);                                                                         //!< Tune the move to achieve a better acceptance/rejection ratio
        
    private:
        
        StochasticNode<int>*            variable;                                                                           //!< The variable the move is working on
        int                             storedValue;                                                                        //!< The stored value of the move used for rejections.
        double                          p;                                                                                  //!< The parameter of the geometric distribution that influence the proposal size.
    };
    
}

#endif

