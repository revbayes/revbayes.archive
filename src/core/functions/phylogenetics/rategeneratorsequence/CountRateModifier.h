//
//  CountRateModifier.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/8/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__CountRateModifier__
#define __rb_mlandis__CountRateModifier__

#include "CharacterHistoryRateModifier.h"
#include "StochasticNode.h"
#include "TopologyNode.h"

#include <string>

namespace RevBayesCore
{
    class CountRateModifier : public CharacterHistoryRateModifier
    {
    public:
        CountRateModifier(unsigned nc);
        CountRateModifier(const CountRateModifier& g);
        
        double                              computeRateMultiplier(std::vector<CharacterEvent*> curState, CharacterEvent* newState);
        double                              computeRateMultiplier(std::vector<CharacterEvent*> curState, CharacterEvent* newState, double age=0.0);
        double                              computeRateMultiplier(const TopologyNode& node, std::vector<CharacterEvent*> curState, CharacterEvent* newState, double age = 0.0);
        double                              computeSiteRateMultiplier(const TopologyNode& node, CharacterEvent* curState, CharacterEvent* newState, double age=0.0);
        double                              computeSiteRateMultiplier(const TopologyNode& node, unsigned curState, unsigned newState, unsigned charIdx=0, double age=0.0);
        const std::vector<double>&          getProportions(void) const;
        void                                setProportions(const std::vector<double>& dp);
        
        void                                update(void);
        CountRateModifier*                  clone(void) const;
        
    protected:
       
        
    private:
        
        // parameter
        std::vector<double> proportions;
        
        // helper variables
        unsigned numCharacters;
        
    };
    
    std::ostream& operator<<(std::ostream& o, const CountRateModifier& x);
}

#endif /* defined(__rb_mlandis__CountRateModifier__) */
