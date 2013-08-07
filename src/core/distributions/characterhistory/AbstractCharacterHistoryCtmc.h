//
//  CharacterHistoryCtmc.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AbstractCharacterHistoryCtmc__
#define __rb_mlandis__AbstractCharacterHistoryCtmc__

#include "TypedDistribution.h"
#include "BranchHistory.h"
#include "RateMatrix.h"

#include <set>
#include <iostream>

namespace RevBayesCore
{    
    class AbstractCharacterHistoryCtmc : public TypedDistribution<BranchHistory>
    {
        
    public:

        AbstractCharacterHistoryCtmc(BranchHistory* bh, const TypedDagNode<RateMatrix> *rm, size_t nc, std::string lbls);
        // allows for partial update of history
        void                            setValue(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet, const std::set<CharacterEvent*>& parentSet, const std::set<CharacterEvent*>& childSet, const std::set<size_t>& indexSet );
                
        // inherited pure virtual functions
        void                            redrawValue(void);
        void                            setRateModifiers(const std::vector< void (*)() >& vfp);
        
        // pure virtual functions
        AbstractCharacterHistoryCtmc*   clone(void) const = 0;
        virtual void                    swapParameter(const DagNode *oldP, const DagNode *newP) = 0;
        virtual double                  computeLnProbability(void) = 0;
        virtual double                  sumOfRates(std::vector<CharacterEvent*> s) = 0;
        virtual double                  transitionRate(std::vector<CharacterEvent*> oldState, std::multiset<CharacterEvent*> newState);
        
    protected:
        
    private:
        const TypedDagNode<RateMatrix>* rateMatrix;
        std::vector< void (*)() >       rateModifiers;
        
        
    };
};

#endif /* defined(__rb_mlandis__AbstractCharacterHistoryCtmc__) */
