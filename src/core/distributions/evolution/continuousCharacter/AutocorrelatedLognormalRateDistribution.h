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


#ifndef AutocorrelatedLognormalRateDistribution_H
#define AutocorrelatedLognormalRateDistribution_H

#include "RbVector.h"
#include "TimeTree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class AutocorrelatedLognormalRateDistribution : public TypedDistribution< RbVector<double> > {
        
    public:
        // constructor(s)
        AutocorrelatedLognormalRateDistribution(const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* s, const TypedDagNode< double >* rr);
        AutocorrelatedLognormalRateDistribution(const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* s, const TypedDagNode< double >* rr, const TypedDagNode< double >* sv);
        
        // public member functions
        AutocorrelatedLognormalRateDistribution*                clone(void) const;                                                                      //!< Create an independent clone
        double                                                  computeLnProbability(void);
        void                                                    redrawValue(void);
        
        // special handling of state changes
        void                                                    getAffected(std::set<DagNode *>& affected, DagNode* affecter);                          //!< get affected nodes
        void                                                    keepSpecialization(DagNode* affecter);
        void                                                    restoreSpecialization(DagNode *restorer);
        void                                                    touchSpecialization(DagNode *toucher);
        
    protected:
        // Parameter management functions
        void                                                    swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Swap a parameter
        
    private:
        // helper methods
        void                                                    simulate();
        double                                                  recursiveLnProb(const TopologyNode& n);
        void                                                    recursiveSimulate(const TopologyNode& n, double r);
        
        // private members
        const TypedDagNode< TimeTree >*                         tau;
        const TypedDagNode< double >*                           sigma;
        const TypedDagNode< double >*                           rootRate;
        const TypedDagNode< double >*                           scaleValue;
        
    };
    
}
#endif
