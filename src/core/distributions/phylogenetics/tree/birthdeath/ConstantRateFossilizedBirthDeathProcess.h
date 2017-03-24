#ifndef ConstantRateFossilizedBirthDeathProcess_H
#define ConstantRateFossilizedBirthDeathProcess_H

#include <stdio.h>

#include "AbstractBirthDeathProcess.h"

namespace RevBayesCore {
    
    class Clade;
    class Taxon;

	class ConstantRateFossilizedBirthDeathProcess : public AbstractBirthDeathProcess {
	
	public:
        ConstantRateFossilizedBirthDeathProcess(const TypedDagNode<double> *o,
												const TypedDagNode<double> *s, const TypedDagNode<double> *e,
												const TypedDagNode<double> *p, const TypedDagNode<double> *r,
                                                const bool& uo, const std::string &cdt, const std::vector<Taxon> &tn);
		
		// public member functions
        ConstantRateFossilizedBirthDeathProcess*            clone(void) const;
        virtual double                                      getRootAge(void) const;
        virtual void                                        setValue(Tree *v, bool f=false);                                                                    //!< Set the current value, e.g. attach an observation (clamp)

    protected:
        double                                              computeLnProbabilityDivergenceTimes(void) const;                                //!< Compute the log-transformed probability of the current value.
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                //!< Swap a parameter
        
        // virtual methods that may be overwritten, but then the derived class should call this methods
        virtual void                                        restoreSpecialization(DagNode *restorer);
        virtual void                                        touchSpecialization(DagNode *toucher, bool touchAll);

    private:
        
        // helper functions
        double                                              computeLnProbabilityTimes(void) const;                                          //!< Compute the log-transformed probability of the current value.
        double                                              lnProbNumTaxa(size_t n, double start, double end, bool MRCA) const { throw RbException("Cannot compute P(nTaxa)."); }
        double                                              simulateDivergenceTime(double origin, double present) const;                    //!< Simulate a speciation event.
        double                                              pSurvival(double start, double end) const;                                      //!< Compute the probability of survival of the process (without incomplete taxon sampling).
		double												pZero(double t, double c1, double c2) const;
		double												lnQ(double t, double c1, double c2) const;
		double												pHatZero(double t) const;
		
        // members
        const TypedDagNode<double>*                         lambda;                                                                         //!< The speciation rate.
        const TypedDagNode<double>*                         mu;                                                                             //!< The extinction rate.
        const TypedDagNode<double>*                         psi;                                                                            //!< The sampling probability of a just extinct species.
        const TypedDagNode<double>*                         rho;                                                                            //!< The sampling probability of extant taxa.
        bool                                                useOrigin;
	
	};


}

#endif /* defined(ConstantRateFossilizedBirthDeathProcess_H) */
