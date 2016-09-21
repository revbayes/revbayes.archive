//
//  ConstantRateOutgroupBirthDeathProcess.h
//  revbayes-proj
//
//  Created by Michael Landis on 5/5/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#ifndef ConstantRateOutgroupBirthDeathProcess_h
#define ConstantRateOutgroupBirthDeathProcess_h

#include "AbstractBirthDeathProcess.h"

namespace RevBayesCore {
    
    class Clade;
    class Taxon;
    
    class ConstantRateOutgroupBirthDeathProcess : public AbstractBirthDeathProcess {
        
    public:
        ConstantRateOutgroupBirthDeathProcess(const TypedDagNode<double> *ra, const TypedDagNode<double> *s, const TypedDagNode<double> *e,
                                               const TypedDagNode<double> *ri, const TypedDagNode<double> *ro, const std::string &cdt, const std::vector<Taxon> &tn);
        
        // public member functions
        ConstantRateOutgroupBirthDeathProcess*            clone(void) const;
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                //!< Swap a parameter
        
    private:
        
        // helper functions
        double                                              computeLnProbabilityTimes(void) const;                                          //!< Compute the log-transformed probability of the current value.
        double                                              lnProbNumTaxa(size_t n, double start, double end, bool MRCA) const { throw RbException("Cannot compute P(nTaxa)."); }
        double                                              simulateDivergenceTime(double origin, double present) const;                    //!< Simulate a speciation event.
        double                                              pSurvival(double start, double end) const;                                      //!< Compute the probability of survival of the process (without incomplete taxon sampling).
        double												pOne(double t, double b, double d, double r) const;
        double												pZero(double t, double b, double d, double r) const;
        
        // members
        const TypedDagNode<double>*                         lambda;                                                                         //!< The speciation rate.
        const TypedDagNode<double>*                         mu;                                                                             //!< The extinction rate.
        const TypedDagNode<double>*                         rhoIngroup;                                                                     //!< The sampling probability of extant taxa.
        const TypedDagNode<double>*                         rhoOutgroup;                                                                    //!< The sampling probability of extant taxa.
        
    };
    
    
}

#endif /* ConstantRateOutgroupBirthDeathProcess_h */
