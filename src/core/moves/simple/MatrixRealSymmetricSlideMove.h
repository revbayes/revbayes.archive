//
//  MatrixRealSimpleMove.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-28.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef MatrixRealSimpleMove_H
#define MatrixRealSimpleMove_H

#include <iostream>
#include "SimpleMove.h"
#include "StochasticNode.h"
#include "MatrixReal.h"


namespace RevBayesCore {
    
    /**
     * @brief Sliding move of a single element randomly picked from a symmetric matrix.
     *
     *
     * This move randomly picks an element of a matrix of positive real numbers.
     * That means, that we randomly pick the i-th row and j-th column with equal probability.
     * Then, we propose a sliding distance and slide the value.
     * The actual sliding distance is computed by delta = lambda * ( u - 0.5 )
     * where u ~ Uniform(0,1).
     * The proposal is thus m[i][j] += lambda * ( u - 0.5 )
     * and additionally m[j][i] = m[i][j]
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     *
     */
    class MatrixRealSymmetricSlideMove : public SimpleMove {
        
    public:
        MatrixRealSymmetricSlideMove(StochasticNode<MatrixReal>* n, double l, bool t, double w);                         //!< Constructor
        
        // Basic utility functions
        MatrixRealSymmetricSlideMove*           clone(void) const;                                                                  //!< Clone this object.
        const std::string&                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing.
        void                                    swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the variable if it was replaced.
        
    protected:
        
        double                                  performSimpleMove(void);                                                            //!< Perform move
        void                                    printParameterSummary(std::ostream &o) const;
        void                                    rejectSimpleMove(void);
        void                                    acceptSimpleMove(void);
        void                                    tune(void);
        void                                    touch( DagNode *toucher );
        
    private:
        
        StochasticNode<MatrixReal>*             variable;
            
        double                                  lambda;                                                                             //!< The Sliding parameter of the move (larger lambda -> larger proposals).
        size_t                                  stored_i;
        size_t                                  stored_j;
        double                                  storedValue;                                                                        //!< The stored value of the last modified element.
        
    };
    
}


#endif /* defined(__revbayes__MatrixRealSimpleMove__) */
