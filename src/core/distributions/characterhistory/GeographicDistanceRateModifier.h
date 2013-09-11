//
//  GeographicDistanceRateModifier.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/8/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__GeographicDistanceRateModifier__
#define __rb_mlandis__GeographicDistanceRateModifier__

#include "AbstractCharacterHistoryRateModifier.h"
#include <string>

namespace RevBayesCore
{
    class GeographicDistanceRateModifier : public AbstractCharacterHistoryRateModifier
    {
    public:
        GeographicDistanceRateModifier(void); // pass and parse map
        double computeRateModifier(std::vector<CharacterEvent*> curState, std::set<CharacterEvent*> newState);
        
    protected:
        double computePairwiseDistances(int i, int j);
        
        
    private:
        std::string distanceType;
        std::vector<std::vector<double> > geographicCoordinates;
        std::vector<std::vector<double> > geographicDistances;
        
        //
        int numAreas;
        
        // set this up to have clean/dirty/etc ??
    };
}

#endif /* defined(__rb_mlandis__GeographicDistanceRateModifier__) */
