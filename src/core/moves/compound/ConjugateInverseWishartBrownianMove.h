#ifndef ConjugateInverseWishartBrownianMove_H
#define ConjugateInverseWishartBrownianMove_H

#include <map>
#include <ostream>
#include <set>
#include <string>

#include "AbstractGibbsMove.h"
#include "MatrixReal.h"
#include "StochasticNode.h"
#include "Tree.h"

namespace RevBayesCore {
    
    class ConjugateInverseWishartBrownianMove : public AbstractGibbsMove {
        
    public:
        ConjugateInverseWishartBrownianMove(StochasticNode<MatrixReal>* s, TypedDagNode<double>* k, TypedDagNode<int>* d, double w);                                                         //!<  constructor
        virtual                                ~ConjugateInverseWishartBrownianMove(void);                                                             //!< Destructor
        
        // Basic utility functions
        ConjugateInverseWishartBrownianMove*    clone(void) const;                                                                  //!< Clone object
        const std::string&                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        
    protected:
        void                                    performGibbsMove(void);                                 //!< Perform move
        void                                    swapNodeInternal(DagNode *oldN, DagNode *newN);                                             //!< Swap the pointers to the variable on which the move works on.
        
    private:
        
        // member variables
        StochasticNode<MatrixReal>*             sigma;
        TypedDagNode<double>*                   kappa;
        TypedDagNode<int>*                      df;
        
    };
    
}

#endif
