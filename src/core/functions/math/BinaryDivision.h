#ifndef BinaryDivision_H
#define BinaryDivision_H

#include "TypedFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    /**
     * \brief Binary division.
     *
     * We compute the ratio a / b.
     *
     * \copyright (c) Copyright 2009-2015 (GPL version 3)
     * \author The RevBayes Development Core Team (Sebastian Hoehna)
     * \since Version 1.0, 2012-08-13
     *
     */
    template <class firstValueType, class secondValueType, class returnType>
    class BinaryDivision : public TypedFunction<returnType> {
        
    public:
        BinaryDivision(const TypedDagNode<firstValueType> *a, const TypedDagNode<secondValueType> *b);
        
        BinaryDivision*                         clone(void) const;                                                  //!< Create a clon.
        void                                    update(void);                                                       //!< Recompute the value
        
    protected:
        void                                    swapParameterInternal(const DagNode *oldP, const DagNode *newP);    //!< Implementation of swaping parameters
        
    private:
        const TypedDagNode<firstValueType>*     a;
        const TypedDagNode<secondValueType>*    b;
    };
}





template<class firstValueType, class secondValueType, class returnType>
RevBayesCore::BinaryDivision<firstValueType, secondValueType, returnType>::BinaryDivision(const TypedDagNode<firstValueType> *l, const TypedDagNode<secondValueType> *r) : TypedFunction<returnType>( new returnType() ), a( l ), b( r ) {
    this->addParameter( l );
    this->addParameter( r );
   
}


template<class firstValueType, class secondValueType, class returnType>
RevBayesCore::BinaryDivision<firstValueType, secondValueType, returnType>* RevBayesCore::BinaryDivision<firstValueType, secondValueType, returnType>::clone( void ) const {
    return new BinaryDivision(*this);
}

template<class firstValueType, class secondValueType, class returnType>
void RevBayesCore::BinaryDivision<firstValueType, secondValueType, returnType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == a)
    {
        a = static_cast<const TypedDagNode<firstValueType>* >( newP );
    }
    if (oldP == b)
    {
        b = static_cast<const TypedDagNode<secondValueType>* >( newP );
    }

}


template<class firstValueType, class secondValueType, class returnType>
void RevBayesCore::BinaryDivision<firstValueType, secondValueType, returnType>::update( void )
{
    firstValueType a_v = a->getValue();
    secondValueType b_v = b->getValue();
    *this->value = a_v / b_v;
}

#endif
