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

namespace RevBayesCore
{
    class AreaHistoryCtmc : public AbstractCharacterHistoryCtmc
    {
    
    public:
        AreaHistoryCtmc();
        double                  sumOfRates(std::vector<CharacterEvent*> s);
        double                  transitionRate(std::vector<CharacterEvent*> s, CharacterEvent* evt);
        
    protected:

    private:
        
    };
};

#endif /* defined(__rb_mlandis__AreaHistoryCtmc__) */
