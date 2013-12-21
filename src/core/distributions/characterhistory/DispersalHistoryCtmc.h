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
#include "AreaSizeRateModifier.h"
#include "GeographicDistanceRateModifier.h"
#include "GeographicGridRateModifier.h"
#include "RangeSizeRateModifier.h"

namespace RevBayesCore
{
    class DispersalHistoryCtmc : public AbstractCharacterHistoryCtmc
    {
        
    public:
        DispersalHistoryCtmc(TypedDagNode<RateMatrix> *rm, std::vector<const TypedDagNode<double>* > r, const TypedDagNode<TimeTree>* t, const TypedDagNode<double>* br, const TypedDagNode<double>* dp, const TypedDagNode<double>* grsp, const TypedDagNode<double>* lrsp, const TypedDagNode<double>* rsf, const TypedDagNode<double>* asp, size_t nc, size_t ns, size_t idx, GeographicDistanceRateModifier* gd=NULL, RangeSizeRateModifier* grs=NULL, RangeSizeRateModifier* lrs=NULL, AreaSizeRateModifier* asrm=NULL);
       
        DispersalHistoryCtmc(std::vector<const TypedDagNode<double>* > r, const TypedDagNode<TimeTree>* t, const TypedDagNode<double>* br, const TypedDagNode<double>* dp, const TypedDagNode<double>* grsp, const TypedDagNode<double>* lrsp, const TypedDagNode<double>* rsf, const TypedDagNode<double>* asp, size_t nc, size_t ns, size_t idx, GeographicDistanceRateModifier* gd=NULL, RangeSizeRateModifier* grs=NULL, RangeSizeRateModifier* lrs=NULL, AreaSizeRateModifier* asrm=NULL, std::vector<GeographicGridRateModifier*> ggrmv=std::vector<GeographicGridRateModifier*>());
        
        DispersalHistoryCtmc(const DispersalHistoryCtmc& m);
                
        DispersalHistoryCtmc*               clone(void) const;
        double                              computeLnProbability(void);
        double                              computeLnProposal(void);
        //void                                redrawValue(void);
        double                              sumOfRates(std::vector<CharacterEvent*> currState);
        void                                swapParameter(const DagNode *oldP, const DagNode *newP);
        virtual void                        touchSpecialization(DagNode *toucher);
        double                              transitionRate(std::vector<CharacterEvent*> currState, CharacterEvent* nextState);
        
        // inherited virtual functions
        virtual void                                            redrawValue(void);
        virtual void                                            simulate(void);
        
        virtual void                                            simulatePath(const std::set<size_t>& indexSet);
        virtual void                                            samplePath(const std::set<size_t>& indexSet);
        virtual double                                          sampleChildCharacterState(const std::set<size_t>& indexSet);
        virtual double                                          sampleParentCharacterState(const std::set<size_t>& indexSet);
        virtual double                                          sampleRootCharacterState(const std::set<size_t>& indexSet);
        virtual double                                          sampleChildCharacterState(const std::set<size_t>& indexSet, const std::vector<CharacterEvent*>& state1, const std::vector<CharacterEvent*>& state2, double t1, double t2);
        
        
    protected:
        bool                                                    historyContainsExtinction(const std::vector<CharacterEvent*>& v, const std::multiset<CharacterEvent*,CharacterEventCompare>& s);
        virtual double                                          sampleCharacterState(const std::set<size_t>& indexSet, std::vector<CharacterEvent*>& states, double t);
        unsigned int                                            numOn(std::vector<CharacterEvent*> v);
        unsigned int                                            numOn(std::set<CharacterEvent*> s);
        unsigned int                                            numOn(std::set<CharacterEvent*,CharacterEventCompare> s);
        
        const TypedDagNode<double>*         distancePower;
        const TypedDagNode<double>*         gainRangeSizePower;
        const TypedDagNode<double>*         lossRangeSizePower;
        const TypedDagNode<double>*         rangeSizeFrequency;
        const TypedDagNode<double>*         areaSizePower;
        
        GeographicDistanceRateModifier*     geographicDistances;
        RangeSizeRateModifier*              gainRangeSizeRateModifier;
        RangeSizeRateModifier*              lossRangeSizeRateModifier;
        AreaSizeRateModifier*               areaSizeRateModifier;
        std::vector<GeographicGridRateModifier*> geographicGrids;
        
    private:
        
    };
}

#endif /* defined(__rb_mlandis__DispersalHistoryCtmc__) */
