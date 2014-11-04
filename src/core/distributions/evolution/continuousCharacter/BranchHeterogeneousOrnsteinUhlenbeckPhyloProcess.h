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
        
        // public member functions
        BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess*       clone(void) const;                                                                      //!< Create an independent clone
        double                                                  computeLnProbability(void);
        void                                                    redrawValue(void);
        
        void                                                    setMean( const TypedDagNode< double >* x);
        void                                                    setMean( const TypedDagNode< RbVector< double > >* x);
        void                                                    setPhi( const TypedDagNode< double >* x);
        void                                                    setPhi( const TypedDagNode< RbVector< double > >* x);
        void                                                    setSigma( const TypedDagNode< double >* x);
        void                                                    setSigma( const TypedDagNode< RbVector< double > >* x);
        void                                                    setRootMean( const TypedDagNode< double >* x);
        void                                                    setRootStdDev( const TypedDagNode< double >* x);

    protected:
        // Parameter management functions
        void                                                    swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Swap a parameter

    private:
        // helper methods
        void                                                    simulate();
        double                                                  recursiveLnProb(const TopologyNode& n);
        void                                                    recursiveSimulate(const TopologyNode& n);
        
        // private members
        const TypedDagNode< TimeTree >*                         tau;
        const TypedDagNode< RbVector< double > >*               heterogeneousMean;
        const TypedDagNode< RbVector< double > >*               heterogeneousPhi;
        const TypedDagNode< RbVector< double > >*               heterogeneousSigma;
        const TypedDagNode< double >*                           homogeneousMean;
        const TypedDagNode< double >*                           homogeneousPhi;
        const TypedDagNode< double >*                           homogeneousSigma;
        const TypedDagNode< double >*                           rootMean;
        const TypedDagNode< double >*                           rootStdDev;

    };
    
}


#endif	/* BRANCHHETEROGENEOUSORNSTEINUHLENBECKPHYLOPROCESS_H */

