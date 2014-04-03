//
//  BiogeographyRateMapFunction.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/3/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__BiogeographyRateMapFunction__
#define __rb_mlandis__BiogeographyRateMapFunction__

#include "RateMap_Biogeography.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class BiogeographyRateMapFunction : public TypedFunction<RateMap> {
        
    public:
        BiogeographyRateMapFunction(const TypedDagNode<std::vector<double> > *glr, const TypedDagNode<double> *dp, size_t nc); // pass in geography object??
        BiogeographyRateMapFunction(const BiogeographyRateMapFunction &n);                                                                  //!< Copy constructor
        virtual                                            ~BiogeographyRateMapFunction(void);                                              //!< Virtual destructor
        
        // public member functions
        BiogeographyRateMapFunction*                        clone(void) const;                                                              //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode<std::vector<double> >*           gainLossRates;
        const TypedDagNode<double>*                         distancePower;
        
    };
    
}

#endif /* defined(__rb_mlandis__BiogeographyRateMapFunction__) */
