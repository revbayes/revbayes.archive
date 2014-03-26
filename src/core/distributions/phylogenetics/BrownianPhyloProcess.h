/**
 * @file
 * This file contains the declaration of the BrownianPhyloProcess class.
 * A BrownianPhyloProcess implements a univariate Brownian motion along a tree.
 * this process is sampled only at nodes
 *
 * @brief Declaration of the BrownianPhyloProcess class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-18
 *
 * $Id$
 */


#ifndef BrownianPhyloProcess_H
#define BrownianPhyloProcess_H

#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class BrownianPhyloProcess : public TypedDistribution< std::vector<double> > {
        
    public:
        // constructor(s)
        BrownianPhyloProcess(const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* s, const TypedDagNode< double >* rr);
        BrownianPhyloProcess(const BrownianPhyloProcess &n);
        
        // public member functions
        BrownianPhyloProcess*                clone(void) const;                                                                      //!< Create an independent clone
        double                                                  computeLnProbability(void);
        void                                                    redrawValue(void);
        void                                                    swapParameter(const DagNode *oldP, const DagNode *newP);                                //!< Implementation of swaping parameters
        
        // special handling of state changes
  /*
        void                                                    getAffected(std::set<DagNode *>& affected, DagNode* affecter);                          //!< get affected nodes
        void                                                    keepSpecialization(DagNode* affecter);
        void                                                    restoreSpecialization(DagNode *restorer);
        void                                                    touchSpecialization(DagNode *toucher);
   */     
        
    private:
        // helper methods
        void                                                    simulate();
        double                                                  recursiveLnProb(const TopologyNode& n);
        void                                                    recursiveSimulate(const TopologyNode& n);
        
        // private members
        const TypedDagNode< TimeTree >*                         tau;
        const TypedDagNode< double >*                           sigma;
        const TypedDagNode< double >*                           rootVal;
        
    };
    
}
#endif
