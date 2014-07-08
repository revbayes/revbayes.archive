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
        valueType&                          getValue(void);                                                             //!< Get value from Rev object
        const valueType&                    getValue(void) const;                                                       //!< Get const value from Rev object
        void                                touch(RevBayesCore::DagNode *toucher );                                     //!< Set dirty flag
        void                                update(void);                                                               //!< Update the value of the function
   
    protected:
        void                                swapParameterInternal(const RevBayesCore::DagNode *oldP, const RevBayesCore::DagNode *newP);    //!< Exchange a parameter
        
    private:
        UserFunctionCall*                   userFunctionCall;                                                           //!< The user function call
        RevPtr<Variable>                    returnVariable;                                                             //!< The object returned by the function
    };
    
}

#include "AbstractCharacterData.h"
#include "ContinuousCharacterData.h"
#include "RateMatrix.h"
#include "RateMatrix_JC.h"
#include "RlDeterministicNode.h"

using namespace RevLanguage;


/** Constructor from pointer to the call of a user-defined function, which we own */
template <class valueType>
TypedUserFunction<valueType>::TypedUserFunction(UserFunctionCall* uFC) :
    RevBayesCore::TypedFunction<valueType>( NULL ),
    userFunctionCall( uFC ),
    returnVariable( NULL )
{
    // Add parameters to conform to RevBayesCore::Function
    std::vector<const RevBayesCore::DagNode*> params = userFunctionCall->getParameters();
    for ( std::vector<const RevBayesCore::DagNode*>::const_iterator it = params.begin(); it != params.end(); it++ )
        RevBayesCore::TypedFunction<valueType>::addParameter( (*it) );

    // Set dirty flag
    this->setDirty( true );
}


/** Copy constructor */
template <class valueType>
TypedUserFunction<valueType>::TypedUserFunction(const TypedUserFunction<valueType> &x) :
    RevBayesCore::TypedFunction<valueType>( x ),
    userFunctionCall( x.userFunctionCall->clone() ),
    returnVariable( NULL )
{
    /* Just set dirty flag. No need to add parameters, it happens automatically. */
    this->setDirty( true );
}


/** Destructor: Delete userFunctionCall, which we own. The returnVariable is a smart pointer and will be deleted automatically. */
template <class valueType>
TypedUserFunction<valueType>::~TypedUserFunction( void )
{
    delete userFunctionCall;
}


/** Clone function: return type-safe clone of myself */
template <class valueType>
TypedUserFunction<valueType>* TypedUserFunction<valueType>::clone( void ) const
{
    return new TypedUserFunction<valueType>( *this );
}


/** Get value (const version), possibly after updating if the state is dirty */
template <class valueType>
const valueType& TypedUserFunction<valueType>::getValue(void) const
{
    if (this->isDirty() )
    {
        const_cast< TypedUserFunction<valueType>* >( this )->update();
        const_cast< TypedUserFunction<valueType>* >( this )->setDirty( false );     // just in case
    }

    return static_cast< RevBayesCore::TypedDagNode<valueType>* >( returnVariable->getRevObject().getDagNode() )->getValue();
}


/** Get value, possibly after updating if the state is dirty */
template <class valueType>
valueType& TypedUserFunction<valueType>::getValue(void)
{
    
    if ( this->isDirty() )
    {
        update();
        this->setDirty( false );    // just in case
    }
    
    return static_cast< RevBayesCore::TypedDagNode<valueType>* >( returnVariable->getRevObject().getDagNode() )->getValue();
}


/**
 * Swap internal parameters. Nothing to do in this class because it keeps reference variables to the arguments,
 * so they change automatically when the referenced variables change.
 */
template <class valueType>
void TypedUserFunction<valueType>::swapParameterInternal(const RevBayesCore::DagNode *oldP, const RevBayesCore::DagNode *newP)
{
    // Nothing to do: the user function call only has reference variables to the arguments, so they change
    // automatically when the referenced variable changes
}


/** Set our state to dirty for later update if somebody wants to see our value */
template <class valueType>
void TypedUserFunction<valueType>::touch( RevBayesCore::DagNode* toucher )
{
    this->setDirty( true );
}


/** Update our internal return variable and set state to clean */
template <class valueType>
void TypedUserFunction<valueType>::update( void )
{
    returnVariable = userFunctionCall->execute();
    
    this->setDirty( false );
}


#endif
