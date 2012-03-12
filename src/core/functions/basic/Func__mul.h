/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__mul, which is used to multiply
 * two variables.
 *
 * @brief Declaration and implementation of Func__mul
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__mul_H
#define Func__mul_H

#include "RbFunction.h"

#include <map>
#include <string>

class DAGNode;

template <typename firstValType, typename secondValType, typename retType>
class Func__mul :  public RbFunction {

    public:
                                    Func__mul( void );
                                    Func__mul( const Func__mul& f);
        virtual                    ~Func__mul( void );
    
        // overloaded operators
        Func__mul&                  operator=( const Func__mul& f);
    
        // Basic utility functions
        Func__mul*                  clone(void) const;                                          //!< Clone the object
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
        retType                     retValue;
};

#endif

#include "DAGNode.h"
#include "Integer.h"
#include "MatrixRealOperators.h"
#include "RbException.h"
#include "RbUtil.h"
#include "Real.h"
#include "TypeSpec.h"
#include "ValueRule.h"



/** default constructor */
template <typename firstValType, typename secondValType, typename retType>
Func__mul<firstValType, secondValType, retType>::Func__mul( void ) : RbFunction( ) {
    first  = NULL;
    second = NULL;
}

/** default constructor */
template <typename firstValType, typename secondValType, typename retType>
Func__mul<firstValType, secondValType, retType>::Func__mul( const Func__mul& f ) : RbFunction( f ) {
    
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
template <typename firstValType, typename secondValType, typename retType>
Func__mul<firstValType, secondValType, retType>::~Func__mul( void ) {
    
    if ( first != NULL && first->decrementReferenceCount() == 0 ) {
        delete first;
    }
    
    if ( second != NULL && second->decrementReferenceCount() == 0 ) {
        delete second;
    }
}


/** Overloaded assignment operator */
template <typename firstValType, typename secondValType, typename retType>
Func__mul<firstValType,secondValType,retType>& Func__mul<firstValType, secondValType, retType>::operator=( Func__mul<firstValType, secondValType, retType> const &f ) {
    
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
template <typename firstValType, typename secondValType, typename retType>
Func__mul<firstValType, secondValType, retType>* Func__mul<firstValType, secondValType, retType>::clone( void ) const {

    return new Func__mul( *this );
}


/** Execute function: We rely on operator overloading to provide the necessary functionality */
template <typename firstValType, typename secondValType, typename retType>
const RbLanguageObject& Func__mul<firstValType,secondValType,retType>::executeFunction( void ) {

    const firstValType&  val1 = static_cast<const firstValType&> ( first->getValue() );
    const secondValType& val2 = static_cast<const secondValType&>( second->getValue() );

                  retValue = val1 * val2;
        
    return retValue;
}


/** Get argument rules */
template <typename firstValType, typename secondValType, typename retType>
const ArgumentRules& Func__mul<firstValType, secondValType, retType>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules.push_back( new ValueRule( "first",  firstValType() .getTypeSpec() ) );
        argumentRules.push_back( new ValueRule( "second", secondValType().getTypeSpec() ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class name of object */
template <typename firstValType, typename secondValType, typename retType>
const std::string& Func__mul<firstValType, secondValType, retType>::getClassName(void) { 
    
    static std::string rbClassName = "Func__mul<" + firstValType().getTypeSpec() + "," + secondValType().getTypeSpec() + "," + retType().getTypeSpec() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename firstValType, typename secondValType, typename retType>
const TypeSpec& Func__mul<firstValType, secondValType, retType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename firstValType, typename secondValType, typename retType>
const TypeSpec& Func__mul<firstValType, secondValType, retType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename firstValType, typename secondValType, typename retType>
const TypeSpec& Func__mul<firstValType, secondValType, retType>::getReturnType(void) const {

    return retType().getTypeSpec();
}


/** We catch here the setting of the argument variables to store our parameters. */
template <typename firstValType, typename secondValType, typename retType>
void Func__mul<firstValType, secondValType, retType>::setArgumentVariable(std::string const &name, const Variable* var) {
    
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

