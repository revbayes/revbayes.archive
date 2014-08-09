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
#include "UserFunction.h"

#include <iostream>

namespace RevLanguage {
    
    template <class valueType>
    class TypedDagNode;
    
    template<class valueType>
    class TypedUserFunction : public RevBayesCore::TypedFunction<valueType> {
        
    public:
                                            TypedUserFunction(UserFunction* fxn);                                       //!< Constructor setting user function call
                                            TypedUserFunction(const TypedUserFunction<valueType>& x);                   //!< Copy constructor
        virtual                            ~TypedUserFunction(void);                                                    //!< Virtual destructor
        
        TypedUserFunction&                  operator=(const TypedUserFunction& x);                                      //!< Assignment operator

        // Public methods
        TypedUserFunction<valueType>*       clone(void) const;                                                          //!< Clone the function
        valueType&                          getValue(void);                                                             //!< Get value from Rev object
        const valueType&                    getValue(void) const;                                                       //!< Get const value from Rev object
        void                                touch(RevBayesCore::DagNode *toucher );                                     //!< Set dirty flag
        void                                update(void);                                                               //!< Update the value of the function
   
    protected:
        void                                swapParameterInternal(const RevBayesCore::DagNode *oldP, const RevBayesCore::DagNode *newP);    //!< Exchange a parameter
        
    private:
        UserFunction*                       userFunction;                                                               //!< The user function
        RevPtr<Variable>                    returnVariable;                                                             //!< The object returned by the function
    };
    
}

#include "AbstractCharacterData.h"
#include "ContinuousCharacterData.h"
#include "RlDeterministicNode.h"
#include "Variable.h"

using namespace RevLanguage;


/** Constructor from pointer to a user-defined function, which we own */
template <class valueType>
TypedUserFunction<valueType>::TypedUserFunction(UserFunction* fxn) :
    RevBayesCore::TypedFunction<valueType>( NULL ),
    userFunction( fxn ),
    returnVariable( NULL )
{
    // Set dirty flag
    this->setDirty( true );
    
    // Add parameters for double book-keeping
    const std::set<const RevBayesCore::DagNode*>& parameters = userFunction->getParameters();
    for ( std::set<const RevBayesCore::DagNode*>::const_iterator it = parameters.begin(); it != parameters.end(); ++it )
        this->addParameter( (*it ) );
}


/** Copy constructor */
template <class valueType>
TypedUserFunction<valueType>::TypedUserFunction(const TypedUserFunction<valueType> &x) :
    RevBayesCore::TypedFunction<valueType>( x ),
    userFunction( x.userFunction->clone() ),
    returnVariable( NULL )
{
    /* Just set dirty flag. No need to add parameters, it happens automatically. */
    this->setDirty( true );

    // Add parameters for double book-keeping
    const std::set<const RevBayesCore::DagNode*>& parameters = userFunction->getParameters();
    for ( std::set<const RevBayesCore::DagNode*>::const_iterator it = parameters.begin(); it != parameters.end(); ++it )
        this->addParameter( (*it ) );
}


/** Destructor: Delete userFunction, which we own. The returnVariable is a smart pointer and will be deleted automatically. */
template <class valueType>
TypedUserFunction<valueType>::~TypedUserFunction( void )
{
    delete userFunction;
}


/** Assignment operator */
template <class valueType>
TypedUserFunction<valueType>& TypedUserFunction<valueType>::operator=(const TypedUserFunction<valueType>& x)
{
    if ( this != &x )
    {
        RevBayesCore::TypedFunction<valueType>::operator=( &x );
        
        /* Delete old user function and get a clone of the new one */
        delete userFunction;
        userFunction = x.userFunction->clone();

        // Parameter reassignment should be automatic
    }
    
    return ( *this );
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
    
    // The base class takes care of the parameters that are part of the double book-keeping
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
    returnVariable = userFunction->executeCode();
    
    this->setDirty( false );
}


#endif
