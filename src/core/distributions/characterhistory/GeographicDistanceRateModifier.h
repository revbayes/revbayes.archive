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
#include "StochasticNode.h"
#include <string>

namespace RevBayesCore
{
    class GeographicDistanceRateModifier : public AbstractCharacterHistoryRateModifier
    {
    public:
        GeographicDistanceRateModifier(std::vector<std::vector<double> > gc, double dp=1.0, double threshhold = 1e-9, std::string dt="haversine"); // pass map... pass it parameter pointer?
        double computeRateModifier(std::vector<CharacterEvent*> curState, CharacterEvent* newState); // ... or pass value to computeRateModifier
        void updateGeographicDistancePowers(double dp=1.0, bool upd=true);
        void update(void);
        GeographicDistanceRateModifier* clone(void) const;
        void print(std::vector<std::vector<double> > m);
        void printAll(void);
        
    protected:
        double computePairwiseDistances(int i, int j);
        void computeAllPairwiseDistances(void);
        
    private:
        std::string distanceType;
        std::vector<std::vector<double> > geographicCoordinates;
        std::vector<std::vector<double> > geographicDistances;
        std::vector<std::vector<double> > geographicDistancePowers;
        std::vector<std::vector<size_t> > geographicDistanceOrder;
        
        // helper variables
        int numAreas;
        double threshhold;
        double distancePower;
        
    };
}

#endif /* defined(__rb_mlandis__GeographicDistanceRateModifier__) */
