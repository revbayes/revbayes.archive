/**
 * @file
 * This file contains the declaration of the AutocorrelatedLognormalRate class. 
 * A AutocorrelatedLognormalRate object holds the mapping between parameter values and the indix of this parameters.
 *
 *
 * @brief Declaration of the AutocorrelatedLognormalRate class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-18
 *
 * $Id$
 */


#ifndef AutocorrelatedLognormalRateBranchwiseVarDistribution_H
#define AutocorrelatedLognormalRateBranchwiseVarDistribution_H

#include "RbVector.h"
#include "TimeTree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class AutocorrelatedLognormalRateBranchwiseVarDistribution : public TypedDistribution< std::vector<double> > {
        
    public:
        // constructor(s)
		AutocorrelatedLognormalRateBranchwiseVarDistribution(const TypedDagNode< TimeTree > *t, const TypedDagNode< std::vector< double > > *s, const TypedDagNode< double >* rr);
		AutocorrelatedLognormalRateBranchwiseVarDistribution(const TypedDagNode< TimeTree > *t, TypedDagNode< std::vector< double > > *s, const TypedDagNode< double >* rr, const TypedDagNode< double >* sv);
		AutocorrelatedLognormalRateBranchwiseVarDistribution(const AutocorrelatedLognormalRateBranchwiseVarDistribution &n);
        
        // public member functions
        AutocorrelatedLognormalRateBranchwiseVarDistribution*                clone(void) const;                                                                      //!< Create an independent clone
        double                                                  computeLnProbability(void);
        void                                                    redrawValue(void);
        
        // special handling of state changes
        void                                                    getAffected(std::set<DagNode *>& affected, DagNode* affecter);                          //!< get affected nodes
        void                                                    keepSpecialization(DagNode* affecter);
        void                                                    restoreSpecialization(DagNode *restorer);
        void                                                    touchSpecialization(DagNode *toucher);

        // Parameter management functions
        std::set<const DagNode*>                                getParameters(void) const;                                          //!< Return parameters
        void                                                    swapParameter(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
        
    private:
        // helper methods
        void                                                    simulate();
        double                                                  recursiveLnProb(const TopologyNode& n);
        void                                                    recursiveSimulate(const TopologyNode& n, double r);
        
        // private members
        const TypedDagNode< TimeTree >*                         tau;
        const TypedDagNode< std::vector< double > >*			sigma;
        const TypedDagNode< double >*                           rootRate;
        const TypedDagNode< double >*                           scaleValue;
        
    };
    
}
#endif
