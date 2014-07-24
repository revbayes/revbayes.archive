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


#include "PrecisionMatrix.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"
#include "MultivariatePhyloProcess.h"

namespace RevBayesCore {
    
    class MultivariateBrownianPhyloProcess : public TypedDistribution<MultivariatePhyloProcess> {
        
    public:
        // constructor(s)
        MultivariateBrownianPhyloProcess(const TypedDagNode< TimeTree > *intau, const TypedDagNode<PrecisionMatrix>* insigma);
        MultivariateBrownianPhyloProcess(const MultivariateBrownianPhyloProcess &from);
        
        // public member functions
        MultivariateBrownianPhyloProcess*                       clone(void) const;
        
        double                                                  computeLnProbability(void);
        size_t                                                  getDim() const {return sigma->getValue().getDim();}
        void                                                    redrawValue(void);
        
        const TypedDagNode< TimeTree >*                         getTimeTree() const {return tau;}
        
        // Parameter management functions
        std::set<const DagNode*>                                getParameters(void) const;                                          //!< Return parameters
        void                                                    swapParameter(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter

        // special handling of state changes
        void                                                    getAffected(std::set<DagNode *>& affected, DagNode* affecter);                          //!< get affected nodes
        void                                                    keepSpecialization(DagNode* affecter);
        void                                                    restoreSpecialization(DagNode *restorer);
        void                                                    touchSpecialization(DagNode *toucher);
        
    private:
        // helper methods
        void                                                    simulate();
        double                                                  recursiveLnProb(const TopologyNode& n);
        void                                                    recursiveSimulate(const TopologyNode& n);

        void                                                    corruptAll();
        void                                                    recursiveCorruptNodes(const TopologyNode& n);

        // private members
        const TypedDagNode< TimeTree >*                         tau;
        const TypedDagNode< PrecisionMatrix >*                  sigma;
        
//        std::vector<bool>                                       changedNodes;
        std::vector<bool>                                       dirtyNodes;
        std::vector<double>                                     nodeLogProbs;
        // double bklnProb;
    };
    
}

#endif /* defined(__revbayes__MultivariateBrownianProcess__) */
