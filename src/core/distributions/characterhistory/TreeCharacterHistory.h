//
//  TreeCharacterHistory.h
//  rb_mlandis
//
//  Created by Michael Landis on 9/26/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__TreeCharacterHistory__
#define __rb_mlandis__TreeCharacterHistory__

#include "TypedDistribution.h"
#include "BranchHistory.h"

#include <set>
#include <iostream>

namespace RevBayesCore
{
    class TreeCharacterHistory : public TypedDistribution<BranchHistory>
    {
        
    public:

        TreeCharacterHistory(std::vector<const TypedDagNode<BranchHistory>* > bh, const TypedDagNode<TimeTree>* tr, size_t ns, size_t nc);
        TreeCharacterHistory(const TreeCharacterHistory& m);
        
        // inherited pure virtual functions
        virtual double                                          computeLnProbability(void);
        virtual TreeCharacterHistory*                           clone(void) const;
        virtual void                                            redrawValue(void);
        virtual void                                            simulate(void);
        virtual void                                            swapParameter(const DagNode *oldP, const DagNode *newP);
        
    protected:
        std::vector<const TypedDagNode<BranchHistory>* >        branchHistories;
        const TypedDagNode<TimeTree>*                           tree;
        size_t                                                  numStates;
        size_t                                                  numCharacters;
        std::vector<bool>                                       dirtyBranch; // instead, perhaps t/k/r specialization needed?
        
    private:
        
    };
}

#endif /* defined(__rb_mlandis__TreeCharacterHistory__) */
