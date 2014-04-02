//
//  BiogeographyRateFunctionMatrixFunction.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/2/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__BiogeographyRateFunctionMatrixFunction__
#define __rb_mlandis__BiogeographyRateFunctionMatrixFunction__

#include "RateMatrix_Biogeography.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class BiogeographyRateFunctionMatrixFunction : public TypedFunction<RateFunctionMatrix> {
        
    public:
        BiogeographyRateFunctionMatrixFunction(const TypedDagNode<double> *d, size_t nc); // eventually, pass in geography object to get nc...
        BiogeographyRateFunctionMatrixFunction(const BiogeographyRateFunctionMatrixFunction &n);                                                                              //!< Copy constructor
        virtual                                            ~BiogeographyRateFunctionMatrixFunction(void);                                                    //!< Virtual destructor
        
        // public member functions
        BiogeographyRateFunctionMatrixFunction*                              clone(void) const;                                                              //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode<double>*                         distancePower;
        
    };
    
}



#endif /* defined(__rb_mlandis__BiogeographyRateFunctionMatrixFunction__) */
