#ifndef MultiRateBirthDeathProcess_H
#define MultiRateBirthDeathProcess_H

#include "AbstractBirthDeathProcess.h"
#include "BiSSE.h"
#include "RateMatrix.h"
#include "Taxon.h"
#include "TimeTree.h"
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
        MultiRateBirthDeathProcess(const TypedDagNode<double> *o, const TypedDagNode<double> *ro,
                            const TypedDagNode<RbVector<double> >* l, const TypedDagNode<RbVector<double> >* m, const TypedDagNode<RateMatrix>* q,
                            const TypedDagNode< RbVector< double > >* p,
                            const TypedDagNode<double> *rh, const std::string &cdt, const std::vector<Taxon> &tn, const std::vector<Clade> &c);
        
        // pure virtual member functions
        virtual MultiRateBirthDeathProcess*                 clone(void) const;                                                                                  //!< Create an independent clone
        
        
    protected:
                
        virtual double                                      computeLnProbabilityTimes(void) const;                                                              //!< Compute the log-transformed probability of the current value.
        void                                                computeNodeProbability(const TopologyNode &n, size_t nIdx) const;
        double                                              computeRootLikelihood() const;
        virtual double                                      pSurvival(double start, double end) const;                                                          //!< Compute the probability of survival of the process (without incomplete taxon sampling).
        virtual std::vector<double>*                        simSpeciations(size_t n, double origin) const;                                                      //!< Simulate n speciation events.
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                                    //!< Swap a parameter

        // members
        
        const TypedDagNode<RbVector<double> >*              lambda;
        const TypedDagNode<RbVector<double> >*              mu;
        const TypedDagNode< RbVector< double > >*           pi;                                                                                                 //!< The root frequencies (probabilities of the root states).
        const TypedDagNode<RateMatrix>*                     Q;
        const TypedDagNode<double>*                         rho;                                                                                                //!< Sampling probability of each species.

        std::vector<size_t>                                 activeLikelihood;
        mutable std::vector<bool>                           changedNodes;
        mutable std::vector<bool>                           dirtyNodes;
        mutable std::vector<std::vector<state_type> >       nodeStates;
        
    };
    
}

#endif
