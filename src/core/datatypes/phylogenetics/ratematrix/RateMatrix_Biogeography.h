//
//  RateMatrix_Biogeography.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/2/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__RateMatrix_Biogeography__
#define __rb_mlandis__RateMatrix_Biogeography__

#include "RateFunctionMatrix.h"
#include <vector>


namespace RevBayesCore {
    
    class RateMatrix_Biogeography : public RateFunctionMatrix {
        
    public:
        RateMatrix_Biogeography(size_t ns, size_t nc);                                                                                               //!< Construct rate matrix with n states
        RateMatrix_Biogeography(const RateMatrix_Biogeography& m);                                                                       //!< Copy constructor
        virtual                             ~RateMatrix_Biogeography(void);                                                              //!< Destructor
        
        // overloaded operators
        RateMatrix_Biogeography&            operator=(const RateMatrix_Biogeography& r);
        
        // implemented pure virtual functions
        double                              getRate(DiscreteCharacterState* fromState, DiscreteCharacterState* toState) const;
        double                              getSumOfRates(DiscreteCharacterState* fromState) const;
        const std::vector<double>&          getGainLossRates(void) const;

        RateMatrix_Biogeography*            clone(void) const;
        void                                updateMatrix(void);
        void                                setDistancePower(double d);
        void                                setGainLossRates(const std::vector<double>& r);
        
    private:
        
        std::vector<double>                 gainLossRates;
        double                              distancePower;
        
    };
    
}

#endif /* defined(__rb_mlandis__RateMatrix_Biogeography__) */
