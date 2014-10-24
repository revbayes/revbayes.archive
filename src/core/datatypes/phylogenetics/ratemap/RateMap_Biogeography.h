//
//  RateMap_Biogeography.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/2/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__RateMap_Biogeography__
#define __rb_mlandis__RateMap_Biogeography__

#include "RateMap.h"
#include "GeographyRateModifier.h"
#include <vector>

namespace RevBayesCore {
    
    class BranchHistory;
    class RateMap_Biogeography : public RateMap {
        
    public:
        RateMap_Biogeography(size_t nc, bool fe=true);                                                                               //!< Construct rate matrix with n states
        RateMap_Biogeography(const RateMap_Biogeography& m);                                                           //!< Copy constructor
        virtual                                     ~RateMap_Biogeography(void);                                       //!< Destructor
        
        // overloaded operators
        RateMap_Biogeography&                       operator=(const RateMap_Biogeography& r);
        
        // virtual RateMap functions
//        void                                        calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix& P, double age=0.0) const;   //!< Calculate the transition probabilities for the rate matrix
        void                                        calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix& P, size_t charIdx) const;   //!< Calculate the transition probabilities for the rate matrix
        void                                        calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition
        RateMap_Biogeography*                       clone(void) const;
        double                                      getRate(const TopologyNode& node, std::vector<CharacterEvent*> from, CharacterEvent* to, double age=0.0) const;
        double                                      getRate(const TopologyNode& node, std::vector<CharacterEvent*> from, CharacterEvent* to, unsigned* counts, double age=0.0) const;
        double                                      getSiteRate(const TopologyNode& node, CharacterEvent* from, CharacterEvent* to, double age=0.0) const;
        double                                      getSiteRate(const TopologyNode& node, unsigned from, unsigned to, unsigned charIdx=0, double age=0.0) const;
        double                                      getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, double age=0.0) const;
        double                                      getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, unsigned* counts, double age=0.0) const;
        double                                      getUnnormalizedSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, unsigned* counts, double age=0.0) const;
        const bool                                  isAreaAvailable(size_t charIdx, double age=0.0) const;
        const bool                                  areAreasAdjacent(size_t fromCharIdx, size_t toCharIdx, double age=0.0) const;
        const std::set<size_t>                      getRangeAndFrontierSet(const TopologyNode& node, BranchHistory* bh, double age=0.0) const;
        void                                        updateMap(void);
        
        // public methods
        double                                      getDistancePower(void) const;
        void                                        setDistancePower(double d);
//        double                                      getHomogeneousClockRate(void) const;
//        void                                        setHomogeneousClockRate(double d);
//        const std::vector<double>&                  getHeterogeneousClockRates(void) const;
//        void                                        setHeterogeneousClockRates(const std::vector<double>& r);
//        const std::vector<double>&                  getHomogeneousGainLossRates(void) const;
//        void                                        setHomogeneousGainLossRates(const std::vector<double>& r);
//        const std::vector<std::vector<double> >&    getHeterogeneousGainLossRates(void) const;
//        void                                        setHeterogeneousGainLossRates(const std::vector<std::vector<double> >& r);
//        const std::vector<double>&                  getRootFrequencies(void) const;
//        void                                        setRootFrequencies(const std::vector<double>& r);
        
        // other crazy stuff for BiogeographyRateMapFunction to handle
        void                                        setGeographyRateModifier(const GeographyRateModifier& gdrm);
        void                                        setGeographicDistancePowers(const GeographyRateModifier& gdrm);
        const GeographyRateModifier &               getGeographyRateModifier(void);
        const std::vector<double>&                  getEpochs(void) const;
        
    private:
        size_t                                      numOn(const std::vector<CharacterEvent*>& s) const;
        size_t                                      numOn(const std::vector<CharacterEvent*>& s, double age) const;
        unsigned                                    getEpochIndex(double age) const;
        size_t                                      branchOffset;

//        double                                      homogeneousClockRate;
//        std::vector<double>                         heterogeneousClockRates;
//        std::vector<double>                         homogeneousGainLossRates;
//        std::vector<std::vector<double> >           heterogeneousGainLossRates;
//        std::vector<double>                         rootFrequencies;
        double                                      distancePower;
        
        // geography models
        bool                                        useGeographyRateModifier;
        GeographyRateModifier*                      geographyRateModifier;
        std::vector<double>                         epochs;
        size_t                                      numEpochs;
        size_t                                      epochOffset;
        std::vector<double>                         adjacentAreaVector;
        std::vector<double>                         availableAreaVector;

        // model flags
//        bool                                        branchHeterogeneousClockRates;
//        bool                                        branchHeterogeneousGainLossRates;
        bool                                        forbidExtinction;
        
        bool                                        useUnnormalizedRates;
        bool                                        useAreaAdjacency;
        bool                                        useAreaAvailable;
        bool                                        useDistanceDependence;
        bool                                        useRootFrequencies;
        
    };
    
}
#endif /* defined(__rb_mlandis__RateMap_Biogeography__) */
