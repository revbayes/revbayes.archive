#ifndef MultiRateBirthDeathProcess_H
#define MultiRateBirthDeathProcess_H

#include "AbstractBirthDeathProcess.h"
#include "CDSE.h"
#include "RateMatrix.h"
#include "Simplex.h"
#include "Taxon.h"
#include "Tree.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    class Clade;
    
    /**
     * @file
     * This file contains the declaration of the random variable class for multi rate birth-death process.
     * This class is derived from the stochastic node and each instance will represent a random variable.
     *
     * @brief Declaration of the constant rate Birth-Death process class.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2015-03-10, version 1.0
     *
     */
    class MultiRateBirthDeathProcess : public AbstractBirthDeathProcess {
        
    public:
        MultiRateBirthDeathProcess(const TypedDagNode<double> *ro,
                            const TypedDagNode<RbVector<double> >* l, const TypedDagNode<RbVector<double> >* m, const TypedDagNode<RateGenerator>* q,
                            const TypedDagNode< double >* r, const TypedDagNode< Simplex >* p,
                            const TypedDagNode<double> *rh, const std::string &cdt, const std::vector<Taxon> &tn);
        
        // pure virtual member functions
        virtual MultiRateBirthDeathProcess*                 clone(void) const;                                                                                  //!< Create an independent clone
        
        
    protected:
                
        virtual double                                      computeLnProbabilityTimes(void) const;                                                              //!< Compute the log-transformed probability of the current value.
        void                                                computeNodeProbability(const TopologyNode &n, size_t nIdx) const;
        double                                              computeRootLikelihood() const;
        double                                              lnProbNumTaxa(size_t n, double start, double end, bool MRCA) const { throw RbException("Cannot compute P(nTaxa)."); }
        virtual double                                      pSurvival(double start, double end) const;                                                          //!< Compute the probability of survival of the process (without incomplete taxon sampling).
        double                                              simulateDivergenceTime(double origin, double present) const;                                        //!< Simulate a speciation event.
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                                    //!< Swap a parameter

        // members
        
        const TypedDagNode<RbVector<double> >*              lambda;
        const TypedDagNode<RbVector<double> >*              mu;
        const TypedDagNode<Simplex>*                        pi;                                                                                                 //!< The root frequencies (probabilities of the root states).
        const TypedDagNode<RateGenerator>*                  Q;
        const TypedDagNode<double>*                         rate;                                                                                                //!< Sampling probability of each species.
        const TypedDagNode<double>*                         rho;                                                                                                //!< Sampling probability of each species.

        std::vector<size_t>                                 activeLikelihood;
        mutable std::vector<bool>                           changed_nodes;
        mutable std::vector<bool>                           dirty_nodes;
        mutable std::vector<std::vector<state_type> >       nodeStates;
        size_t                                              numRateCategories;
        mutable std::vector<std::vector<double> >           scalingFactors;
        mutable double                                      totalScaling;
    
        const double                                        NUM_TIME_SLICES;
    };
    
}

#endif
