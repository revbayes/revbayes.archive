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

#include "MultivariateBrownianPhyloProcess.h"
#include "InverseWishartDistribution.h"
#include "TimeTree.h"

#include "CompoundMove.h"

namespace RevBayesCore {
    
    
    class ConjugateInverseWishartBrownianMove : public CompoundMove {
        
    public:
        ConjugateInverseWishartBrownianMove(StochasticNode<PrecisionMatrix>* s, StochasticNode<MultivariatePhyloProcess>* p, TypedDagNode<double>* k, TypedDagNode<int>* d, double w);                         //!< Constructor
        
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
        
        StochasticNode<PrecisionMatrix>*            sigma;
        StochasticNode<MultivariatePhyloProcess>*   process;
        TypedDagNode<double>*                       kappa;
        TypedDagNode<int>*                          df;
        
        PrecisionMatrix                             bksigma;
        
    };
    
}

#endif	/* BROWNIANINVERSEWISHARTMIXINGMOVE_H */

