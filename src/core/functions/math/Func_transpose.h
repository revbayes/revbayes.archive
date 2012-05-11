/**
 * @file
 * This file contains the declaration of Func_transpose, which 
 * is a templated class for matrix transpose functions.
 *
 * @brief Declaration of Func_transpose
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_transpose_H
#define Func_transpose_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

template <typename matrixType>
class Func_transpose :  public RbFunction {

    public:
        // Basic utility functions
        Func_transpose*             clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value

    protected:
        RbPtr<RbLanguageObject>     executeFunction(void);                                      //!< Execute function

};

#endif

#include "ConstArgumentRule.h"
#include "TypeSpec.h"



/** Clone object */
template <typename matrixType>
Func_transpose<matrixType>* Func_transpose<matrixType>::clone( void ) const {

    return new Func_transpose( *this );
}


/** Execute function by simply rearranging elements in new matrix of same type */
template <typename matrixType>
RbPtr<RbLanguageObject> Func_transpose<matrixType>::executeFunction( void ) {

    const matrixType& mat = static_cast<const matrixType&>( args[0]->getVariable()->getValue() );

    matrixType *retValue = new matrixType( mat.getNumberOfColumns(), mat.getNumberOfRows() );
    
    for ( size_t i = 0; i < mat.getNumberOfRows(); i++ )
        for ( size_t j = 0; j < mat.getNumberOfColumns(); j++ )
            retValue[j][i] = mat[i][j];

    return retValue;
}


/** Get argument rules */
template <typename matrixType>
const ArgumentRules& Func_transpose<matrixType>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if (!rulesSet) {

        argumentRules.push_back( new ConstArgumentRule( "x", matrixType().getTypeSpec() ) );

        rulesSet = true;
    }

    return argumentRules;
}



/** Get class name of object */
template <typename matrixType>
const std::string& Func_transpose<matrixType>::getClassName(void) { 
    
    static std::string rbClassName = "Func_transpose<" + matrixType().getTypeSpec() + ">";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
template <typename matrixType>
const TypeSpec& Func_transpose<matrixType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
template <typename matrixType>
const TypeSpec& Func_transpose<matrixType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get return type */
template <typename matrixType>
const TypeSpec& Func_transpose<matrixType>::getReturnType( void ) const {

    return matrixType().getTypeSpec();
}

