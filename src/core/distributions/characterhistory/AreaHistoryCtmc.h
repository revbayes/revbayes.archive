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
        //AreaHistoryCtmc* ahc = new AreaHistoryCtmc(bh, q, numStates, numChars, rateMods);
        AreaHistoryCtmc(BranchHistory* bh, TypedDagNode<RateMatrix> *rm, size_t ns, size_t nc, TypedDagNode<std::vector<AbstractCharacterHistoryRateModifier*> >* rateMods);
        AreaHistoryCtmc(TypedDagNode<RateMatrix> *rm, size_t ns, size_t nc, TypedDagNode<std::vector<AbstractCharacterHistoryRateModifier*> >* rateMods);
        AreaHistoryCtmc*        clone(void) const;
        double                  sumOfRates(std::vector<CharacterEvent*> curState);
        double                  transitionRate(std::vector<CharacterEvent*> curState, CharacterEvent* evt);
        void                    swapParameter(const DagNode *oldP, const DagNode *newP);
        double                  computeLnProbability(void);
        void                    redrawValue(void);
        
    protected:

    private:

    };
}

#endif /* defined(__rb_mlandis__AreaHistoryCtmc__) */
