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

class DAGNode;
class VectorString;

template <typename matrixType>
class Func_transpose :  public RbFunction {

    public:
        // Basic utility functions
        Func_transpose*             clone(void) const;                                          //!< Clone the object
        const VectorString&         getClass(void) const;                                       //!< Get class vector
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
    	RbPtr<RbLanguageObject>     execute(void);                                              //!< Execute function
        const RbPtr<ArgumentRules>  getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
    
    private:
        static const TypeSpec       typeSpec;
};

#endif

#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"


// Definition of the static type spec member
template <typename matrixType>
const TypeSpec Func_transpose<matrixType>::typeSpec("Func_transpose", new TypeSpec(matrixType().getType()));


/** Clone object */
template <typename matrixType>
Func_transpose<matrixType>* Func_transpose<matrixType>::clone( void ) const {

    return new Func_transpose( *this );
}


/** Execute function by simply rearranging elements in new matrix of same type */
template <typename matrixType>
RbPtr<RbLanguageObject> Func_transpose<matrixType>::execute( void ) {

    const RbPtr<matrixType> mat = static_cast<matrixType*>( args[0]->getValue().get() );

    RbPtr<matrixType> matT( new matrixType( mat->getNumberOfColumns(), mat->getNumberOfRows() ) );
    
    for ( size_t i = 0; i < mat->getNumberOfRows(); i++ )
        for ( size_t j = 0; j < mat->getNumberOfColumns(); j++ )
            (*(*matT)[j])[i] = (*(*mat)[i])[j];

    return RbPtr<RbLanguageObject>( matT );
}


/** Get argument rules */
template <typename matrixType>
const RbPtr<ArgumentRules> Func_transpose<matrixType>::getArgumentRules( void ) const {

    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    static bool          rulesSet = false;

    if (!rulesSet) {

        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "", matrixType().getType() ) ) );

        rulesSet = true;
    }

    return argumentRules;
}


/** Get class vector describing type of object */
template <typename matrixType>
const VectorString& Func_transpose<matrixType>::getClass( void ) const {

    std::string         funcName = "Func_transpose<" + matrixType().getType() + ">"; 
    static VectorString rbClass  = VectorString( funcName ) + RbFunction::getClass();
    
    return rbClass;
}


/** Get return type */
template <typename matrixType>
const TypeSpec& Func_transpose<matrixType>::getReturnType( void ) const {

    return matrixType().getTypeSpec();
}


/** Get return type */
template <typename matrixType>
const TypeSpec& Func_transpose<matrixType>::getTypeSpec( void ) const {
    
    return typeSpec;
}

