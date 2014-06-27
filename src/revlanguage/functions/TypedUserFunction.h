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

namespace RevBayesCore {
    
    template <class valueType>
    class TypedDagNode;
    
    template<class valueType>
    class TypedUserFunction : public TypedFunction<valueType> {
        
    public:
                                            TypedUserFunction(RevLanguage::UserFunction* uF, std::vector<RevLanguage::Argument> args);  //!< Constructor setting user function and its arguments
                                            TypedUserFunction(const TypedUserFunction<valueType>& f);                   //!< Copy constructor
        virtual                            ~TypedUserFunction(void);                                                    //!< Virtual destructor
        
        // Public methods
        TypedUserFunction<valueType>*       clone(void) const;                                                          //!< Clone the function
        void                                update(void);                                                               //!< Update the value of the function
        void                                swapParameterInternal(const DagNode *oldP, const DagNode *newP) {}          //!< Exchange a parameter
        
    private:
        RevLanguage::UserFunction*          userFunction;                                                               //!< The user function
    };
    
}



template <class valueType>
RevBayesCore::TypedUserFunction<valueType>::TypedUserFunction(RevLanguage::UserFunction* uF, std::vector<RevLanguage::Argument> args) :
    TypedFunction<valueType>( new valueType(), "<Rev Function>" ),
    userFunction( uF->clone() )
{
    
    for ( std::vector<RevLanguage::Argument>::iterator it=args.begin(); it!=args.end(); ++it )
        this->addParameter( it->getVariable()->getRevObject().getDagNode() );
    
    /* Update value */
    update();
}


template <class valueType>
RevBayesCore::TypedUserFunction<valueType>::TypedUserFunction(const RevBayesCore::TypedUserFunction<valueType> &n) :
    TypedFunction<valueType>( n ),
    userFunction( n.userFunction->clone() )
{
    /* Just update value. No need to add parameters, it happens automatically. */
    update();
}


template <class valueType>
RevBayesCore::TypedUserFunction<valueType>::~TypedUserFunction( void ) {
    
    /* We don't delete the parameters, because they might be used elsewhere. The model is responsible for this. */

    /* We need to delete our copy of the user function */
    delete userFunction;
}


template <class valueType>
RevBayesCore::TypedUserFunction<valueType>* RevBayesCore::TypedUserFunction<valueType>::clone( void ) const {
    
    return new TypedUserFunction<valueType>( *this );
}


template <class valueType>
void RevBayesCore::TypedUserFunction<valueType>::update( void ) {

    // We can rely on the fact that only objects with value nodes have a TypedUserFunction
    
    RevLanguage::RevObject* retValue = userFunction->executeCode();
    
    if(TypedFunction<valueType>::value != NULL)
        delete TypedFunction<valueType>::value;
    
    *(TypedFunction<valueType>::value) = static_cast< RevBayesCore::TypedDagNode<valueType>* >(userFunction->executeCode()->getDagNode())->getValue();

    delete retValue;
}


#endif
