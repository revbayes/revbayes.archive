/**
 * @file
 * This file contains the declaration of the random variable class for time varinyg rate birth-death process.
 * This class is derived from the stochastic node and each instance will represent a random variable.
 *
 * @brief Declaration of the time varying rate Birth-Death process class.
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

#ifndef TimeDependentBirthDeathProcess_H
#define TimeDependentBirthDeathProcess_H

#include "StepFunction.h"
#include "TimeTree.h"
#include "TrapezoidalRule.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class Clade;
    
    class TimeDependentBirthDeathProcess : public TypedDistribution<TimeTree> {
        
    public:
        TimeDependentBirthDeathProcess(const TypedDagNode<UnivariateFunction> *l, const TypedDagNode<UnivariateFunction> *m, const TypedDagNode<double> *r, unsigned int nTaxa, const std::vector<std::string> &tn, const std::vector<Clade> &c);        
        TimeDependentBirthDeathProcess(const TimeDependentBirthDeathProcess &n);                                                                                    //!< Copy constructor
        virtual                                            ~TimeDependentBirthDeathProcess(void);                                                                 //!< Virtual destructor
        
        // public member functions
        TimeDependentBirthDeathProcess*                     clone(void) const;                                                                                  //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                                            //!< Implementation of swaping parameters
        
    private:
        
        // helper functions
        void                                                attachTimes(TimeTree *psi, std::vector<TopologyNode *> &tips, size_t index, const std::vector<double> times, double T);
        void                                                buildRandomBinaryTree(std::vector<TopologyNode *> &tips);
        double                                              divRate(double t);
        void                                                initialize(void);
        bool                                                matchesConstraints(void);
        double                                              p1(double t, double T) const;
        double                                              pSurvival(double start, double end, double T) const;
        double                                              pWaiting(double start, double end, double T, size_t n) const;
        double                                              simOrigin(void);
        double                                              simSpeciation(double origin);
        void                                                simulateTree(void);
        
        // members
        std::vector<Clade>                                  constraints;
        const TypedDagNode<UnivariateFunction>*             lambda;
        const TypedDagNode<UnivariateFunction>*             mu;
        const TypedDagNode<double>*                         rho;
        unsigned int                                        numTaxa;
        std::vector<std::string>                            taxonNames;
        double                                              logTreeTopologyProb;
        
        
//        UnivariateFunction*                                 diversificationRate;
        StepFunction*                                       diversificationRateIntegral;
        StepFunction*                                       survivalProbIntegral;
        
        bool                                                approximate;
//        double                                              PRESENT;
        bool                                                init;
//        mutable TrapezoidalRule                             integrator;
//        TrapezoidalRule                                     survivingRateIntegration;
//        TrapezoidalRule                                     waitingRateIntegration;
        
    };
    
}

#endif
