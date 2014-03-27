/**
 * @file
 * This file contains the declaration and implementation of the templated function used
 * for indirect (deterministic) references to variables. The reference function is only
 * used in deterministic nodes created by Rev in statements like
 *
 *    b := a
 *
 * If you are referencing elements of a variable, or use any other expression in
 * the rhs of a deterministic assignment, the normal statement processing will 
 * produce the appropriate deterministic node variable.
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
    
    if ( TypedFunction<valueType>::value != NULL )
        delete TypedFunction<valueType>::value;

    TypedFunction<valueType>::value = Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( variable->getValue() );
}


template <class valueType>
void RevBayesCore::TypedReferenceFunction<valueType>::swapParameterInternal(const RevBayesCore::DagNode *oldP, const RevBayesCore::DagNode *newP) {
    
    
    if ( oldP == variable )
        variable = static_cast< const TypedDagNode<valueType>* >( newP );
}

#endif
