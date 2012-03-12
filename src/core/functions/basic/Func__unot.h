/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__unot, which is used to perform
 * the unary RbBoolean not (!) operation.
 *
 * @brief Declaration and implementation of Func__unot
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Func__unot_H
#define Func__unot_H

#include "RbFunction.h"
#include "RbBoolean.h"

#include <string>

class DAGNode;

template <typename valType>
class Func__unot :  public RbFunction {

    public:
                                    Func__unot( void );
                                    Func__unot( const Func__unot& f);
        virtual                    ~Func__unot( void );
    
        // overloaded operators
        Func__unot&                  operator=( const Func__unot& f);
    
        // Basic utility functions
        Func__unot*                 clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value

    protected:
        const RbLanguageObject&     executeFunction(void);                                      //!< Execute function
        void                        setArgumentVariable(const std::string& name, const Variable* var);

    private:
    
        // Arguments
        const Variable*             value;
    
        // function return value
        RbBoolean                   retValue;
};

#endif

#include "RbBoolean.h"
#include "DAGNode.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"



/** default constructor */
template <typename valType>
Func__unot<valType>::Func__unot( void ) : RbFunction( ) {
    value = NULL;
}

/** default constructor */
template <typename valType>
Func__unot<valType>::Func__unot( const Func__unot& f ) : RbFunction( f ) {
    
    value = f.value;
    if ( value != NULL ) {
        value->incrementReferenceCount();
    }
}

/** destructor */
template <typename valType>
Func__unot<valType>::~Func__unot( void ) {
    
    if ( value != NULL && value->decrementReferenceCount() == 0 ) {
        delete value;
    }
}


/** Overloaded assignment operator */
template <typename valType>
Func__unot<valType>& Func__unot<valType>::operator=( Func__unot<valType> const &f ) {
    
    if ( this != &f ) {
        // call the base class assignment operator
        RbFunction::operator=( f );
        
        // free the memory first
        if ( value != NULL && value->decrementReferenceCount() == 0 ) {
            delete value;
        }
        
        // reassign the member variables
        value = f.value;
        if ( value != NULL ) {
            value->incrementReferenceCount();
        }
    }
    
    return *this;
}


/** Clone object */
template <typename valType>
Func__unot<valType>* Func__unot<valType>::clone( void ) const {

    return new Func__unot( *this );
}


/** Execute function: We rely on operator overloading to provide the necessary functionality */
template <typename valType>
const RbLanguageObject& Func__unot<valType>::executeFunction( void ) {

    const valType& val = static_cast<const valType&> ( value->getValue() );
    retValue = ! (val);

    return retValue;
}


/** Get argument rules */
template <typename valType>
const ArgumentRules& Func__unot<valType>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules.push_back( new ValueRule( "value", valType().getTypeSpec() ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class name of object */
template <typename firstValType>
const std::string& Func__unot<firstValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func__unot<" + firstValType().getTypeSpec() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename firstValType>
const TypeSpec& Func__unot<firstValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename firstValType>
const TypeSpec& Func__unot<firstValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename firstValType>
const TypeSpec& Func__unot<firstValType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbBoolean::getClassTypeSpec();
    
    return returnTypeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
template <typename firstValType>
void Func__unot<firstValType>::setArgumentVariable(std::string const &name, const Variable* var) {
    
    if ( name == "value" ) {
        // free the memory of the old variable
        // Variable uses reference counting so we need to free the memory manually
        if ( value != NULL && value->decrementReferenceCount() == 0 ) {
            delete value;
        }
        
        // set my variable to the new variable
        value = var;
        
        // increment the reference count for the variable
        if (value != NULL ) {
            value->incrementReferenceCount();
        }
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}

