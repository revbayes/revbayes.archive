/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func_vector, which is used to create
 * value vectors.
 *
 * @brief Declaration and implementation of Func_vector
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_vector_H
#define Func_vector_H

#include "RbFunction.h"
#include <map>
#include <string>

class DAGNode;

template <typename valType, typename retType>
class Func_vector :  public RbFunction {

    public:
        // Basic utility functions
        Func_vector*                clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value

    protected:
        RbPtr<RbLanguageObject>     executeFunction(const std::vector<const RbObject*>& args);  //!< Execute function
    
};

#endif

#include "ConstArgumentRule.h"
#include "Ellipsis.h"
#include "RbUtil.h"
#include "TypeSpec.h"


/** Clone object */
template <typename valType, typename retType>
Func_vector<valType, retType>* Func_vector<valType, retType>::clone( void ) const {

    return new Func_vector( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
template <typename valType, typename retType>
RbPtr<RbLanguageObject> Func_vector<valType, retType>::executeFunction( const std::vector<const RbObject*>& args ) {
    
    RbVector *theVector = new RbVector( valType::getClassTypeSpec() );
    for ( size_t i = 0; i < args.size(); i++ )
        theVector->push_back( static_cast<valType*>( args[i]->clone() ) );

    return theVector;
}


/** Get argument rules */
template <typename valType, typename retType>
const ArgumentRules& Func_vector<valType, retType>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules.push_back( new ConstArgumentRule( "", valType().getTypeSpec() ) );
        argumentRules.push_back( new Ellipsis (     valType().getTypeSpec() ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class name of object */
template <typename firstValType, typename secondValType>
const std::string& Func_vector<firstValType, secondValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func_vector<" + firstValType().getTypeSpec() + "," + secondValType().getTypeSpec() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename firstValType, typename secondValType>
const TypeSpec& Func_vector<firstValType, secondValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename firstValType, typename secondValType>
const TypeSpec& Func_vector<firstValType, secondValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename valType, typename retType>
const TypeSpec& Func_vector<valType, retType>::getReturnType( void ) const {

    return retType().getTypeSpec();
}

