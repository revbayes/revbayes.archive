//
//  MultivariateBrownianProcess.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-26.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__MultivariateBrownianPhyloProcess__
#define __revbayes__MultivariateBrownianPhyloProcess__

#include <iostream>


#include "MatrixRealSymmetric.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"
#include "MultivariateRealNodeContainer.h"

namespace RevBayesCore {
    
    class MultivariateBrownianPhyloProcess : public TypedDistribution<MultivariateRealNodeContainer> {
        
    public:
        // constructor(s)
        MultivariateBrownianPhyloProcess(const TypedDagNode< TimeTree > *intau, const TypedDagNode<MatrixRealSymmetric>* insigma);
        MultivariateBrownianPhyloProcess(const MultivariateBrownianPhyloProcess &from);
        
        // public member functions
        MultivariateBrownianPhyloProcess*                       clone(void) const;
        
        double                                                  computeLnProbability(void);
        size_t                                                  getDim() const {return sigma->getValue().getDim();}
        void                                                    redrawValue(void);
        
        const TimeTree*                         getTimeTree() const {return &tau->getValue();}
        
        // Parameter management functions
        std::set<const DagNode*>                                getParameters(void) const;                                          //!< Return parameters
        void                                                    swapParameter(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter

        // special handling of state changes
        void                                                    keepSpecialization(DagNode* affecter);
        void                                                    restoreSpecialization(DagNode *restorer);
        void                                                    touchSpecialization(DagNode *toucher);
        
    private:
        // helper methods
        void                                                    simulate();
        double                                                  recursiveLnProb(const TopologyNode& n);
        void                                                    recursiveSimulate(const TopologyNode& n);

        // special handling of state changes
        void                                                    flagNodes();        
        void                                                    corruptAll();
        void                                                    recursiveCorruptAll(const TopologyNode& n);
        
        // private members
        const TypedDagNode< TimeTree >*                         tau;
        const TypedDagNode< MatrixRealSymmetric >*              sigma;
        
        std::vector<bool>                                       dirtyNodes;
        std::vector<double>                                     nodeLogProbs;
    };
    
}

#endif /* defined(__revbayes__MultivariateBrownianProcess__) */
