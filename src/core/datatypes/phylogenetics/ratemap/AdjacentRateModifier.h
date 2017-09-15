//
//  AdjacentRateModifier.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/8/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdjacentRateModifier__
#define __rb_mlandis__AdjacentRateModifier__

#include "CharacterHistoryRateModifier.h"
#include "StochasticNode.h"
#include "TopologyNode.h"

#include <set>
#include <string>
#include <vector>


namespace RevBayesCore
{
    
    
    
    class AdjacentRateModifier : public CharacterHistoryRateModifier
    {
    public:
        AdjacentRateModifier(size_t ns, size_t nc);
        AdjacentRateModifier(const AdjacentRateModifier& g);
        
        
        AdjacentRateModifier&               assign(const Assignable &m);
        double                              computeRateMultiplier(std::vector<CharacterEvent*> currState, CharacterEvent* newState, double age=0.0);
        double                              computeSiteRateMultiplier(const TopologyNode& node, CharacterEvent* currState, CharacterEvent* newState, double age=0.0);
        double                              computeSiteRateMultiplier(const TopologyNode& node, unsigned currState, unsigned newState, unsigned charIdx=0, double age=0.0);
        
        double                              computeRateMultiplierUsingWidth(std::vector<CharacterEvent*> currState, CharacterEvent* newState, double age=0.0);
        double                              computeRateMultiplierUsingMatrix(std::vector<CharacterEvent*> currState, CharacterEvent* newState, double age=0.0);
        
        void                                setContextMatrix(const RbVector<RbVector<double> >& c);
        void                                setGainFactor(double f);
        void                                setLossFactor(double f);
        void                                setWidth(size_t w);
        
        
        void                                update(void);
        AdjacentRateModifier*               clone(void) const;
        
    protected:
       
        
    private:
        void                                initializeContexts(RbVector<RbVector<double> > c);
        
        struct adjacency {
            size_t                          from;
            size_t                          to;
            double                          weight;
        };
        
        std::vector<std::vector<adjacency> > context_matrix;
        size_t                               width;
        double                               gain_factor;
        double                               loss_factor;
        
        std::string                          context_type;
    };
    
    std::ostream& operator<<(std::ostream& o, const AdjacentRateModifier& x);
}

#endif /* defined(__rb_mlandis__AdjacentRateModifier__) */
