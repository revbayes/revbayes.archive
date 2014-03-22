/**
 * @file
 * This file contains the declaration of the typed lookup function. It is used for dynamic lookup
 * of elements of containers, and for indirect references to variables. Lookup functions are
 * used in deterministic nodes created by Rev statements like
 *
 *    b := a
 *    b := a[1]
 *    b := a[i]
 *
 * @author Fredrik Ronquist
 * @license GPL version 3
 */

#ifndef TypedReferenceFunction_h
#define TypedReferenceFunction_h

#include "TypedFunction.h"

#include <iostream>

namespace RevBayesCore {
    
    template <class valueType>
    class TypedDagNode;
    
    template<class valueType>
    class TypedReferenceFunction : public TypedFunction<valueType> {
        
    public:
                                            TypedReferenceFunction(const TypedDagNode<valueType>* var );                //!< Constructor setting variable referenced
                                            TypedReferenceFunction(const TypedReferenceFunction &f);                    //!< Copy constructor
        virtual                            ~TypedReferenceFunction(void);                                               //!< Virtual destructor
        
        // pure virtual public methors
        TypedReferenceFunction<valueType>*  clone(void) const;                                                          //!< Clone the function
        void                                swapParameterInternal(const DagNode *oldP, const DagNode *newP) ;           //!< Exchange a parameter to the function
        void                                update(void);                                                               //!< Update the value of the function

    private:
        const TypedDagNode<valueType>*      variable;                                                                   //!< The variable we reference
    };
    
}



template <class valueType>
RevBayesCore::TypedReferenceFunction<valueType>::TypedReferenceFunction(const TypedDagNode<valueType>* var) :
TypedFunction<valueType>( Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( var->getValue() ), "<Reference Function>" ),
    variable( var )
{
    /* Add referenced variable as parent */
    this->addParameter( variable );
    
    /* Update value */
    update();
}


template <class valueType>
RevBayesCore::TypedReferenceFunction<valueType>::TypedReferenceFunction(const RevBayesCore::TypedReferenceFunction<valueType> &n) :
    TypedFunction<valueType>( n ),
    variable( n.variable )
{
    /* Just update value. No need to add parameters, it happens automatically. */
    update();
}


template <class valueType>
RevBayesCore::TypedReferenceFunction<valueType>::~TypedReferenceFunction( void ) {
    
    /* We don't delete the parameters, because they might be used elsewhere. The model is responsible for this. */
}


template <class valueType>
RevBayesCore::TypedReferenceFunction<valueType>* RevBayesCore::TypedReferenceFunction<valueType>::clone( void ) const {
    
    return new TypedReferenceFunction<valueType>( *this );
}


template <class valueType>
void RevBayesCore::TypedReferenceFunction<valueType>::update( void ) {
    
    TypedFunction<valueType>::value = Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( variable->getValue() );
}


template <class valueType>
void RevBayesCore::TypedReferenceFunction<valueType>::swapParameterInternal(const RevBayesCore::DagNode *oldP, const RevBayesCore::DagNode *newP) {
    
    
    if ( oldP == variable )
        variable = static_cast< const TypedDagNode<valueType>* >( newP );
}

#endif
