//
//  BiogeographyRateMapFunction.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/3/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__BiogeographyRateMapFunction__
#define __rb_mlandis__BiogeographyRateMapFunction__

#include "GeographyRateModifier.h"
#include "RateMap_Biogeography.h"
#include "RbVector.h"
#include "TimeTree.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class BiogeographyRateMapFunction : public TypedFunction<RateMap> {
        
    public:
        BiogeographyRateMapFunction(size_t nc, bool fe=true);                                                                                             // pass in geography object??
        virtual                                            ~BiogeographyRateMapFunction(void);                                              //!< Virtual destructor
        
        // public member functions
        BiogeographyRateMapFunction*                        clone(void) const;                                                              //!< Create an independent clone
        
        // set parameters
        void                                                setClockRate(const TypedDagNode< double > *r);
        void                                                setClockRate(const TypedDagNode< RbVector< double > > *r);
        //void                                                setDistancePower(const TypedDagNode<double>* dp);
        void                                                setGeographyRateModifier(const TypedDagNode<GeographyRateModifier>* drm);
        void                                                setGainLossRates(const TypedDagNode< RbVector<double> >* glr);
//        void                                                setGainLossRates(const TypedDagNode<std::vector<std::vector<double> > >* glr);
        void                                                setRootFrequencies(const TypedDagNode< RbVector< double > > *f);
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode< double >*                           homogeneousClockRate;
        const TypedDagNode< RbVector< double > >*               heterogeneousClockRates;
        const TypedDagNode< RbVector<double> >*                 homogeneousGainLossRates;
        const TypedDagNode< RbVector<double> >*                 heterogeneousGainLossRates;
        //const TypedDagNode<double>*                             distancePower;
        const TypedDagNode<GeographyRateModifier>*              geographyRateModifier;
        const TypedDagNode<TimeTree>*                           tau;
        const TypedDagNode< RbVector<double> >*                 rootFrequencies;
        
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
