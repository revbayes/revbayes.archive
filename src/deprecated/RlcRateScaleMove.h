#ifndef RlcRateScaleMove_H
#define RlcRateScaleMove_H

#include "RbVector.h"
#include "SimpleMove.h"
#include "StochasticNode.h"

#include <ostream>
#include <vector>
#include <string>

namespace RevBayesCore {
    
    /**
     * @brief Move that scales the rate of a RateJumpProcess if a jump occurred.
     *
     * 
     * 
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     *
     */
    class RlcRateScaleMove : public SimpleMove {
        
    public:
        RlcRateScaleMove(StochasticNode< RbVector<double> >* n, double l, bool t, double w);                                          //!< Constructor
        
        // Basic utility functions
        RlcRateScaleMove*                           clone(void) const;                                                                  //!< Clone this object.
        const std::string&                          getMoveName(void) const;                                                            //!< Get the name of the move for summary printing.
        void                                        swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the variable if it was replaced.
        
    protected:
        
        double                                      performSimpleMove(void);                                                            //!< Perform move
        void                                        rejectSimpleMove(void);
        void                                        acceptSimpleMove(void);
        void                                        touch( DagNode *toucher );
        void                                        tune(void);                                                                         //!< Tune the parameter.
       
    private:
        
        StochasticNode< RbVector<double> >*         variable;
        
        size_t                                      index;                                                                              //!< The index of the last modified element.
        double                                      lambda;                                                                             //!< The scale parameter of the move (larger lambda -> larger proposals).
		double                                      storedValue;                                                                        //!< The stored value of the last modified element.
        
    };
    
}

#endif

