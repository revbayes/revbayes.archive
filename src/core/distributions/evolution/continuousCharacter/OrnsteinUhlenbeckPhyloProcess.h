/* 
 * File:   OrnsteinUhlenbeckPhyloProcess.h
 * Author: nl
 *
 * Created on 26 juillet 2014, 15:03
 */

#ifndef ORNSTEINUHLENBECKPHYLOPROCESS_H
#define	ORNSTEINUHLENBECKPHYLOPROCESS_H

#include "RbVector.h"
#include "RealNodeContainer.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class OrnsteinUhlenbeckPhyloProcess : public TypedDistribution< RealNodeContainer > {
        
    public:
        // constructor(s)
        OrnsteinUhlenbeckPhyloProcess(const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* s, const TypedDagNode< double >* m, const TypedDagNode< double >* p);
        
        // public member functions
        OrnsteinUhlenbeckPhyloProcess*                          clone(void) const;                                                                      //!< Create an independent clone
        double                                                  computeLnProbability(void);
        void                                                    redrawValue(void);
        
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
        const TypedDagNode< double >*                           sigma;
        const TypedDagNode< double >*                           mean;
        const TypedDagNode< double >*                           phi;
        
    };
    
}


#endif	/* ORNSTEINUHLENBECKPHYLOPROCESS_H */

