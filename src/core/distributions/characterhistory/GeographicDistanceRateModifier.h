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
#include "TimeAtlas.h"
#include "GeographicArea.h"
#include "StochasticNode.h"
#include <string>

namespace RevBayesCore
{
    class GeographicDistanceRateModifier : public AbstractCharacterHistoryRateModifier
    {
    public:
        GeographicDistanceRateModifier( TimeAtlas* ta, int index=0, double dp=1.0, double threshhold=1e-6, std::string dt="haversine" );
        //GeographicDistanceRateModifier(std::vector<std::vector<double> > gc, double dp=1.0, double threshhold = 1e-6, std::string dt="haversine"); // pass map... pass it parameter pointer?
        GeographicDistanceRateModifier(const GeographicDistanceRateModifier& g);
        
        double                              computeRateModifier(std::vector<CharacterEvent*> curState, CharacterEvent* newState);
        double                              computeRateModifier(std::vector<CharacterEvent*> curState, CharacterEvent* newState, double age=0.0);
        double                              computeRateModifier(const TopologyNode& node, std::vector<CharacterEvent*> curState, CharacterEvent* newState, double age = 0.0);
        void                                setDistancePower(double dp=1.0, bool upd=true);
        const std::vector<double>&          getGeographicDistancePowers(void) const;
        void                                setGeographicDistancePowers(const std::vector<double>& dp);
        unsigned                            getEpochIndex(double age);
        void                                update(void);
        GeographicDistanceRateModifier*     clone(void) const;
        void                                print(std::vector<std::vector<double> > m);
        void                                printAll(void);
        
    protected:
        double                              computePairwiseDistances(int i, int j, int k);
        void                                computeAllPairwiseDistances(void);
        void                                computeAllPairwiseDistanceOrder(void);
        
    private:
        
        // map objects
        TimeAtlas* atlas;
        std::vector<std::vector<GeographicArea*> > areas;
        std::vector<double> epochs;
        int index;
        
        // distance values
        std::string distanceType;
        std::vector<double> geographicCoordinates;
        std::vector<double> geographicDistances;
        std::vector<double> geographicDistancePowers;
        std::vector<size_t> geographicDistanceOrder;
        
        // helper variables
        unsigned numAreas;
        unsigned numEpochs;
        unsigned epochOffset;
        unsigned areaOffset;
        
        double threshhold;
        double distancePower;
        
    };
}

#endif /* defined(__rb_mlandis__GeographicDistanceRateModifier__) */
