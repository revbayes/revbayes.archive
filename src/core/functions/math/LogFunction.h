/**
 * @file
 * This file contains the declaration of the logarithm function, f(x) = log( a(x) ).
 *
 * @brief Declaration of the logarithm function.
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-06-20 22:57:09 +0200 (Wed, 20 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-08-13, version 1.0
 *
 * $Id: RandomNumberFactory.h 1643 2012-06-20 20:57:09Z hoehna $
 */

#ifndef LogFunction_H
#define LogFunction_H

#include "ContinuousFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    class LogFunction : public ContinuousFunction {
        
    public:
        LogFunction(const TypedDagNode<double> *a, const TypedDagNode<double> *b);
        
        LogFunction*                        clone(void) const;                                                  //!< Create a clon.
        void                                update(void);                                                       //!< Recompute the value
        
    protected:
        void                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);    //!< Implementation of swaping parameters

    private:
        const TypedDagNode<double>*         a;
        const TypedDagNode<double>*         base;
    };
}

#endif
