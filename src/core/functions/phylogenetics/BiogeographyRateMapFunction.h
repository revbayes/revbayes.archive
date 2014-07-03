//
//  BiogeographyRateMapFunction.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/3/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__BiogeographyRateMapFunction__
#define __rb_mlandis__BiogeographyRateMapFunction__

#include "GeographicDistanceRateModifier.h"
#include "RateMap_Biogeography.h"
#include "TimeTree.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class BiogeographyRateMapFunction : public TypedFunction<RateMap> {
        
    public:
        BiogeographyRateMapFunction(size_t nc, bool fe=true);                                                                                             // pass in geography object??
        BiogeographyRateMapFunction(const BiogeographyRateMapFunction &n);                                                                  //!< Copy constructor
        virtual                                            ~BiogeographyRateMapFunction(void);                                              //!< Virtual destructor
        
        // public member functions
        BiogeographyRateMapFunction*                        clone(void) const;                                                              //!< Create an independent clone
        
        // set parameters
        void                                                setClockRate(const TypedDagNode< double > *r);
        void                                                setClockRate(const TypedDagNode< std::vector< double > > *r);
        //void                                                setDistancePower(const TypedDagNode<double>* dp);
        void                                                setGeographicDistanceRateModifier(const TypedDagNode<GeographicDistanceRateModifier>* drm);
        void                                                setGainLossRates(const TypedDagNode<std::vector<double> >* glr);
//        void                                                setGainLossRates(const TypedDagNode<std::vector<std::vector<double> > >* glr);
        void                                                setRootFrequencies(const TypedDagNode< std::vector< double > > *f);
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode< double >*                           homogeneousClockRate;
        const TypedDagNode< std::vector< double > >*            heterogeneousClockRates;
        const TypedDagNode<std::vector<double> >*               homogeneousGainLossRates;
        const TypedDagNode<std::vector<double> >*               heterogeneousGainLossRates;
        //const TypedDagNode<double>*                             distancePower;
        const TypedDagNode<GeographicDistanceRateModifier>*     geographicDistanceRateModifier;
        const TypedDagNode<TimeTree>*                           tau;
        
        // geography epochs
        // rate epochs
        // branchwise models, etc
        
        bool                                                branchHeterogeneousClockRates;
        bool                                                branchHeterogeneousGainLossRates;
        bool                                                useGeographicDistance;
        
    };
    
    std::ostream& operator<<(std::ostream& o, const std::vector<std::vector<double> >& x);
    
}

#endif /* defined(__rb_mlandis__BiogeographyRateMapFunction__) */
