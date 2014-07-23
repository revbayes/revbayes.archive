/* 
 * File:   BrownianInverseWishartSlidingMove.h
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

#include "CompoundMove.h"
#include "StochasticNode.h"

#include "MultivariateBrownianPhyloProcess.h"
#include "InverseWishartDistribution.h"



namespace RevBayesCore {
    
    class BrownianInverseWishartSlidingMove : public CompoundMove {
        
    public:
        BrownianInverseWishartSlidingMove( std::vector<DagNode*> n, double l, bool t, double w);                                                         //!<  constructor
        // BrownianInverseWishartSlidingMove( StochasticNode<MultivariatePhyloProcess>* p, StochasticNode<PrecisionMatrix>* s, double l, bool t, double w);                                                         //!<  constructor
        
        // Basic utility functions
        BrownianInverseWishartSlidingMove*                  clone(void) const;                                                                  //!< Clone object
        void                                    swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                                  performCompoundMove(void);                                                            //!< Perform move
        void                                    printParameterSummary(std::ostream &o) const;
        void                                    rejectCompoundMove(void);
        void                                    tune(void);
        
    private:
        
        // member variables
        StochasticNode<MultivariatePhyloProcess>*           process;
        StochasticNode<PrecisionMatrix>*                    sigma;                         
        StochasticNode<double>*                             kappa;                         

        double                                      lambda;                                                                             //!< The Sliding parameter of the move (larger lambda -> larger proposals).

        // stored objects to undo proposal
        MultivariatePhyloProcess                    bkprocess;                                                                        //!< The stored value of the last modified element.
        PrecisionMatrix                             bksigma;
        
    };
    
}


#endif	/* BROWNIANINVERSEWISHARTMIXINGMOVE_H */

