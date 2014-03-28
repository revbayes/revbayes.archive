//
//  PrecisionMatrixSimpleMove.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-28.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__PrecisionMatrixSimpleMove__
#define __revbayes__PrecisionMatrixSimpleMove__

#include <iostream>
#include "SimpleMove.h"
#include "StochasticNode.h"
#include "PrecisionMatrix.h"


namespace RevBayesCore {
    
    /**
     * @brief Scaling move of a single element randomly picked from a vector.
     *
     *
     * This move randomly picks an element of a vector of positive real numbers,
     * proposes a scaling factor and then Slidings the value.
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
    class PrecisionMatrixMove : public SimpleMove {
        
    public:
        PrecisionMatrixMove(StochasticNode<PrecisionMatrix >* n, double l, bool t, double w);                         //!< Constructor
        
        // Basic utility functions
        PrecisionMatrixMove*                     clone(void) const;                                                                  //!< Clone this object.
        const std::string&                          getMoveName(void) const;                                                            //!< Get the name of the move for summary printing.
        void                                        swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the variable if it was replaced.
        
    protected:
        
        double                                  performSimpleMove(void);                                                            //!< Perform move
        void                                    printParameterSummary(std::ostream &o) const;
        void                                    rejectSimpleMove(void);
        void                                    acceptSimpleMove(void);
        void                                    tune(void);
        void                                    touch( DagNode *toucher );
        
    private:
        
        StochasticNode<PrecisionMatrix >*   variable;
        
        double                                      lambda;                                                                             //!< The Sliding parameter of the move (larger lambda -> larger proposals).
 		PrecisionMatrix                                      storedValue;                                                                        //!< The stored value of the last modified element.
        
    };
    
}


#endif /* defined(__revbayes__PrecisionMatrixSimpleMove__) */
