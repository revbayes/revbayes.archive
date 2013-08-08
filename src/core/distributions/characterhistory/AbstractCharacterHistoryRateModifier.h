//
//  AbstractCharacterHistoryRateModifier.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/8/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AbstractCharacterHistoryRateModifier__
#define __rb_mlandis__AbstractCharacterHistoryRateModifier__

#include <set>
#include <vector>

namespace RevBayesCore
{
    class CharacterEvent;
    class AbstractCharacterHistoryRateModifier
    {
    public:
        AbstractCharacterHistoryRateModifier(void);
        
    protected:
        virtual double computeDistanceModifier(std::vector<CharacterEvent*> startingState, std::set<CharacterEvent*> stateChange) = 0;
        
    private:
        
        
    };
    
};

#endif /* defined(__rb_mlandis__AbstractCharacterHistoryRateModifier__) */
