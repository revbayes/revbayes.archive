/**
 * @file
 * This file contains the implementation of Func_distance, a class
 * used to construct a distance matrix.
 *
 * @brief Implementation of Func_distance
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "CharacterData.h"
#include "Func_distance.h"
#include "MatrixReal.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "UserInterface.h"
#include "ValueRule.h"
#include "VectorString.h"
#include <sstream>
#include <vector>


// Definition of the static type spec member
const TypeSpec Func_distance::typeSpec(Func_distance_name);
const TypeSpec Func_distance::returnTypeSpec(MatrixReal_name);

/** Clone object */
Func_distance* Func_distance::clone(void) const {

    return new Func_distance( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_distance::executeFunction(void) {

    // get the information from the arguments for reading the file
    RbPtr<CharacterData> m( static_cast<CharacterData*>( (RbObject*)(*args)[0]->getValue() ) );
    RbPtr<RbString> dName( static_cast<RbString*>( (RbObject*)(*args)[1]->getValue() ) );


    //MatrixReal::MatrixReal(const size_t nRows, const size_t nCols, double x) : Matrix(VectorReal_name) {
    RbPtr<MatrixReal> d( new MatrixReal(4, 4, 0.0) );

    return RbPtr<RbLanguageObject>( d );

    return RbPtr<RbLanguageObject>::getNullPtr();
}


/** Get argument rules */
RbPtr<const ArgumentRules> Func_distance::getArgumentRules(void) const {

    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    static bool rulesSet = false;

    if (!rulesSet)
        {
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "data",  CharacterData_name ) ) );
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "model", RbString_name      ) ) );
        rulesSet = true;
        }

    return RbPtr<const ArgumentRules>( argumentRules );
}


/** Get class vector describing type of object */
const VectorString& Func_distance::getClass(void) const {

    static VectorString rbClass = VectorString( Func_distance_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_distance::getReturnType(void) const {

    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_distance::getTypeSpec(void) const {

    return typeSpec;
}

