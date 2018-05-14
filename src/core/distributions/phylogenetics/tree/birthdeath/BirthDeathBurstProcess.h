#ifndef BirthDeathBurstProcess_H
#define BirthDeathBurstProcess_H

#include "AbstractBirthDeathProcess.h"

namespace RevBayesCore {
    
    class Clade;
    class Taxon;
    
    class BirthDeathBurstProcess : public AbstractBirthDeathProcess {
        
    public:
        BirthDeathBurstProcess(const TypedDagNode<double> *ra,
                               const TypedDagNode<double> *s, const TypedDagNode<double> *e,
                               const TypedDagNode<double> *b, const TypedDagNode<double> *bt, const TypedDagNode<double> *r,
                               const std::string &cdt, const std::vector<Taxon> &tn);
        
        // public member functions
        BirthDeathBurstProcess*         clone(void) const;
        
    protected:
        double                                              computeLnProbabilityDivergenceTimes(void) const;                                //!< Compute the log-transformed probability of the current value.
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                //!< Swap a parameter
        
        // helper functions
        double                                              computeLnProbabilityTimes(void) const;                                          //!< Compute the log-transformed probability of the current value.
        double                                              lnProbNumTaxa(size_t n, double start, double end, bool MRCA) const { throw RbException("Cannot compute P(nTaxa)."); }
        double                                              lnProbTreeShape(void) const;
        double                                              simulateDivergenceTime(double origin, double present) const;                    //!< Simulate a speciation event.
        double                                              pSurvival(double start, double end) const;                                      //!< Compute the probability of survival of the process (without incomplete taxon sampling).
//        double                                              pZero(double t, double c1, double c2) const;
        double                                              lnQ(double t) const;
        double                                              pZero(double t) const;
        
        // members
        const TypedDagNode<double>*                         lambda;                                                                         //!< The speciation rate.
        const TypedDagNode<double>*                         mu;                                                                             //!< The extinction rate.
        const TypedDagNode<double>*                         beta;                                                                           //!< The speciation probability of at the burst time
        const TypedDagNode<double>*                         time_burst;                                                                     //!< The of the burst event
        const TypedDagNode<double>*                         rho;                                                                            //!< The sampling probability of extant taxa.
        
        std::vector<bool>                                   lineage_bursted_at_event;                                                       //!< Did the lineage burst at this event?
    };
    
    
}

#endif /* defined(BirthDeathBurstProcess_H) */
