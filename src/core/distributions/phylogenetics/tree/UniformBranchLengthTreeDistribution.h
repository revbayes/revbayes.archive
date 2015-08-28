//
//  UniformBranchLengthTreeDistribution.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/17/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__UniformBranchLengthTreeDistribution__
#define __rb_mlandis__UniformBranchLengthTreeDistribution__

#include "BranchLengthTree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class Clade;
    
    class UniformBranchLengthTreeDistribution : public TypedDistribution<BranchLengthTree> {
        
    public:
        UniformBranchLengthTreeDistribution(
                                    const TypedDagNode<double>*                 maxBrLen,
                                    const std::vector<std::string>&             taxaNames
                                    );                                                                                  //!< Constructor
        
        virtual                                            ~UniformBranchLengthTreeDistribution(void);                  //!< Virtual destructor
        
        // public member functions
        UniformBranchLengthTreeDistribution*                clone(void) const;                                          //!< Create an independent clone
        double                                              computeLnProbability(void);                                 //!< Compute ln prob of current value
        void                                                redrawValue(void);                                          //!< Draw a new random value from distribution
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // helper functions
        void                                                buildRandomBinaryHistory(std::vector<TopologyNode *> &tips);
        void                                                simulateTree(void);
        
        // members
        const TypedDagNode<double>*                         maxBranchLength;
        size_t                                              numTaxa;
        std::vector<std::string>                            taxonNames;
    };
    
}

#endif /* defined(__rb_mlandis__UniformBranchLengthTreeDistribution__) */
