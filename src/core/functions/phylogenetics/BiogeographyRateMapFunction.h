//
//  BiogeographyRateMapFunction.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/3/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__BiogeographyRateMapFunction__
#define __rb_mlandis__BiogeographyRateMapFunction__

//#include "CountRateModifier.h"
#include "GeographyRateModifier.h"
#include "RateMatrix.h"
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
        BiogeographyRateMapFunction(const BiogeographyRateMapFunction &n);                                                                  //!< Copy constructor
        virtual                                            ~BiogeographyRateMapFunction(void);                                              //!< Virtual destructor
        
        // public member functions
        BiogeographyRateMapFunction*                        clone(void) const;                                                              //!< Create an independent clone
        
        // set parameters
        void                                                setClockRate(const TypedDagNode< double > *r);
        void                                                setClockRate(const TypedDagNode< std::vector< double > > *r);
        void                                                setGeographyRateModifier(const TypedDagNode<GeographyRateModifier>* drm);
//        void                                                setCountRateModifier(const TypedDagNode<double>* rsrm);
        void                                                setGainLossRates(const TypedDagNode<std::vector<double> >* glr);
        void                                                setRateMatrix(const TypedDagNode<RateMatrix>* rm);
        void                                                setRootFrequencies(const TypedDagNode< std::vector< double > > *f);
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode< double >*                       homogeneousClockRate;
        const TypedDagNode< std::vector< double > >*        heterogeneousClockRates;
        const TypedDagNode<RateMatrix>*                     homogeneousRateMatrix;
        const TypedDagNode<RbVector<RateMatrix> >*          heterogeneousRateMatrices;

        const TypedDagNode<GeographyRateModifier>*              geographyRateModifier;
//      const TypedDagNode<CountRateModifier>*                  countRateModifier;
        const TypedDagNode<TimeTree>*                           tau;
        const TypedDagNode<std::vector<double> >*               rootFrequencies;
        
        // geography epochs
        // rate epochs
        // branchwise models, etc
        
        bool                                                branchHeterogeneousClockRates;
        bool                                                branchHeterogeneousRateMatrices;
        bool                                                useGeographicDistance;
        
    };
    
    std::ostream& operator<<(std::ostream& o, const std::vector<std::vector<double> >& x);
    
}

#endif /* defined(__rb_mlandis__BiogeographyRateMapFunction__) */
