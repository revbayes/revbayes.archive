#ifndef VectorFixedSingleElementSlidingMove_H
#define VectorFixedSingleElementSlidingMove_H

#include "SimpleMove.h"
#include "StochasticNode.h"
#include "RbVector.h"

#include <ostream>
#include <vector>
#include <string>

namespace RevBayesCore {
    
    /**
     * @brief Scaling move of a single element which is pre-specified from a vector.
     *
     * 
     * This move roposes a scaling factor and then Slidings the value.
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
    class VectorFixedSingleElementSlidingMove : public SimpleMove {
        
    public:
        VectorFixedSingleElementSlidingMove(StochasticNode<RbVector<double> >* n, double l, bool t, double w, size_t elem);                         //!< Constructor
        
        // Basic utility functions
        VectorFixedSingleElementSlidingMove*        clone(void) const;                                                                  //!< Clone this object.
        const std::string&                          getMoveName(void) const;                                                            //!< Get the name of the move for summary printing.
        void                                        swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the variable if it was replaced.
        
    protected:
        
        double                                      performSimpleMove(void);                                                            //!< Perform move
        void                                        printParameterSummary(std::ostream &o) const;
        void                                        rejectSimpleMove(void);
        void                                        acceptSimpleMove(void);
        void                                        tune(void);
        void                                        touch( DagNode *toucher );
        
    private:
        
        StochasticNode< RbVector<double> >*         variable;

        double                                      lambda;                                                                             //!< The Sliding parameter of the move (larger lambda -> larger proposals).
        size_t                                      index;                                                                              //!< The index of the last modified element.
		double                                      storedValue;                                                                        //!< The stored value of the last modified element.
        
    };
    
}

#endif

