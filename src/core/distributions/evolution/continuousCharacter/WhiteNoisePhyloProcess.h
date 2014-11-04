/**
 * @file
 * This file contains the declaration of the WhiteNoisePhyloProcess class.
 * A WhiteNoisePhyloProcess implements a univariate WhiteNoise motion along a tree.
 * this process is sampled only at nodes
 *
 * @brief Declaration of the WhiteNoisePhyloProcess class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-18
 *
 * $Id$
 */


#ifndef WhiteNoisePhyloProcess_H
#define WhiteNoisePhyloProcess_H

#include "RbVector.h"
#include "TimeTree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class WhiteNoisePhyloProcess : public TypedDistribution< RbVector<double> > {
        
    public:
        // constructor(s)
        WhiteNoisePhyloProcess(const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* s);
        
        // public member functions
        WhiteNoisePhyloProcess*                                 clone(void) const;                                                                      //!< Create an independent clone
        double                                                  computeLnProbability(void);
        void                                                    redrawValue(void);
 
        /*
        // special handling of state changes
        void                                                    getAffected(std::set<DagNode *>& affected, DagNode* affecter);                          //!< get affected nodes
        void                                                    keepSpecialization(DagNode* affecter);
        void                                                    restoreSpecialization(DagNode *restorer);
        void                                                    touchSpecialization(DagNode *toucher);
        */
        
    protected:
        // Parameter management functions
        void                                                    swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        // helper methods
        void                                                    simulate();
        double                                                  recursiveLnProb(const TopologyNode& n);
        void                                                    recursiveSimulate(const TopologyNode& n);
        
        // private members
        const TypedDagNode< TimeTree >*                         tau;
        const TypedDagNode< double >*                           sigma;
        
    };
    
}
#endif
