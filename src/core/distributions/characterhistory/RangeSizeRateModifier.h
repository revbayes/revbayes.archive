//
//  RangeSizeRateModifier.h
//  rb_mlandis
//
//  Created by Michael Landis on 10/18/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__RangeSizeRateModifier__
#define __rb_mlandis__RangeSizeRateModifier__

#include "AbstractCharacterHistoryRateModifier.h"
#include "StochasticNode.h"
#include <string>

namespace RevBayesCore
{
    class RangeSizeRateModifier : public AbstractCharacterHistoryRateModifier
    {
    public:
        RangeSizeRateModifier(unsigned int numAreas, unsigned int stateModified);
        double computeRateModifier(std::vector<CharacterEvent*> curState, CharacterEvent* newState);
        RangeSizeRateModifier* clone(void) const;
        
        void updateFrequency(double f);
        void updatePower(double p);
        void update(void);
        
    protected:
        
    private:
        int numAreas;
        int stateModified;
        double frequency;
        double power;
        
    };
}

#endif /* defined(__rb_mlandis__RangeSizeRateModifier__) */
