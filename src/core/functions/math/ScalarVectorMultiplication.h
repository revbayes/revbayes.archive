#ifndef ScalarVectorMultiplication_H
#define ScalarVectorMultiplication_H

#include "RbVector.h"
#include "StringUtilities.h"    // For string concatenation through Multiplication
#include "TypedFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    
    /**
     * \brief Multiplication between a scalar and a vector.
     *
     * Here we add a scalar to a vector as a function to enable the use in deterministic expressions.
     * This class has three template parameters to specify the corresponding types.
     * We completely rely on overloade operator* functionality.
     *
     * \copyright (c) Copyright 2009-2013 (GPL version 3)
     * \author The RevBayes Development Core Team (Sebastian Hoehna)
     * \since Version 1.0, 2012-08-13
     *
     */
    template <class firstValueType, class secondValueType, class returnType>
    class ScalarVectorMultiplication : public TypedFunction< RbVector<returnType> > {
        
    public:
        ScalarVectorMultiplication(const TypedDagNode<firstValueType> *a, const TypedDagNode< RbVector<secondValueType> > *b);
        
        ScalarVectorMultiplication*                         clone(void) const;                                                          //!< Create a clon.
        void                                                update(void);                                                               //!< Recompute the value
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        const TypedDagNode<firstValueType>*                 a;
        const TypedDagNode< RbVector<secondValueType> >*  b;
        
    };
}



template<class firstValueType, class secondValueType, class returnType>
RevBayesCore::ScalarVectorMultiplication<firstValueType, secondValueType, returnType>::ScalarVectorMultiplication(const TypedDagNode<firstValueType> *l, const TypedDagNode< RbVector<secondValueType> > *r) : TypedFunction< RbVector<returnType> >( new RbVector<returnType>(r->getValue().size(),returnType()) ),
    a( l ),
    b( r )
{
    this->addParameter( l );
    this->addParameter( r );
    
}


template<class firstValueType, class secondValueType, class returnType>
RevBayesCore::ScalarVectorMultiplication<firstValueType, secondValueType, returnType>* RevBayesCore::ScalarVectorMultiplication<firstValueType, secondValueType, returnType>::clone( void ) const
{
    
    return new ScalarVectorMultiplication(*this);
}


template<class firstValueType, class secondValueType, class returnType>
void RevBayesCore::ScalarVectorMultiplication<firstValueType, secondValueType, returnType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == a)
    {
        a = static_cast<const TypedDagNode<firstValueType>* >( newP );
    }
    
    if (oldP == b)
    {
        b = static_cast<const TypedDagNode< RbVector<secondValueType> >* >( newP );
    }
}


template<class firstValueType, class secondValueType, class returnType>
void RevBayesCore::ScalarVectorMultiplication<firstValueType, secondValueType, returnType>::update( void )
{
    // remove the old values
    
    const firstValueType &lhs = a->getValue();
    const RbVector<secondValueType> &rhs = b->getValue();
    size_t l = rhs.size();
    for (size_t i=0; i<l; ++i)
    {
        (*this->value)[i] = lhs * rhs[i];
    }
    
}


#endif
