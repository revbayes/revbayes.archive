//
//  RateMap_AminoAcid.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/2/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__RateMap_AminoAcid__
#define __rb_mlandis__RateMap_AminoAcid__

#include "RbVector.h"
#include "RateMatrix.h"
#include "RateMap.h"
#include "GeographyRateModifier.h"
#include <vector>

namespace RevBayesCore {
    
    class RateMap_AminoAcid : public RateMap {
        
    public:
        RateMap_AminoAcid(size_t nc);                                                                               //!< Construct rate matrix with n states
        RateMap_AminoAcid(const RateMap_AminoAcid& m);                                                           //!< Copy constructor
        virtual                                     ~RateMap_AminoAcid(void);                                       //!< Destructor
        
        // overloaded operators
        RateMap_AminoAcid&                          operator=(const RateMap_AminoAcid& r);
        
        // virtual RateMap functions
        void                                        calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix& P, size_t charIdx) const;   //!< Calculate the transition probabilities for the rate matrix
        void                                        calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition
        RateMap_AminoAcid*                          clone(void) const;
        double                                      getRate(const TopologyNode& node, std::vector<CharacterEvent*> from, CharacterEvent* to, unsigned* counts=NULL, double age=0.0) const;
        double                                      getSiteRate(const TopologyNode& node, CharacterEvent* from, CharacterEvent* to, double age=0.0) const;
        double                                      getSiteRate(const TopologyNode& node, unsigned from, unsigned to, unsigned charIdx=0, double age=0.0) const;
        double                                      getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, double age=0.0) const;
        double                                      getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, unsigned* counts, double age=0.0) const;
        void                                        updateMap(void);
        
        // public methods
        double                                      getHomogeneousClockRate(void) const;
        void                                        setHomogeneousClockRate(double d);
        const std::vector<double>&                  getHeterogeneousClockRates(void) const;
        void                                        setHeterogeneousClockRates(const std::vector<double>& r);
        const RateMatrix*                           getHomogeneousRateMatrix(void) const;
        void                                        setHomogeneousRateMatrix(const RateMatrix* r);
        const RbVector<RateMatrix*>&                getHeterogeneousRateMatrices(void) const;
        void                                        setHeterogeneousRateMatrices(const RbVector<RateMatrix*>& r);
        const std::vector<double>&                  getRootFrequencies(void) const;
        void                                        setRootFrequencies(const std::vector<double>& r);

    private:
        double                                      homogeneousClockRate;
        std::vector<double>                         heterogeneousClockRates;
        RateMatrix*                                 homogeneousRateMatrix;
        RbVector< RateMatrix* >                     heterogeneousRateMatrices;
        std::vector<double>                         rootFrequencies;
                
        // model flags
        bool                                        branchHeterogeneousClockRates;
        bool                                        branchHeterogeneousRateMatrices;
        bool                                        useUnnormalizedRates;
        bool                                        useRootFrequencies;
        
    };
    
}
#endif /* defined(__rb_mlandis__RateMap_AminoAcid__) */
