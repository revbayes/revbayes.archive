#ifndef BirthDeathProcess_H
#define BirthDeathProcess_H

#include "TimeTree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class Clade;
    
    /**
     * @file
     * This file contains the declaration of the random variable class for constant rate birth-death process.
     * This class is derived from the stochastic node and each instance will represent a random variable.
     *
     * @brief Declaration of the constant rate Birth-Death process class.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-01-17, version 1.0
     *
     */
    class BirthDeathProcess : public TypedDistribution<TimeTree> {
        
    public:
        BirthDeathProcess(const TypedDagNode<double> *o, 
                                      const TypedDagNode<double> *r, const std::string& ss, const std::string &cdt, 
                                      unsigned int nTaxa, const std::vector<std::string> &tn, const std::vector<Clade> &c);        
        
        // pure virtual member functions
        virtual BirthDeathProcess*                          clone(void) const = 0;                                                                              //!< Create an independent clone
        
        
        // public member functions
        double                                              computeLnProbability(void);                                                                         //!< Compute the log-transformed probability of the current value.
        void                                                redrawValue(void);                                                                                  //!< Redraw a new value under the current parameters.
        
        // public member functions you may want to overwrite
        virtual void                                        swapParameter(const DagNode *oldP, const DagNode *newP);                                            //!< Implementation of swaping parameters
        
    protected:
        // pure virtual helper functions
        virtual double                                      lnSpeciationRate(double t) const = 0;                                                               //!< Get the log-transformed speciation rate at time t.
        virtual double                                      rateIntegral(double t_low, double t_high) const = 0;                                                //!< Compute the rate integral.
        virtual std::vector<double>                         simSpeciations(size_t n, double origin, double r) const = 0;                                        //!< Simulate n speciation events.
        virtual double                                      pSurvival(double start, double end) const = 0;                                                      //!< Compute the probability of survival of the process (without incomplete taxon sampling).

        
        // helper functions
        void                                                attachTimes(TimeTree *psi, std::vector<TopologyNode *> &tips, size_t index, 
                                                                        const std::vector<double> &times, double T);
        void                                                buildRandomBinaryTree(std::vector<TopologyNode *> &tips);
        std::vector<double>                                 divergenceTimesSinceOrigin(void) const;                                                             //!< Extract the divergence times from the tree.
        int                                                 diversity(double t) const;                                                                          //!< Diversity at time t.
        double                                              lnP1(double t, double T, double r) const;
        bool                                                matchesConstraints(void);
        double                                              pSurvival(double start, double end, double r) const;                                                //!< Compute the probability of survival of the process including uniform taxon sampling.
        void                                                simulateTree(void);
        
        // members
        std::string                                         condition;                                                                                          //!< The condition of the process (none/survival/#taxa).
        std::vector<Clade>                                  constraints;                                                                                        //!< Topological constrains.
        const TypedDagNode<double>*                         origin;                                                                                             //!< Time since the origin.
        const TypedDagNode<double>*                         rho;                                                                                                //!< Sampling probability of each species.
        unsigned int                                        numTaxa;                                                                                            //!< Number of taxa (needed for correct initialization).
        std::string                                         samplingStrategy;                                                                                   //!< The incomplete taxon sampling strategy (uniform/diversified).
        std::vector<std::string>                            taxonNames;                                                                                         //!< Taxon names that will be attached to new simulated trees.
        double                                              logTreeTopologyProb;                                                                                //!< Log-transformed tree topology probability (combinatorial constant).
        
    };
    
}

#endif
