/**
* @file
* This file contains the declaration and implementation of the templated RevBayesCore function
* used as the internal deterministic node function object in user-defined functions.
*
* @author Fredrik Ronquist
* @license GPL version 3
*/

#ifndef TypedUserFunction_H
#define TypedUserFunction_H

#include "TypedFunction.h"
#include "UserFunctionCall.h"

#include <iostream>

namespace RevLanguage {
    
    template <class valueType>
    class TypedDagNode;
    
    template<class valueType>
    class TypedUserFunction : public RevBayesCore::TypedFunction<valueType> {
        
    public:
                                            TypedUserFunction(UserFunctionCall* uFC);                                   //!< Constructor setting user function call
                                            TypedUserFunction(const TypedUserFunction<valueType>& x);                   //!< Copy constructor
        virtual                            ~TypedUserFunction(void);                                                    //!< Virtual destructor
        
        // Public methods
        TypedUserFunction<valueType>*       clone(void) const;                                                          //!< Clone the function
        void                                touch(RevBayesCore::DagNode *toucher );                                     //!< Set dirty flag
        void                                update(void);                                                               //!< Update the value of the function
   
    protected:
        void                                swapParameterInternal(const RevBayesCore::DagNode *oldP, const RevBayesCore::DagNode *newP);    //!< Exchange a parameter
        
    private:
        UserFunctionCall*                   userFunctionCall;                                                            //!< The user function call
    
    };
    
}

#include "RlDeterministicNode.h"

using namespace RevLanguage;

template <class valueType>
TypedUserFunction<valueType>::TypedUserFunction(UserFunctionCall* uFC) :
    RevBayesCore::TypedFunction<valueType>( new valueType() ),
    userFunctionCall( uFC )
{
    // Add parameters to conform to RevBayesCore::Function
    std::vector<const RevBayesCore::DagNode*> params = userFunctionCall->getParameters();
    for ( std::vector<const RevBayesCore::DagNode*>::const_iterator it = params.begin(); it != params.end(); it++ )
        RevBayesCore::TypedFunction<valueType>::addParameter( (*it) );

    // Set dirty flag
    this->setDirty( true );
}


template <class valueType>
TypedUserFunction<valueType>::TypedUserFunction(const TypedUserFunction<valueType> &x) :
    RevBayesCore::TypedFunction<valueType>( x ),
    userFunctionCall( x.userFunctionCall->clone() )
{
    /* Just set dirty flag. No need to add parameters, it happens automatically. */
    this->setDirty(true);
}


template <class valueType>
TypedUserFunction<valueType>::~TypedUserFunction( void ) {
    
    // No need to delete parameters. The model is responsible for this.

    delete userFunctionCall;
}


template <class valueType>
TypedUserFunction<valueType>* TypedUserFunction<valueType>::clone( void ) const {
    
    return new TypedUserFunction<valueType>( *this );
}


template <class valueType>
void TypedUserFunction<valueType>::swapParameterInternal(const RevBayesCore::DagNode *oldP, const RevBayesCore::DagNode *newP) {
    
    // Nothing to do: the user function call only has reference variables to the arguments, so they change
    // automatically when the referenced variable changes
    
}


template <class valueType>
void TypedUserFunction<valueType>::touch( RevBayesCore::DagNode* toucher ) {
    
    this->setDirty( true );
}


template <class valueType>
void TypedUserFunction<valueType>::update( void ) {

    RevObject* retValue = userFunctionCall->execute();
    
    *(RevBayesCore::TypedFunction<valueType>::value) = static_cast< RevBayesCore::TypedDagNode<valueType>* >(retValue->getDagNode())->getValue();

    delete retValue;
    
    this->setDirty( false );
}


#endif
