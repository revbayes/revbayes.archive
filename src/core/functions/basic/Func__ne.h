/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__ne, which is used to compare
 * two variables for inequality.
 *
 * @brief Declaration and implementof Func__ne
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__ne_H
#define Func__ne_H

#include "RbFunction.h"
#include "RbBoolean.h"

#include <map>
#include <string>

class DAGNode;

template <typename firstValType, typename secondValType>
class Func__ne :  public RbFunction {

    public:
        Func__ne( void );
        Func__ne( const Func__ne& f);
        virtual                    ~Func__ne( void );
    
        // overloaded operators
        Func__ne&                  operator=( const Func__ne& f);
    
        // Basic utility functions
        Func__ne*                   clone(void) const;                                          //!< Clone the object
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
Func__ne<firstValType, secondValType>::Func__ne( void ) : RbFunction( ) {
    first  = NULL;
    second = NULL;
}

/** default constructor */
template <typename firstValType, typename secondValType>
Func__ne<firstValType, secondValType>::Func__ne( const Func__ne& f ) : RbFunction( f ) {
    
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
Func__ne<firstValType, secondValType>::~Func__ne( void ) {
    
    if ( first != NULL && first->decrementReferenceCount() == 0 ) {
        delete first;
    }
    
    if ( second != NULL && second->decrementReferenceCount() == 0 ) {
        delete second;
    }
}


/** Overloaded assignment operator */
template <typename firstValType, typename secondValType>
Func__ne<firstValType,secondValType>& Func__ne<firstValType, secondValType>::operator=( Func__ne<firstValType, secondValType> const &f ) {
    
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
Func__ne<firstValType, secondValType>* Func__ne<firstValType, secondValType>::clone( void ) const {

    return new Func__ne( *this );
}


/** Execute function: We rely on operator overloading to provide the functionality */
template <typename firstValType, typename secondValType>
const RbLanguageObject& Func__ne<firstValType,secondValType>::executeFunction( void ) {

    const firstValType&  val1 = static_cast<const firstValType&> ( first->getValue()  );
    const secondValType& val2 = static_cast<const secondValType&>( second->getValue() );
    retValue.setValue( val1 != val2 );
    
    return retValue;
}


/** Get argument rules */
template <typename firstValType, typename secondValType>
const ArgumentRules& Func__ne<firstValType, secondValType>::getArgumentRules(void) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) {
        argumentRules.push_back( new ValueRule( "first", firstValType() .getTypeSpec() ) );
        argumentRules.push_back( new ValueRule( "second", secondValType().getTypeSpec() ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class name of object */
template <typename firstValType, typename secondValType>
const std::string& Func__ne<firstValType, secondValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func__ne<" + firstValType().getTypeSpec() + "," + secondValType().getTypeSpec() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__ne<firstValType, secondValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__ne<firstValType, secondValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__ne<firstValType, secondValType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbBoolean::getClassTypeSpec();
    
    return returnTypeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
template <typename firstValType, typename secondValType>
void Func__ne<firstValType, secondValType>::setArgumentVariable(std::string const &name, const Variable* var) {
    
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

