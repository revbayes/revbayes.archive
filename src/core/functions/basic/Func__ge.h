/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__ge, which is used to determine
 * whether the first variable is greater than or equal to
 * the second.
 *
 * @brief Declaration and implementof Func__ge
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__ge_H
#define Func__ge_H

#include "RbFunction.h"
#include "RbBoolean.h"

#include <map>
#include <string>

class DAGNode;

template <typename firstValType, typename secondValType>
class Func__ge :  public RbFunction {

    public:
                                    Func__ge( void );
                                    Func__ge( const Func__ge& f);
        virtual                    ~Func__ge( void );
    
        // overloaded operators
        Func__ge&                  operator=( const Func__ge& f);
    
        // Basic utility functions
        Func__ge*                   clone(void) const;                                          //!< Clone the object
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
        const Variable*             first;
        const Variable*             second;
    
        // function return value
        RbBoolean                   retValue;
};

#endif

#include "RbBoolean.h"
#include "DAGNode.h"
#include "Integer.h"
#include "RbException.h"
#include "RbUtil.h"
#include "Real.h"
#include "TypeSpec.h"
#include "ValueRule.h"



/** default constructor */
template <typename firstValType, typename secondValType>
Func__ge<firstValType, secondValType>::Func__ge( void ) : RbFunction( ) {
    first  = NULL;
    second = NULL;
}

/** default constructor */
template <typename firstValType, typename secondValType>
Func__ge<firstValType, secondValType>::Func__ge( const Func__ge& f ) : RbFunction( f ) {
    
    first  = f.first;
    if ( first != NULL ) {
        first->incrementReferenceCount();
    }
    second = f.second;
    if ( second != NULL ) {
        second->incrementReferenceCount();
    }
}

/** destructor */
template <typename firstValType, typename secondValType>
Func__ge<firstValType, secondValType>::~Func__ge( void ) {
    
    if ( first != NULL && first->decrementReferenceCount() == 0 ) {
        delete first;
    }
    
    if ( second != NULL && second->decrementReferenceCount() == 0 ) {
        delete second;
    }
}


/** Overloaded assignment operator */
template <typename firstValType, typename secondValType>
Func__ge<firstValType,secondValType>& Func__ge<firstValType, secondValType>::operator=( Func__ge<firstValType, secondValType> const &f ) {
    
    if ( this != &f ) {
        // call the base class assignment operator
        RbFunction::operator=( f );
        
        // free the memory first
        if ( first != NULL && first->decrementReferenceCount() == 0 ) {
            delete first;
        }
        
        if ( second != NULL && second->decrementReferenceCount() == 0 ) {
            delete second;
        }
        
        // reassign the member variables
        first  = f.first;
        if ( first != NULL ) {
            first->incrementReferenceCount();
        }
        
        second = f.second;
        if ( second != NULL ) {
            second->incrementReferenceCount();
        }
    }
    
    return *this;
}


/** Clone object */
template <typename firstValType, typename secondValType>
Func__ge<firstValType, secondValType>* Func__ge<firstValType, secondValType>::clone( void ) const {

    return new Func__ge( *this );
}


/** Execute function: We rely on operator overloading to provide the functionality */
template <typename firstValType, typename secondValType>
const RbLanguageObject& Func__ge<firstValType,secondValType>::executeFunction( void ) {

    const firstValType&  val1 = static_cast<const firstValType&> ( first->getValue()  );
    const secondValType& val2 = static_cast<const secondValType&>( second->getValue() );
    retValue.setValue( val1 >= val2 );
    
    return retValue;
}


/** Get argument rules */
template <typename firstValType, typename secondValType>
const ArgumentRules& Func__ge<firstValType, secondValType>::getArgumentRules(void) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) {
        argumentRules.push_back( new ValueRule( "first",  firstValType() .getTypeSpec() ) );
        argumentRules.push_back( new ValueRule( "second", secondValType().getTypeSpec() ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class name of object */
template <typename firstValType, typename secondValType>
const std::string& Func__ge<firstValType, secondValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func__ge<" + firstValType().getTypeSpec() + "," + secondValType().getTypeSpec() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__ge<firstValType, secondValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__ge<firstValType, secondValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__ge<firstValType, secondValType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbBoolean::getClassTypeSpec();
    
    return returnTypeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
template <typename firstValType, typename secondValType>
void Func__ge<firstValType, secondValType>::setArgumentVariable(std::string const &name, const Variable* var) {
    
    if ( name == "first" ) {
        // free the memory of the old variable
        // Variable uses reference counting so we need to free the memory manually
        if ( first != NULL && first->decrementReferenceCount() == 0 ) {
            delete first;
        }
        
        // set my variable to the new variable
        first = var;
        
        // increment the reference count for the variable
        if (first != NULL ) {
            first->incrementReferenceCount();
        }
    }
    else if ( name == "second" ) {
        // free the memory of the old variable
        // Variable uses reference counting so we need to free the memory manually
        if ( second != NULL && second->decrementReferenceCount() == 0 ) {
            delete second;
        }
        
        // set my variable to the new variable
        second = var;
        
        // increment the reference count for the variable
        if (second != NULL ) {
            second->incrementReferenceCount();
        }
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}

