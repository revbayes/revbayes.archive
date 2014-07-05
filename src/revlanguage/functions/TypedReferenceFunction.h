#ifndef TypedReferenceFunction_h
#define TypedReferenceFunction_h

#include "TypedFunction.h"

#include <iostream>

namespace RevBayesLanguage {
    
    template <class valueType>
    class TypedDagNode;
    
    /**
     * @brief TypedReferenceFunction: Indirect reference function
     *
     * TypedReferenceFunction is the templated function used for indirect
     * (deterministic) references to other variables. The reference function
     * is only used in deterministic nodes created by Rev statements like
     *
     *    b := a
     *
     * If you are referencing elements of a variable, or use any other expression in
     * the rhs of a deterministic assignment, the normal statement processing will
     * produce the appropriate deterministic node variable with a function inside
     * it.
     *
     * @author RevBayes Core Development Team
     * @license GPL version 3
     */
    template<class valueType>
    class TypedReferenceFunction : public RevBayesCore::TypedFunction<valueType> {
        
    public:
                                            TypedReferenceFunction(const RevBayesCore::TypedDagNode<valueType>* var );  //!< Constructor setting variable referenced
                                            TypedReferenceFunction(const TypedReferenceFunction &f);                    //!< Copy constructor
        virtual                            ~TypedReferenceFunction(void);                                               //!< Virtual destructor
        
        // Public methods
        TypedReferenceFunction<valueType>*  clone(void) const;                                                          //!< Clone the function
        void                                swapParameterInternal(const RevBayesCore::DagNode *oldP, const RevBayesCore::DagNode *newP) ;   //!< Exchange a parameter to the function
        void                                update(void);                                                               //!< Update the value of the function

    private:
        const RevBayesCore::TypedDagNode<valueType>*      variable;                                                     //!< The variable we reference
    };
    
}



template <class valueType>
RevBayesLanguage::TypedReferenceFunction<valueType>::TypedReferenceFunction(const RevBayesCore::TypedDagNode<valueType>* var) :
    RevBayesCore::TypedFunction<valueType>( RevBayesCore::Cloner<valueType, IsDerivedFrom<valueType, RevBayesCore::Cloneable>::Is >::createClone( var->getValue() ) ),
    variable( var )
{
    /* Add referenced variable as parent */
    this->addParameter( variable );
    
    /* Update value */
    update();
}


template <class valueType>
RevBayesLanguage::TypedReferenceFunction<valueType>::TypedReferenceFunction(const RevBayesLanguage::TypedReferenceFunction<valueType> &n) :
    RevBayesCore::TypedFunction<valueType>( n ),
    variable( n.variable )
{
    /* Just update value. No need to add parameters, it happens automatically. */
    update();
}


template <class valueType>
RevBayesLanguage::TypedReferenceFunction<valueType>::~TypedReferenceFunction( void ) {
    
    /* We don't delete the parameters, because they might be used elsewhere. The model is responsible for this. */
}


template <class valueType>
RevBayesLanguage::TypedReferenceFunction<valueType>* RevBayesLanguage::TypedReferenceFunction<valueType>::clone( void ) const {
    
    return new TypedReferenceFunction<valueType>( *this );
}


template <class valueType>
void RevBayesLanguage::TypedReferenceFunction<valueType>::update( void ) {
    
    if ( RevBayesCore::TypedFunction<valueType>::value != NULL )
        delete RevBayesCore::TypedFunction<valueType>::value;

    RevBayesCore::TypedFunction<valueType>::value = RevBayesCore::Cloner<valueType, IsDerivedFrom<valueType, RevBayesCore::Cloneable>::Is >::createClone( variable->getValue() );
}


template <class valueType>
void RevBayesLanguage::TypedReferenceFunction<valueType>::swapParameterInternal(const RevBayesCore::DagNode *oldP, const RevBayesCore::DagNode *newP) {
    
    
    if ( oldP == variable )
        variable = static_cast< const RevBayesCore::TypedDagNode<valueType>* >( newP );
}

#endif
