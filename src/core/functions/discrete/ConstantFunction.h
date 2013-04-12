/**
 * @file
 * This file contains the declaration of the constant value function, f(x) = a.
 *
 * @brief Declaration of the constant value functions.
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

#ifndef ConstantFunction_H
#define ConstantFunction_H

#include "TypedFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    template <class valueType>
    class ConstantFunction : public TypedFunction<valueType> {
        
    public:
        ConstantFunction(const TypedDagNode<valueType> *a);
        
        ConstantFunction*                       clone(void) const;                                                  //!< Create a clon.
        void                                    update(void);                                                       //!< Recompute the value
        
    protected:
        void                                    swapParameterInternal(const DagNode *oldP, const DagNode *newP);    //!< Implementation of swaping parameters

    private:
        const TypedDagNode<valueType>*          x;
    };
}





template<class valueType>
RevBayesCore::ConstantFunction<valueType>::ConstantFunction(const TypedDagNode<valueType> *a) : TypedFunction<valueType>(new valueType(a->getValue()) ), x( a ) {
    
}


template<class valueType>
RevBayesCore::ConstantFunction<valueType>* RevBayesCore::ConstantFunction<valueType>::clone( void ) const {
    return new ConstantFunction(*this);
}


template <class valueType>
void RevBayesCore::ConstantFunction<valueType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == x) {
        x = static_cast<const TypedDagNode<double>* >( newP );
    }
}


template<class valueType>
void RevBayesCore::ConstantFunction<valueType>::update( void ) {
    *this->value =  x->getValue();
}

#endif
