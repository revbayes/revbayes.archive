/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__uplus, which is used to perform
 * the unary plus operation.
 *
 * @brief Declaration and implementation of Func__uplus
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Func__uplus_H
#define Func__uplus_H

#include "RbFunction.h"

#include <string>

class DAGNode;

template <typename valType, typename retType>
class Func__uplus :  public RbFunction {

public:
                                    Func__uplus( void );
                                    Func__uplus( const Func__uplus& f);
        virtual                    ~Func__uplus( void );
    
        // overloaded operators
        Func__uplus&                operator=( const Func__uplus& f);
    
        // Basic utility functions
        Func__uplus*                clone(void) const;                                          //!< Clone the object
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
};

#endif

#include "RbLanguageObject.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"



/** default constructor */
template <typename firstValType, typename retType>
Func__uplus<firstValType, retType>::Func__uplus( void ) : RbFunction( ) {
    value = NULL;
}

/** default constructor */
template <typename firstValType, typename retType>
Func__uplus<firstValType, retType>::Func__uplus( const Func__uplus& f ) : RbFunction( f ) {
    
    value = f.value;
    if ( value != NULL ) {
        value->incrementReferenceCount();
    }
}

/** destructor */
template <typename firstValType, typename retType>
Func__uplus<firstValType, retType>::~Func__uplus( void ) {
    
    if ( value != NULL && value->decrementReferenceCount() == 0 ) {
        delete value;
    }
}


/** Overloaded assignment operator */
template <typename firstValType, typename retType>
Func__uplus<firstValType,retType>& Func__uplus<firstValType, retType>::operator=( Func__uplus<firstValType, retType> const &f ) {
    
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
Func__uplus<valType, retType>* Func__uplus<valType, retType>::clone( void ) const {

    return new Func__uplus( *this );
}


/** Execute function: We simply return a copy of the value */
template <typename valType, typename retType>
const RbLanguageObject& Func__uplus<valType, retType>::executeFunction( void ) {

    const valType& val = static_cast<const valType&> ( value->getValue() );

    return val;
}


/** Get argument rules */
template <typename valType, typename retType>
const ArgumentRules& Func__uplus<valType, retType>::getArgumentRules( void ) const {

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
const std::string& Func__uplus<firstValType, secondValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func__uplus<" + firstValType().getType() + "," + secondValType().getType() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__uplus<firstValType, secondValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename firstValType, typename secondValType>
const TypeSpec& Func__uplus<firstValType, secondValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename valType, typename retType>
const TypeSpec& Func__uplus<valType, retType>::getReturnType( void ) const {

    return retType().getTypeSpec();
}


/** We catch here the setting of the argument variables to store our parameters. */
template <typename firstValType, typename retType>
void Func__uplus<firstValType, retType>::setArgumentVariable(std::string const &name, const Variable* var) {
    
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

