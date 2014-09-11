//
//  AminoAcidRateMapFunction.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/3/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AminoAcidRateMapFunction__
#define __rb_mlandis__AminoAcidRateMapFunction__

#include "GeographyRateModifier.h"
#include "RateMap_AminoAcid.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "TimeTree.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class AminoAcidRateMapFunction : public TypedFunction<RateMap> {
        
    public:
        AminoAcidRateMapFunction(size_t nc);                                                                                             // pass in geography object??
        AminoAcidRateMapFunction(const AminoAcidRateMapFunction &n);                                                                  //!< Copy constructor
        virtual                                            ~AminoAcidRateMapFunction(void);                                              //!< Virtual destructor
        
        // public member functions
        AminoAcidRateMapFunction*                           clone(void) const;                                                              //!< Create an independent clone
        
        // set parameters
        void                                                setClockRate(const TypedDagNode< double > *r);
        void                                                setClockRate(const TypedDagNode< std::vector< double > > *r);
        void                                                setRateMatrix(const TypedDagNode<RateMatrix>* rm);
//        void                                                setRateMatrix(const TypedDagNode<RbVector<RateMatrix> > >* rm);
        void                                                setRootFrequencies(const TypedDagNode< std::vector< double > > *f);
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode< double >*                       homogeneousClockRate;
        const TypedDagNode< std::vector< double > >*        heterogeneousClockRates;
        const TypedDagNode<RateMatrix>*                    homogeneousRateMatrix;
        const TypedDagNode<RbVector<RateMatrix> >*         heterogeneousRateMatrices;
        const TypedDagNode<TimeTree>*                       tau;
        const TypedDagNode<std::vector<double> >*           rootFrequencies;
        
        // geography epochs
        bool                                                branchHeterogeneousClockRates;
        bool                                                branchHeterogeneousRateMatrices;
        
    };
    
//    std::ostream& operator<<(std::ostream& o, const std::vector<std::vector<double> >& x);
    
}

#endif /* defined(__rb_mlandis__AminoAcidRateMapFunction__) */
