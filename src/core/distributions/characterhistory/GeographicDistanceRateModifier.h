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
        GeographicDistanceRateModifier(void); // pass map
        
    protected:
        void computePairwiseDistances(void);
        double computeDistanceModifier(std::vector<CharacterEvent*> startingState, std::set<CharacterEvent*> stateChange);
        
    private:
        std::string distanceType;
        std::vector<std::vector<double> > geographicCoordinates;
        std::vector<std::vector<double> > geographicDistances;
    };
}

#endif /* defined(__rb_mlandis__GeographicDistanceRateModifier__) */
