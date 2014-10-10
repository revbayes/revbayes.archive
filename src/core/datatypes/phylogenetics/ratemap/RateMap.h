//
//  RateMap.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/2/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__RateMap__
#define __rb_mlandis__RateMap__

#include "CharacterEvent.h"
#include "CharacterState.h"
#include "Cloneable.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include <vector>

namespace RevBayesCore {
    
    class RateMap : public Cloneable {
        
    public:
        RateMap(const RateMap& m);                                                              //!< Copy constructor
        RateMap(size_t ns, size_t nc);                                                          //!< Construct rate matrix with n states
        RateMap&                            operator=(const RateMap& r);
        
        virtual                            ~RateMap(void);                                                                 //!< Destructor
        
        // public methods
        size_t                              getNumberOfStates(void) const;                      //!< Return the number of states
        size_t                              getNumberOfCharacters(void) const;                  //!< Return the number of characters

        // virtual public methods
        virtual void                        calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix& P) const;
        virtual void                        calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix& P, size_t charIdx) const;
        virtual RateMap*                    clone(void) const;
        virtual double                      getRate(const TopologyNode& node, std::vector<CharacterEvent*> from, CharacterEvent* to, unsigned* counts, double age=0.0) const;
        virtual double                      getSiteRate(const TopologyNode& node, CharacterEvent* from, CharacterEvent* to, double age=0.0) const;
        virtual double                      getSiteRate(const TopologyNode& node, unsigned from, unsigned to, unsigned charIdx=0, double age=0.0) const;
        virtual double                      getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, double age=0.0) const;
        virtual double                      getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, unsigned* counts, double age=0.0) const;
        virtual void                        updateMap(void);
        
        double                              getHomogeneousClockRate(void) const;
        void                                setHomogeneousClockRate(double d);
        const std::vector<double>&          getHeterogeneousClockRates(void) const;
        void                                setHeterogeneousClockRates(const std::vector<double>& r);
        const RateMatrix*                   getHomogeneousRateMatrix(void) const;
        void                                setHomogeneousRateMatrix(const RateMatrix* r);
        const RbVector<RateMatrix>&         getHeterogeneousRateMatrices(void) const;
        void                                setHeterogeneousRateMatrices(const RbVector<RateMatrix>& r);
        const std::vector<double>&          getRootFrequencies(void) const;
        void                                setRootFrequencies(const RevBayesCore::RbVector<double>& r);


        
    protected:
        // protected members available for derived classes
        double                              homogeneousClockRate;
        std::vector<double>                 heterogeneousClockRates;
        RateMatrix*                         homogeneousRateMatrix;
        RbVector<RateMatrix>                heterogeneousRateMatrices;
        std::vector<double>                 rootFrequencies;
        
        size_t                              numStates;                                          //!< The number of character states
        size_t                              numCharacters;                                      //!< The number of characters
        bool                                needsUpdate;
        bool                                branchHeterogeneousRateMatrices;
        bool                                branchHeterogeneousClockRates;
        
        
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const RateMap& x);                                           //!< Overloaded output operator
    
}

#endif /* defined(__rb_mlandis__RateMap__) */
