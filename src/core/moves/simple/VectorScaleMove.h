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
     * Note, we assume that the number of elements in the vector does not change!
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     *
     */
    class VectorScaleMove : public Move {
        
    public:
        VectorScaleMove( const std::vector<StochasticNode< double > *> &n, double l, bool t, double w);                                  //!< Constructor
        
        // Basic utility functions
        VectorScaleMove*                            clone(void) const;                                                                  //!< Clone this object.
        const std::string&                          getMoveName(void) const;                                                            //!< Get the name of the move for summary printing.
        void                                        swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the variable if it was replaced.
        
    protected:
        
        void                                        acceptMove(void);
        double                                      performMove(double& probRatio);                                                            //!< Perform move.
        void                                        printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary.
        void                                        rejectMove(void);                                                             //!< Reject the last move.
        void                                        tune(void);                                                                         //!< Tune the parameter.
        
    private:
        
        // parameters
        bool                                        changed;
        double                                      lambda;                                                                             //!< The scale parameter of the move (larger lambda -> larger proposals).
        std::vector<double>                         storedValue;                                                                        //!< The stored value of the last modified element.
        std::vector< StochasticNode< double >* >    variable;                                                                           //!< The variable on which the move operates. Not owned here.
        size_t                                      length;                                                                             //!< The length of the variable vector.
    };
    
}

#endif

