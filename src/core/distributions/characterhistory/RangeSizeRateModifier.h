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
        RangeSizeRateModifier(unsigned int numAreas, unsigned int stateModified, int sgn=1);
        RangeSizeRateModifier(const RangeSizeRateModifier& m);
        double computeRateModifier(std::vector<CharacterEvent*> currState, CharacterEvent* newState);
        double computeRateModifier(std::vector<CharacterEvent*> currState, int newState);
        RangeSizeRateModifier* clone(void) const;
        
        void updateFrequency(double f);
        void updatePower(double p);
        void update(void);
        
    protected:
        
    private:
        unsigned int numAreas;
        int stateModified;
        double frequency;
        double power;
        int sign;
        double slope; // if slope == 0, then no BD effects...
        double intercept; // set intercept to be max rate
        
    };
}

#endif /* defined(__rb_mlandis__RangeSizeRateModifier__) */
