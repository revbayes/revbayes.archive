//
//  AreaHistoryCtmc.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AreaHistoryCtmc__
#define __rb_mlandis__AreaHistoryCtmc__

#include "AbstractCharacterHistoryCtmc.h"
#include "AbstractCharacterHistoryRateModifier.h"

namespace RevBayesCore
{
    class AreaHistoryCtmc : public AbstractCharacterHistoryCtmc
    {
    
    public:
        AreaHistoryCtmc(BranchHistory* bh, const TypedDagNode<RateMatrix> *rateMtx, size_t nc, std::string lbls, std::vector<AbstractCharacterHistoryRateModifier*> rateMods);
        //AreaHistoryCtmc(BranchHistory* bh);
        AreaHistoryCtmc*        clone(void) const;
        double                  sumOfRates(std::vector<CharacterEvent*> s);
        double                  transitionRate(std::vector<CharacterEvent*> s, CharacterEvent* evt);
        void                    swapParameter(const DagNode *oldP, const DagNode *newP);
        double                  computeLnProbability(void);
    protected:

    private:
        
    };
};

#endif /* defined(__rb_mlandis__AreaHistoryCtmc__) */
