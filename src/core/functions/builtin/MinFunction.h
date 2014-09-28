/**
 * @file
 * This file contains the declaration of the minimum function for Vectors.
 *
 * @brief Declaration of the deterministic variable for minimum.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-06, version 1.0
 * @interface TypedDagNode
 *
 * $Id$
 */



#ifndef MinFunction_H
#define MinFunction_H

#include "RbVector.h"
#include "TypedFunction.h"
#include "TypedDagNode.h"

#include <vector>

namespace RevBayesCore {
    
    class MinFunction : public TypedFunction<double> {
        
    public:
        MinFunction(const TypedDagNode< RbVector<double> > * v);
        virtual                                            ~MinFunction(void);                                                          //!< Virtual destructor
        
        // public member functions
        MinFunction*                                        clone(void) const;                                                          //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode< RbVector<double> >*             vals;
        
    };
    
}


#endif
