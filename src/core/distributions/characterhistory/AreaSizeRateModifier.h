//
//  AreaSizeRateModifier.h
//  rb_mlandis
//
//  Created by Michael Landis on 10/25/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AreaSizeRateModifier__
#define __rb_mlandis__AreaSizeRateModifier__

#include "AbstractCharacterHistoryRateModifier.h"
#include "StochasticNode.h"
#include <string>

namespace RevBayesCore
{
    class AreaSizeRateModifier : public AbstractCharacterHistoryRateModifier
    {
    public:
        AreaSizeRateModifier(std::vector<double> asz, unsigned int sm=1);
        AreaSizeRateModifier(const AreaSizeRateModifier& m);
        double computeRateModifier(std::vector<CharacterEvent*> currState, CharacterEvent* newState);
        double computeRateModifier(std::vector<CharacterEvent*> currState, size_t areaIdx);
        AreaSizeRateModifier* clone(void) const;
        
        void updatePower(double p, bool upd=true);
        void update(void);
        
    protected:
        
    private:
        std::vector<double> areaSizes;
        std::vector<double> areaPowers;
        double power;
        unsigned int stateModified;
        unsigned int numAreas;
        
    };
}

#endif /* defined(__rb_mlandis__AreaSizeRateModifier__) */
