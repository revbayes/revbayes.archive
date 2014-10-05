/*
 * File:   BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess.h
 * Author: Bastien Boussau
 *
 * Created on 04/10/2014
 */

#ifndef BRANCHHETEROGENEOUSORNSTEINUHLENBECKPHYLOPROCESS_H
#define	BRANCHHETEROGENEOUSORNSTEINUHLENBECKPHYLOPROCESS_H

#include "RbVector.h"
#include "RealNodeContainer.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess : public TypedDistribution< RealNodeContainer > {
        
    public:
        // constructor(s)
        BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess(const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* s, const TypedDagNode< double >* m, const TypedDagNode< double >* p);
        BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess(const TypedDagNode< TimeTree > *t);
        BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess(const BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess &n);
        
        // public member functions
        BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess*                clone(void) const;                                                                      //!< Create an independent clone
        double                                                  computeLnProbability(void);
        void                                                    redrawValue(void);
        
        // Parameter management functions
        std::set<const DagNode*>                                getParameters(void) const;                                          //!< Return parameters
        void                                                    swapParameter(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
        void setSigma( const TypedDagNode< double >* x);
        void setMean( const TypedDagNode< double >* x);
        void setPhi( const TypedDagNode< double >* x);
        void setSigma( const TypedDagNode< std::vector< double > >* x);
        void setMean( const TypedDagNode< std::vector< double > >* x);
        void setPhi( const TypedDagNode< std::vector< double > >* x);
        void setRootMean( const TypedDagNode< double >* x);
        void setRootStdDev( const TypedDagNode< double >* x);

    private:
        // helper methods
        void                                                    simulate();
        double                                                  recursiveLnProb(const TopologyNode& n);
        void                                                    recursiveSimulate(const TopologyNode& n);
        
        // private members
        const TypedDagNode< TimeTree >*                         tau;
        const TypedDagNode< double >*                           homogeneousSigma;
        const TypedDagNode< double >*                           homogeneousMean;
        const TypedDagNode< double >*                           homogeneousPhi;
        const TypedDagNode< std::vector< double > >*               heterogeneousSigma;
        const TypedDagNode< std::vector< double > >*               heterogeneousMean;
        const TypedDagNode< std::vector< double > >*               heterogeneousPhi;
        const TypedDagNode< double >*                           rootMean;
        const TypedDagNode< double >*                           rootStdDev;

    };
    
}


#endif	/* BRANCHHETEROGENEOUSORNSTEINUHLENBECKPHYLOPROCESS_H */

