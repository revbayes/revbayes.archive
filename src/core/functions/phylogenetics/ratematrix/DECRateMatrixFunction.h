//
//  DECRateMatrixFunction.h
//  revbayes-proj
//
//  Created by Michael Landis on 3/16/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__DECRateMatrixFunction__
#define __revbayes_proj__DECRateMatrixFunction__

#include "RateMatrix_DECRateMatrix.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class DECRateMatrixFunction : public TypedFunction<RateGenerator> {
        
    public:
        DECRateMatrixFunction(const TypedDagNode< RbVector<RbVector<double> > > *dr, const TypedDagNode< RbVector<double> > *er, const TypedDagNode< RbVector<double> > *rs, bool cs=true, bool ex=false, bool os=true);
        virtual                                             ~DECRateMatrixFunction(void);                                                    //!< Virtual destructor
        
        // public member functions
        DECRateMatrixFunction*                              clone(void) const;                                                              //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode< RbVector<RbVector<double> > >*  dispersalRates;
        const TypedDagNode< RbVector<double> >*             extirpationRates;
        const TypedDagNode< RbVector<double> >*             rangeSize;
    };
    
}

#endif /* defined(__revbayes_proj__DECRateMatrixFunction__) */
