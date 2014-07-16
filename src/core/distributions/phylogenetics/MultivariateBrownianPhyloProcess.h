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
        // MultivariateBrownianPhyloProcess(const TypedDagNode< TimeTree > *intau, const TypedDagNode<PrecisionMatrix>* insigma, const TypedDagNode< std::vector<double> >* inrootval);
        MultivariateBrownianPhyloProcess(const TypedDagNode< TimeTree > *intau, const TypedDagNode<PrecisionMatrix>* insigma);
        MultivariateBrownianPhyloProcess(const MultivariateBrownianPhyloProcess &from);
        
        // public member functions
        MultivariateBrownianPhyloProcess*                       clone(void) const;
        
        double                                                  computeLnProbability(void);
        void                                                    redrawValue(void);
        void                                                    swapParameter(const DagNode *oldP, const DagNode *newP);                                //!< Implementation of swaping parameters
        size_t                                                  getDim() {return sigma->getValue().getDim();}
        
        double                                                  getMean(int k);
        double                                                  getStdev(int k);
        void                                                    recursiveGetStats(int k, const TopologyNode& from, double& e1, double& e2, int& n);
        const TypedDagNode< TimeTree >*                         getTree() {return tau;}
        
    private:
        // helper methods
        void                                                    simulate();
        double                                                  recursiveLnProb(const TopologyNode& n);
        void                                                    recursiveSimulate(const TopologyNode& n);
        
        // private members
        const TypedDagNode< TimeTree >*                         tau;
        const TypedDagNode< PrecisionMatrix >*                  sigma;
//      const TypedDagNode< std::vector<double> >*              rootVal;
        
    };
    
}

#endif /* defined(__revbayes__MultivariateBrownianProcess__) */
