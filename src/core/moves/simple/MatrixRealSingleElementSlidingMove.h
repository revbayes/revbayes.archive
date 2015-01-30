/* 
 * File:   MatrixRealSingleElementSlidingMove.h
 * Author: nl
 *
 * Created on 13 juillet 2014, 17:55
 */

#ifndef MATRIXREALSINGLEELEMENTSLIDINGMOVE_H
#define	MATRIXREALSINGLEELEMENTSLIDINGMOVE_H

#include "MatrixReal.h"
#include "SimpleMove.h"
#include "StochasticNode.h"

#include <ostream>
#include <string>


namespace RevBayesCore {
    
    /**
     * @brief Scaling move of a single element randomly picked from a vector.
     *
     * 
     * This move randomly picks an element of a matrix of positive real numbers.
     * That means, that we randomly pick the i-th row and j-th column with equal probability.
     * Then, we propose a sliding distance and slide the value.
     * The actual sliding distance is computed by delta = lambda * ( u - 0.5 )
     * where u ~ Uniform(0,1).
     * The proposal is thus m[i][j] += lambda * ( u - 0.5 )
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     *
     */
    class MatrixRealSingleElementSlidingMove : public SimpleMove {
        
    public:
        MatrixRealSingleElementSlidingMove(StochasticNode<MatrixReal>* n, double l, bool t, double w);                         //!< Constructor
        
        // Basic utility functions
        MatrixRealSingleElementSlidingMove*         clone(void) const;                                                                  //!< Clone this object.
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
        
        StochasticNode<MatrixReal >*                variable;

        double                                      lambda;                                                                             //!< The Sliding parameter of the move (larger lambda -> larger proposals).
        //!< The two indices of the last modified element.
        size_t                                      indexa;    
        size_t                                      indexb;  
        double                                      storedValue;                                                                        //!< The stored value of the last modified element.
        
    };
    
}


#endif	/* MATRIXREALSINGLEELEMENTSLIDINGMOVE_H */

