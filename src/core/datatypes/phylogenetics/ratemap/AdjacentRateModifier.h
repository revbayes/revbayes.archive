//
//  AdjacentRateModifier.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/8/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdjacentRateModifier__
#define __rb_mlandis__AdjacentRateModifier__

#include "CharacterHistoryRateModifier.h"
#include "StochasticNode.h"
#include "TopologyNode.h"

#include <string>

namespace RevBayesCore
{
    class AdjacentRateModifier : public CharacterHistoryRateModifier
    {
    public:
        AdjacentRateModifier(size_t ns, size_t nc);
        AdjacentRateModifier(const AdjacentRateModifier& g);
        
        double                              computeRateMultiplier(std::vector<CharacterEvent*> curState, CharacterEvent* newState);
        double                              computeRateMultiplier(std::vector<CharacterEvent*> curState, CharacterEvent* newState, double age=0.0);
        double                              computeRateMultiplier(const TopologyNode& node, std::vector<CharacterEvent*> curState, CharacterEvent* newState, double age = 0.0);
        double                              computeSiteRateMultiplier(const TopologyNode& node, CharacterEvent* curState, CharacterEvent* newState, double age=0.0);
        double                              computeSiteRateMultiplier(const TopologyNode& node, unsigned curState, unsigned newState, unsigned charIdx=0, double age=0.0);
        
        void                                setFactor(double f);
        void                                setWidth(size_t w);
        
        
        void                                update(void);
        AdjacentRateModifier*               clone(void) const;
        
    protected:
       
        
    private:
        
        size_t                              width;
        double                              factor;
                
    };
    
    std::ostream& operator<<(std::ostream& o, const AdjacentRateModifier& x);
}

#endif /* defined(__rb_mlandis__AdjacentRateModifier__) */
