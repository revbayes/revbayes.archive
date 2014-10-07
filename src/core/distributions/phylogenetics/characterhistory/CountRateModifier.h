//
//  CountRateModifier.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/8/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__CountRateModifier__
#define __rb_mlandis__CountRateModifier__

#include "AbstractCharacterHistoryRateModifier.h"
#include "StochasticNode.h"
#include <string>

namespace RevBayesCore
{
    class CountRateModifier : public AbstractCharacterHistoryRateModifier
    {
    public:
        CountRateModifier(unsigned nc);
        CountRateModifier(const CountRateModifier& g);
        
        double                              computeRateModifier(std::vector<CharacterEvent*> curState, CharacterEvent* newState);
        double                              computeRateModifier(std::vector<CharacterEvent*> curState, CharacterEvent* newState, double age=0.0);
        double                              computeRateModifier(const TopologyNode& node, std::vector<CharacterEvent*> curState, CharacterEvent* newState, double age = 0.0);
        double                              computeSiteRateModifier(const TopologyNode& node, CharacterEvent* curState, CharacterEvent* newState, double age=0.0);
        double                              computeSiteRateModifier(const TopologyNode& node, unsigned curState, unsigned newState, unsigned charIdx=0, double age=0.0);
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
