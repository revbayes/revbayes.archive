/**
 * @file
 * This file contains the declaration of the random variable class for constant rate birth-death process.
 * This class is derived from the stochastic node and each instance will represent a random variable.
 *
 * @brief Declaration of the constant rate Birth-Death process class.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-06-17, version 1.0
 * @interface TypedDagNode
 *
 * $Id:$
 */

#ifndef BirthDeathProcess_H
#define BirthDeathProcess_H

#include "TimeTree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class Clade;
    
    class BirthDeathProcess : public TypedDistribution<TimeTree> {
        
    public:
        BirthDeathProcess(const TypedDagNode<double> *o, 
                                      const TypedDagNode<double> *r, const std::string& ss, const std::string &cdt, 
                                      unsigned int nTaxa, const std::vector<std::string> &tn, const std::vector<Clade> &c);        
        
        // pure virtual member functions
        virtual BirthDeathProcess*                          clone(void) const = 0;                                                                                  //!< Create an independent clone
        
        
        // public member functions
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        virtual void                                        swapParameter(const DagNode *oldP, const DagNode *newP);                                            //!< Implementation of swaping parameters
        
    protected:
        // pure virtual helper functions
        virtual double                                      lnSpeciationRate(double t) const = 0;
        virtual double                                      rateIntegral(double t_low, double t_high) const = 0;
        virtual double                                      simSpeciation(double origin, double r) = 0;
        virtual double                                      pSurvival(double start, double end, double T, double r) const = 0;

        
        // helper functions
        void                                                attachTimes(TimeTree *psi, std::vector<TopologyNode *> &tips, size_t index, const std::vector<double> &times, double T);
        void                                                buildRandomBinaryTree(std::vector<TopologyNode *> &tips);
        double                                              lnP1(double t, double T, double r) const;
        bool                                                matchesConstraints(void);
        void                                                simulateTree(void);
        
        // members
        std::string                                         condition;
        std::vector<Clade>                                  constraints;
        const TypedDagNode<double>*                         origin;
        const TypedDagNode<double>*                         rho;
        unsigned int                                        numTaxa;
        std::string                                         samplingStrategy;
        std::vector<std::string>                            taxonNames;
        double                                              logTreeTopologyProb;
        
    };
    
}

#endif
