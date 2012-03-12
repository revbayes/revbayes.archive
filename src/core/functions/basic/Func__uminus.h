/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__uminus, which is used to perform
 * the unary minus operation.
 *
 * @brief Declaration and implementation of Func__uminus
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Func__uminus_H
#define Func__uminus_H

#include "RbFunction.h"

#include <string>

class RbLanguageObject;

template <typename valType, typename retType>
class Func__uminus :  public RbFunction {

    public:
                                    Func__uminus( void );
                                    Func__uminus( const Func__uminus& f);
        virtual                    ~Func__uminus( void );
    
        // overloaded operators
        Func__uminus&                  operator=( const Func__uminus& f);
    
        // Basic utility functions
        Func__uminus*               clone(void) const;                                          //!< Clone the object
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
        retType                     retValue;
};

#endif

#include "DAGNode.h"
#include "MatrixRealOperators.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"



/** default constructor */
template <typename firstValType, typename retType>
Func__uminus<firstValType, retType>::Func__uminus( void ) : RbFunction( ) {
    value = NULL;
}

/** default constructor */
template <typename firstValType, typename retType>
Func__uminus<firstValType, retType>::Func__uminus( const Func__uminus& f ) : RbFunction( f ) {
    
    value = f.value;
    if ( value != NULL ) {
        value->incrementReferenceCount();
    }
}

/** destructor */
template <typename firstValType, typename retType>
Func__uminus<firstValType, retType>::~Func__uminus( void ) {
    
    if ( value != NULL && value->decrementReferenceCount() == 0 ) {
        delete value;
    }
}


/** Overloaded assignment operator */
template <typename firstValType, typename retType>
Func__uminus<firstValType,retType>& Func__uminus<firstValType, retType>::operator=( Func__uminus<firstValType, retType> const &f ) {
    
    if ( this != &f ) {
        // call the base class assignment operator
        RbFunction::operator=( f );
        
        // free the memory first
        if ( value != NULL && value->decrementReferenceCount() == 0 ) {
            delete value;
        }
        
        // reassign the member variables
        value  = f.value;
        if ( value != NULL ) {
            value->incrementReferenceCount();
        }
    }
    
    return *this;
}


/** Clone object */
template <typename valType, typename retType>
Func__uminus<valType, retType>* Func__uminus<valType, retType>::clone( void ) const {

    return new Func__uminus( *this );
}


/** Execute function: We rely on operator overloading to provide the necessary functionality */
template <typename valType, typename retType>
const RbLanguageObject& Func__uminus<valType, retType>::executeFunction( void ) {

    const valType& val = static_cast<const valType&> ( value->getValue() );
             retValue = -( val );

    return retValue;
}


/** Get argument rules */
template <typename valType, typename retType>
const ArgumentRules& Func__uminus<valType, retType>::getArgumentRules( void ) const {

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
template <typename firstValType, typename secondValType>
const std::string& Func__uminus<firstValType, secondValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func__uminus<" + firstValType().getTypeSpec() + "," + secondValType().getTypeSpec() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__uminus<firstValType, secondValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__uminus<firstValType, secondValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename valType, typename retType>
const TypeSpec& Func__uminus<valType, retType>::getReturnType( void ) const {

    return retType().getTypeSpec();
}


/** We catch here the setting of the argument variables to store our parameters. */
template <typename firstValType, typename retType>
void Func__uminus<firstValType, retType>::setArgumentVariable(std::string const &name, const Variable* var) {
    
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

