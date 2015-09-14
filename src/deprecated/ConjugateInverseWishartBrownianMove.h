/* 
 * File:   ConjugateInverseWishartBrownianMove.h
 * Author: nl
 *
 * Created on 23 juillet 2014, 16:48
 */


#ifndef BROWNIANINVERSEWISHARTMIXINGMOVE_H
#define	BROWNIANINVERSEWISHARTMIXINGMOVE_H

#include <map>
#include <ostream>
#include <set>
#include <string>

#include "StochasticNode.h"

#include "PhyloMultivariateBrownianProcess.h"
#include "InverseWishartDistribution.h"
#include "TimeTree.h"

#include "CompoundMove.h"

namespace RevBayesCore {
    
    
    class ConjugateInverseWishartBrownianMove : public CompoundMove {
        
    public:
        ConjugateInverseWishartBrownianMove(StochasticNode<MatrixRealSymmetric>* s, StochasticNode< RbVector< RbVector<double> > >* p, TypedDagNode<double>* k, TypedDagNode<int>* d, double w);                         //!< Constructor
//        ConjugateInverseWishartBrownianMove(StochasticNode<MatrixRealSymmetric>* s, StochasticNode<MultivariateRealNodeContainer>* p, StochasticNode<TimeTree>* t, TypedDagNode<double>* k, TypedDagNode<int>* d, double w);                         //!< Constructor
        
        // Basic utility functions
        ConjugateInverseWishartBrownianMove*         clone(void) const;                                                                  //!< Clone this object.
        const std::string&                          getMoveName(void) const;                                                            //!< Get the name of the move for summary printing.
        void                                        swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the variable if it was replaced.
        
    protected:
        
        double                                  performCompoundMove(void);                                                            //!< Perform move
        void                                    printParameterSummary(std::ostream &o) const;
        void                                    rejectCompoundMove(void);
        void                                    acceptCompoundMove(void);
        
    private:
        
        StochasticNode<MatrixRealSymmetric>*                sigma;
        StochasticNode< RbVector< RbVector<double> > >*     process;
        // StochasticNode<TimeTree>*                       tau;
        TypedDagNode<double>*                               kappa;
        TypedDagNode<int>*                                  df;
        
        MatrixRealSymmetric                                 bksigma;
        
    };
    
}

#endif	/* BROWNIANINVERSEWISHARTMIXINGMOVE_H */

