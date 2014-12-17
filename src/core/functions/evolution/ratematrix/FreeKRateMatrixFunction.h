//
//  FreeKRateMatrixFunction.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/4/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__FreeKRateMatrixFunction__
#define __rb_mlandis__FreeKRateMatrixFunction__

#include "RateMatrix_FreeBinary.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class FreeKRateMatrixFunction : public TypedFunction<RateMatrix> {
        
    public:
        FreeKRateMatrixFunction(const TypedDagNode< RbVector<double> > *tr, const TypedDagNode< RbVector<double> > *sf);
        virtual                                            ~FreeKRateMatrixFunction(void);                                                    //!< Virtual destructor
        
        // public member functions
        FreeKRateMatrixFunction*                            clone(void) const;                                                              //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode< RbVector<double> >*             transitionRates;
        const TypedDagNode< RbVector<double> >*             stationaryFrequencies;
    };
    
}

#endif /* defined(__rb_mlandis__FreeKRateMatrixFunction__) */
