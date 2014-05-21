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
#include "GeographicDistanceRateModifier.h"
#include <vector>

namespace RevBayesCore {
        
    class RateMap_Biogeography : public RateMap {
        
    public:
        RateMap_Biogeography(size_t nc, bool fe=true);                                                                               //!< Construct rate matrix with n states
        RateMap_Biogeography(const RateMap_Biogeography& m);                                                           //!< Copy constructor
        virtual                                     ~RateMap_Biogeography(void);                                       //!< Destructor
        
        // overloaded operators
        RateMap_Biogeography&                       operator=(const RateMap_Biogeography& r);
        
        // virtual RateMap functions
        void                                        calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition probabilities for the rate matrix
        RateMap_Biogeography*                       clone(void) const;
        double                                      getRate(const TopologyNode& node, std::vector<CharacterEvent*> from, CharacterEvent* to, double age=0.0) const;
        //double                                      getRate(const TopologyNode& node, std::vector<CharacterEvent*> from, CharacterEvent* to, unsigned n, double age=0.0) const;
        double                                      getRate(const TopologyNode& node, std::vector<CharacterEvent*> from, CharacterEvent* to, unsigned* counts, double age=0.0) const;
        double                                      getSiteRate(const TopologyNode& node, CharacterEvent* from, CharacterEvent* to, double age=0.0) const;
        double                                      getSiteRate(const TopologyNode& node, unsigned from, unsigned to, double age=0.0) const;
        double                                      getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, double age=0.0) const;
        //double                                      getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, unsigned n, double age=0.0) const;
        double                                      getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, unsigned* counts, double age=0.0) const;
        double                                      getLnTransitionProbability(const TopologyNode& node, std::vector<CharacterEvent*> from, CharacterEvent* to, double t, double age=0.0) const;
        void                                        updateMap(void);
        
        // public methods
        double                                      getDistancePower(void) const;
        void                                        setDistancePower(double d);
        double                                      getHomogeneousClockRate(void) const;
        void                                        setHomogeneousClockRate(double d);
        const std::vector<double>&                  getHeterogeneousClockRates(void) const;
        void                                        setHeterogeneousClockRates(const std::vector<double>& r);
        const std::vector<double>&                  getHomogeneousGainLossRates(void) const;
        void                                        setHomogeneousGainLossRates(const std::vector<double>& r);
        const std::vector<std::vector<double> >&    getHeterogeneousGainLossRates(void) const;
        void                                        setHeterogeneousGainLossRates(const std::vector<std::vector<double> >& r);
        
        // other crazy stuff for BiogeographyRateMapFunction to handle
        void                                        setGeographicDistanceRateModifier(const GeographicDistanceRateModifier& gdrm);
        void                                        setGeographicDistancePowers(const GeographicDistanceRateModifier& gdrm);
        const GeographicDistanceRateModifier &      getGeographicDistanceRateModifier(void);
        
    private:
        size_t                                      numOn(const std::vector<CharacterEvent*>& s) const;

        size_t                                      epochOffset;
        size_t                                      branchOffset;

        double                                      homogeneousClockRate;
        std::vector<double>                         heterogeneousClockRates;
        std::vector<double>                         homogeneousGainLossRates;
        std::vector<std::vector<double> >           heterogeneousGainLossRates;
        double                                      distancePower;

        
        // come up with some way to handle epoch age vectors...
        bool                                        useGeographicDistanceRateModifier;
        GeographicDistanceRateModifier*             geographicDistanceRateModifier;
        
        bool                                        branchHeterogeneousClockRates;
        bool                                        branchHeterogeneousGainLossRates;
        bool                                        forbidExtinction;
        
    };
    
}
#endif /* defined(__rb_mlandis__RateMap_Biogeography__) */
