//
//  GeographyRateModifier.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/8/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__GeographyRateModifier__
#define __rb_mlandis__GeographyRateModifier__

#include "AbstractCharacterHistoryRateModifier.h"
#include "TimeAtlas.h"
#include "GeographicArea.h"
#include "StochasticNode.h"
#include "TopologyNode.h"
#include <string>

namespace RevBayesCore
{
    class GeographyRateModifier : public AbstractCharacterHistoryRateModifier
    {
    public:
        GeographyRateModifier( const TimeAtlas* ta,  bool uadj=false, bool uav=false, bool udd=false, int index=0, double dp=10e-6, double threshhold=1e-6, std::string dt="haversine" );
        GeographyRateModifier(const GeographyRateModifier& g);
        
        double                              computeRateModifier(std::vector<CharacterEvent*> curState, CharacterEvent* newState);
        double                              computeRateModifier(std::vector<CharacterEvent*> curState, CharacterEvent* newState, double age=0.0);
        double                              computeRateModifier(const TopologyNode& node, std::vector<CharacterEvent*> curState, CharacterEvent* newState, double age = 0.0);
//        double                              computeRateModifier_test(std::vector<CharacterEvent*> curState, CharacterEvent* newState, double age=0.0);
        double                              computeSiteRateModifier(const TopologyNode& node, CharacterEvent* curState, CharacterEvent* newState, double age=0.0);
        double                              computeSiteRateModifier(const TopologyNode& node, unsigned curState, unsigned newState, unsigned charIdx=0, double age=0.0);
        unsigned                            getNumAvailableAreas(const TopologyNode& node, std::vector<CharacterEvent*> curState, double age=0.0);
        unsigned                            getNumEmigratableAreas(const TopologyNode& node, std::vector<CharacterEvent*> curState, double age=0.0);
        void                                setDistancePower(double dp, bool upd=true);
        const std::vector<double>&          getGeographicDistancePowers(void) const;
        void                                setGeographicDistancePowers(const std::vector<double>& dp);
        unsigned                            getEpochIndex(double age);
        const std::vector<double>&          getEpochs(void) const;
        const std::vector<double>&          getDispersalValues(void) const;
        const std::vector<double>&          getExtinctionValues(void) const;
        const std::vector<double>&          getAvailableAreaVector(void) const;
        const std::vector<double>&          getAdjacentAreaVector(void) const;
        const std::vector<std::set<size_t> >&      getAdjacentAreaSet(void) const;
        const bool                          getUseAreaAvailable(void) const;
        const bool                          getUseAreaAdjacency(void) const;
        
        void                                update(void);
        GeographyRateModifier*              clone(void) const;
        void                                print(std::vector<std::vector<double> > m);
        void                                printAll(void);
        
    protected:
        void                                setInboundDispersal(const std::vector<double> &v);
        double                              computePairwiseDistances(int i, int j, int k);
//        void                                computeAllPairwiseDistances(void);
//        void                                computeAllPairwiseDistanceOrder(void);
       
        void                                initializeAdjacentAreas(void);
        void                                initializeDistances(void);
        void                                initializeDispersalExtinctionValues(void);
        
    private:
        
        // map objects
        const TimeAtlas* atlas;
        std::vector<GeographicArea*> areas;
        std::vector<double> epochs;
        int index;
        
        // distance values
        std::string distanceType;
        std::vector<double> geographicCoordinates;
        std::vector<double> geographicDistances;
        std::vector<double> geographicDistancePowers;
        std::vector<size_t> geographicDistanceOrder;
        
        // adjacencies
        std::vector<double> dispersalValues;
        std::vector<double> extinctionValues;
        std::vector<double> inboundDispersalValues;
        std::vector<std::set<size_t> > adjacentAreaSet;
        std::vector<std::set<size_t> > availableAreaSet;
        std::vector<double> adjacentAreaVector;
        std::vector<double> availableAreaVector;
        
        
        // helper variables
        unsigned numAreas;
        unsigned numEpochs;
        unsigned epochOffset;
        unsigned areaOffset;
        
        bool useAreaAdjacency;
        bool useAreaAvailable;
        bool useDistanceDependence;
        
        double threshhold;
        double distancePower;
        
        std::set<CharacterEvent*> present;
        std::set<CharacterEvent*> absent;
        
    };
    
    std::ostream& operator<<(std::ostream& o, const GeographyRateModifier& x);
}

#endif /* defined(__rb_mlandis__GeographyRateModifier__) */
