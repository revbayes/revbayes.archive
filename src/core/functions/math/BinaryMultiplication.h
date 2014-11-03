/**
 * @file
 * This file contains the declaration of the binary multiplication function, f(x) = a(x) * b(x).
 *
 * @brief Declaration of functions.
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

#ifndef BinaryMultiplication_H
#define BinaryMultiplication_H

#include "TypedFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    template <class firstValueType, class secondValueType, class returnType>
    class BinaryMultiplication : public TypedFunction<returnType> {
        
    public:
        BinaryMultiplication(const TypedDagNode<firstValueType> *a, const TypedDagNode<secondValueType> *b);
        
        BinaryMultiplication*                   clone(void) const;                                                  //!< Create a clon.
        void                                    update(void);                                                       //!< Recompute the value
        
    protected:
        void                                    swapParameterInternal(const DagNode *oldP, const DagNode *newP);    //!< Implementation of swaping parameters
        
    private:
        const TypedDagNode<firstValueType>*     a;
        const TypedDagNode<secondValueType>*    b;
    };
}

template <class firstValueType, class secondValueType, class returnType>
RevBayesCore::BinaryMultiplication<firstValueType, secondValueType, returnType>::BinaryMultiplication(const TypedDagNode<firstValueType> *l, const TypedDagNode<secondValueType> *r) : TypedFunction<returnType>( new returnType() ), a( l ), b( r ) {
    this->addParameter( l );
    this->addParameter( r );

}


template <class firstValueType, class secondValueType, class returnType>
RevBayesCore::BinaryMultiplication<firstValueType, secondValueType, returnType>* RevBayesCore::BinaryMultiplication<firstValueType, secondValueType, returnType>::clone( void ) const {
    return new BinaryMultiplication(*this);
}

template<class firstValueType, class secondValueType, class returnType>
void RevBayesCore::BinaryMultiplication<firstValueType, secondValueType, returnType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == a) {
        a = static_cast<const TypedDagNode<firstValueType>* >( newP );
    }
    if (oldP == b) {
        b = static_cast<const TypedDagNode<secondValueType>* >( newP );
    }
}


template <class firstValueType, class secondValueType, class returnType>
void RevBayesCore::BinaryMultiplication<firstValueType, secondValueType, returnType>::update( void ) {
    *this->value = a->getValue() * b->getValue();
}

#endif
