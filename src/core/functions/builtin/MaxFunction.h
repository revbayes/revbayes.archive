/**
 * @file
 * This file contains the declaration of the maximum function for Vectors.
 *
 * @brief Declaration of the deterministic variable for maximum.
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



#ifndef MaxFunction_H
#define MaxFunction_H

#include "TypedFunction.h"
#include "TypedDagNode.h"

#include <vector>

namespace RevBayesCore {
    
    class MaxFunction : public TypedFunction<double> {
        
    public:
        MaxFunction(const TypedDagNode< RbVector<double> > * v);
        virtual                                            ~MaxFunction(void);                                                       //!< Virtual destructor
        
        // public member functions
        MaxFunction*                                        clone(void) const;                                                          //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode< RbVector<double> >*             vals;
        
    };
    
}


#endif
