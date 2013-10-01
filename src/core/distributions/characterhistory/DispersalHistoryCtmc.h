//
//  DispersalHistoryCtmc.h
//  rb_mlandis
//
//  Created by Michael Landis on 9/25/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__DispersalHistoryCtmc__
#define __rb_mlandis__DispersalHistoryCtmc__


#include "AbstractCharacterHistoryCtmc.h"
#include "GeographicDistanceRateModifier.h"

namespace RevBayesCore
{
    class DispersalHistoryCtmc : public AbstractCharacterHistoryCtmc
    {
        
    public:
        DispersalHistoryCtmc(TypedDagNode<RateMatrix> *rm, std::vector<const TypedDagNode<double>* > r, const TypedDagNode<TimeTree>* t, const TypedDagNode<double>* br, const TypedDagNode<double>* dp, size_t nc, size_t ns, size_t idx, GeographicDistanceRateModifier* gd=NULL);
        DispersalHistoryCtmc(const DispersalHistoryCtmc& m);
                
        DispersalHistoryCtmc*               clone(void) const;
        double                              computeLnProbability(void);
        //void                                redrawValue(void);
        double                              sumOfRates(std::vector<CharacterEvent*> currState);
        void                                swapParameter(const DagNode *oldP, const DagNode *newP);
        virtual void                        touchSpecialization(DagNode *toucher);
        double                              transitionRate(std::vector<CharacterEvent*> currState, CharacterEvent* nextState);
        
        // inherited virtual functions
        virtual void                                            redrawValue(void);
        
        //virtual void                                            simulatePath(void);
        //virtual void                                            simulateChildCharacterState(void);
        //virtual void                                            simulateParentCharacterState(void);
        virtual void                                            simulatePath(const std::set<size_t>& indexSet);
        virtual void                                            simulateChildCharacterState(const std::set<size_t>& indexSet);
        virtual void                                            simulateParentCharacterState(const std::set<size_t>& indexSet);
        
    protected:
        bool                                                    historyContainsExtinction(const std::vector<CharacterEvent*>& v, const std::set<CharacterEvent*,CharacterEventCompare>& s);
        virtual std::set<CharacterEvent*>                       simulateCharacterState(const std::set<size_t>& indexSet, double t);
        unsigned int                                            numOn(std::vector<CharacterEvent*> v);
        unsigned int                                            numOn(std::set<CharacterEvent*> s);
        unsigned int                                            numOn(std::set<CharacterEvent*,CharacterEventCompare> s);
        
        const TypedDagNode<double>*         distancePower;
        GeographicDistanceRateModifier*     geographicDistances;
        
    private:
        
    };
}

#endif /* defined(__rb_mlandis__DispersalHistoryCtmc__) */
