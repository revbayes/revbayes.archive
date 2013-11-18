#ifndef VectorScaleMove_H
#define VectorScaleMove_H

#include "SimpleMove.h"
#include "StochasticNode.h"

#include <ostream>
#include <vector>
#include <string>

namespace RevBayesCore {
    
    /**
     * @brief Scaling move of a all elements from a vector.
     *
     * 
     * This move scales all elements of a vector of real values by the same scaling factor.
     * The actual scaling factor is computed by sf = exp( lambda * ( u - 0.5 ) )
     * where u ~ Uniform(0,1).
     * It generally makes more sense to apply the scaling move on a vector of positive
     * real numbers but technically it works on negative numbers too. However, 
     * the move will never change the sign of the value and thus is incomplete if applied
     * to variable defined on the whole real line.
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     *
     */
    class VectorScaleMove : public SimpleMove {
        
    public:
        VectorScaleMove( StochasticNode< std::vector<double> > *n, double l, bool t, double w);                                  //!< Constructor
        
        // Basic utility functions
        VectorScaleMove*                            clone(void) const;                                                                  //!< Clone this object.
        void                                        swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the variable if it was replaced.
        
    protected:
        
        const std::string&                          getMoveName(void) const;                                                            //!< Get the name of the move for summary printing.
        double                                      performSimpleMove(void);                                                            //!< Perform move.
        void                                        printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary.
        void                                        rejectSimpleMove(void);                                                             //!< Reject the last move.
        void                                        tune(void);                                                                         //!< Tune the parameter.
        
    private:
        
        // parameters
        double                                      lambda;                                                                             //!< The scale parameter of the move (larger lambda -> larger proposals).
        std::vector<double>                         storedValue;                                                                        //!< The stored value of the last modified element.
        StochasticNode< std::vector<double> >*      variable;                                                                           //!< The variable on which the move operates. Not owned here.
        
    };
    
}

#endif

