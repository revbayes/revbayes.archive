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
#include <vector>

namespace RevBayesCore {
        
    class RateMap_Biogeography : public RateMap {
        
    public:
        RateMap_Biogeography(size_t nc);                                                                    //!< Construct rate matrix with n states
        RateMap_Biogeography(const RateMap_Biogeography& m);                                                           //!< Copy constructor
        virtual                                     ~RateMap_Biogeography(void);                                       //!< Destructor
        
        // overloaded operators
        RateMap_Biogeography&                       operator=(const RateMap_Biogeography& r);
        
        // virtual RateMap functions
        RateMap_Biogeography*                       clone(void) const;
        double                                      getRate(CharacterState* from, CharacterState* to) const;
        double                                      getSumOfRates(CharacterState* from) const;
        double                                      getRate(std::vector<CharacterEvent*> from, CharacterEvent* to) const;
        double                                      getSumOfRates(std::vector<CharacterEvent*> from) const;

        double                                      getTransitionProbability(CharacterState* from, CharacterState* to, double t) const;
        double                                      getLnTransitionProbability(CharacterState* from, CharacterState* to, double t) const;
        double                                      getLnTransitionProbability(std::vector<CharacterEvent*> from, CharacterEvent* to, double t) const;
        void                                        updateMap(void);
        
        // public methods
        double                                      getDistancePower(void) const;
        void                                        setDistancePower(double d);
        const std::vector<double>&                  getGainLossRates(void) const;
        void                                        setGainLossRates(const std::vector<double>& r);
        
    private:
        size_t                                      numOn(const std::vector<CharacterEvent*>& s) const;
        std::vector<double>                         gainLossRates;
        double                                      distancePower;
        
    };
    
}
#endif /* defined(__rb_mlandis__RateMap_Biogeography__) */
