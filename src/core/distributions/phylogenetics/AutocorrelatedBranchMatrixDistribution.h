/**
 * @file
 * This file contains the declaration of the AutocorrelatedBranchMatrix class. 
 * A AutocorrelatedBranchMatrix object holds the mapping between parameter values and the index of this parameters.
 *
 *
 * @brief Declaration of the AutocorrelatedBranchMatrix class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-18
 *
 * $Id$
 */


#ifndef AutocorrelatedBranchMatrixDistribution_H
#define AutocorrelatedBranchMatrixDistribution_H

#include "RateMatrix_GTR.h"
#include "RbVector.h"
#include "TimeTree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class AutocorrelatedBranchMatrixDistribution : public TypedDistribution< RbVector<RateMatrix> > {
        
    public:
        // constructor(s)
        AutocorrelatedBranchMatrixDistribution(const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* p, const TypedDagNode< std::vector< double > >* rf, const TypedDagNode< std::vector< double > >* er, const TypedDagNode< double >* a);
        AutocorrelatedBranchMatrixDistribution(const AutocorrelatedBranchMatrixDistribution &n);
        
        // public member functions
        AutocorrelatedBranchMatrixDistribution*                 clone(void) const;                                                                      //!< Create an independent clone
        double                                                  computeLnProbability(void);
        void                                                    redrawValue(void);

        // special handling of state changes
//        void                                                    getAffected(std::set<DagNode *>& affected, DagNode* affecter);                          //!< get affected nodes
//        void                                                    keepSpecialization(DagNode* affecter);
//        void                                                    restoreSpecialization(DagNode *restorer);
        void                                                    touchSpecialization(DagNode *toucher);
        
        // Parameter management functions
        std::set<const DagNode*>                                getParameters(void) const;                                          //!< Return parameters
        void                                                    swapParameter(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter

    private:
        // helper methods
        RbVector< RateMatrix >*                                 simulate();
        double                                                  recursiveLnProb(const TopologyNode& n);
        void                                                    recursiveSimulate(const TopologyNode& n, RbVector< RateMatrix > *v, const std::vector< double > &s);
        
        // private members
        const TypedDagNode< TimeTree >*                         tau;
        const TypedDagNode< double >*                           changeProbability;
        const TypedDagNode< std::vector< double > >*            rootFrequencies;
        const TypedDagNode< std::vector< double > >*            exchangeabilityRates;
        const TypedDagNode< double >*                           alpha;                                                                                  //!< Concentration parameter
        
        RbVector<RateMatrix>                                    uniqueMatrices;
        std::vector< std::vector< double > >                    uniqueBaseFrequencies;
        std::vector<size_t>                                     matrixIndex;
    };
    
}
#endif
