//
//  ConstantRateFossilizedBirthDeathProcess.h
//
//  Created by Tracy Heath on 8/31/15.
//  Copyright (c) 2015 Tracy Heath. All rights reserved.
//

#ifndef ConstantRateFossilizedBirthDeathProcess_H
#define ConstantRateFossilizedBirthDeathProcess_H

#include <stdio.h>

#include "AbstractBirthDeathProcess.h"

namespace RevBayesCore {
    
    class Clade;
    class Taxon;

	class ConstantRateFossilizedBirthDeathProcess : public AbstractBirthDeathProcess {
	
	public:
        ConstantRateFossilizedBirthDeathProcess(const TypedDagNode<double> *o, const TypedDagNode<double> *ra,
												const TypedDagNode<double> *s, const TypedDagNode<double> *e,
												const TypedDagNode<double> *p, const TypedDagNode<double> *r,
                                                const std::string &cdt, const std::vector<Taxon> &tn);
		
		// public member functions
        ConstantRateFossilizedBirthDeathProcess*            clone(void) const;

    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                //!< Swap a parameter
        
    private:
        
        // helper functions
        double                                              computeLnProbabilityTimes(void) const;                                          //!< Compute the log-transformed probability of the current value.
        std::vector<double>*                                simSpeciations(size_t n, double origin) const;                                  //!< Simulate n speciation events.
        double                                              pSurvival(double start, double end) const;                                      //!< Compute the probability of survival of the process (without incomplete taxon sampling).
		double												pZero(double t) const;
		double												lnQ(double t) const;
		double												pHatZero(double t) const;
		
        // members
        const TypedDagNode<double>*                         lambda;                                                                         //!< The speciation rate.
        const TypedDagNode<double>*                         mu;                                                                             //!< The extinction rate.
        const TypedDagNode<double>*                         psi;                                                                            //!< The sampling probability of a just extinct species.
        const TypedDagNode<double>*                         rho;                                                                            //!< The sampling probability of extant taxa.
	
	};


}

#endif /* defined(ConstantRateFossilizedBirthDeathProcess_H) */
