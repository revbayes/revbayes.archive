//
//  VectorSlideMove.h
//  revbayes-proj
//
//  Created by Michael Landis on 4/4/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__VectorSlideMove__
#define __revbayes_proj__VectorSlideMove__

#include "MoveOld.h"
#include "StochasticNode.h"

#include <ostream>
#include <vector>
#include <string>

namespace RevBayesCore {
    
    /**
     * @brief Scaling move of a all elements from a vector.
     *
     *
     * This move slides all elements of a vector of real values by the same scaling factor.
     * The actual scaling factor is computed by sf = exp( lambda * ( u - 0.5 ) )
     * where u ~ Uniform(0,1).
     * Note, we assume that the number of elements in the vector does not change!
     *
     * @author The RevBayes Development Core Team (Michael Landis)
     * @copyright GPL version 3
     *
     */
    class VectorSlideMove : public MoveOld {
        
    public:
        VectorSlideMove( const std::vector<StochasticNode< double > *> &n, double l, bool t, double w);                                  //!< Constructor
        
        // Basic utility functions
        VectorSlideMove*                            clone(void) const;                                                                  //!< Clone this object.
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

#endif /* defined(__revbayes_proj__VectorSlideMove__) */
