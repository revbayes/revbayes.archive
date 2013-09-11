//
//  CharacterHistoryCtmc.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AbstractCharacterHistoryCtmc__
#define __rb_mlandis__AbstractCharacterHistoryCtmc__

#include "AbstractCharacterHistoryRateModifier.h"
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

        //AbstractCharacterHistoryCtmc(BranchHistory* bh);
        AbstractCharacterHistoryCtmc(BranchHistory* bh, TypedDagNode<RateMatrix> *rateMtx, size_t nc, std::string lbls, std::vector<AbstractCharacterHistoryRateModifier*> rateMods);

        // allows for partial update of history
        void                            setValue(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet, const std::set<CharacterEvent*>& parentSet, const std::set<CharacterEvent*>& childSet, const std::set<size_t>& indexSet );
                
        // inherited pure virtual functions
        void                            redrawValue(void);

        // pure virtual functions
        virtual AbstractCharacterHistoryCtmc*   clone(void) const = 0;
        virtual void                    swapParameter(const DagNode *oldP, const DagNode *newP) = 0;
        virtual double                  computeLnProbability(void) = 0;
        virtual double                  sumOfRates(std::vector<CharacterEvent*> s) = 0;
        virtual double                  transitionRate(std::vector<CharacterEvent*> oldState, CharacterEvent* evt) = 0;
        
    protected:
        TypedDagNode<RateMatrix>* rateMatrix;
        std::vector<AbstractCharacterHistoryRateModifier*> rateModifiers;
        
    private:

        
    };
}

#endif /* defined(__rb_mlandis__AbstractCharacterHistoryCtmc__) */
