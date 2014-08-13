#ifndef ScalarVectorDivision_H
#define ScalarVectorDivision_H

#include "StringUtilities.h"    // For string concatenation through Division
#include "TypedFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    
    /**
     * \brief Division between a scalar and a vector.
     *
     * Here we add a scalar to a vector as a function to enable the use in deterministic expressions.
     * This class has three template parameters to specify the corresponding types.
     * We completely rely on overloade operator/ functionality.
     *
     * \copyright (c) Copyright 2009-2013 (GPL version 3)
     * \author The RevBayes Development Core Team (Sebastian Hoehna)
     * \since Version 1.0, 2012-08-13
     *
     */
    template <class firstValueType, class secondValueType, class returnType>
    class ScalarVectorDivision : public TypedFunction<returnType> {
        
    public:
        ScalarVectorDivision(const TypedDagNode<firstValueType> *a, const TypedDagNode<secondValueType> *b);
        
        ScalarVectorDivision*                               clone(void) const;                                                          //!< Create a clon.
        void                                                update(void);                                                               //!< Recompute the value
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        const TypedDagNode<firstValueType>*                 a;
        const TypedDagNode<secondValueType>*                b;
        
    };
}



template<class firstValueType, class secondValueType, class returnType>
RevBayesCore::ScalarVectorDivision<firstValueType, secondValueType, returnType>::ScalarVectorDivision(const TypedDagNode<firstValueType> *l, const TypedDagNode<secondValueType> *r) : TypedFunction<returnType>( new returnType() ),
    a( l ),
    b( r )
{
    this->addParameter( l );
    this->addParameter( r );
    
}


template<class firstValueType, class secondValueType, class returnType>
RevBayesCore::ScalarVectorDivision<firstValueType, secondValueType, returnType>* RevBayesCore::ScalarVectorDivision<firstValueType, secondValueType, returnType>::clone( void ) const
{
    
    return new ScalarVectorDivision(*this);
}


template<class firstValueType, class secondValueType, class returnType>
void RevBayesCore::ScalarVectorDivision<firstValueType, secondValueType, returnType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
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
void RevBayesCore::ScalarVectorDivision<firstValueType, secondValueType, returnType>::update( void )
{
    // remove the old values
    
    size_t l = 0;
    if ( RbUtils::is_vector<firstValueType>::value )
    {
        l = RbUtils::sub_vector<firstValueType>::size( a->getValue() );
    }
    else if ( RbUtils::is_vector<secondValueType>::value )
    {
        l = RbUtils::sub_vector<secondValueType>::size( b->getValue() );
    }
    
    if ( this->value->size() != l )
    {
        this->value->resize( l );
    }
    
    for (size_t i = 0; i < l; ++i)
    {
        
        (*this->value)[i] = RbUtils::sub_vector<firstValueType>::getElement( a->getValue(), i ) / RbUtils::sub_vector<secondValueType>::getElement( b->getValue(), i );
    }
    
}


#endif
