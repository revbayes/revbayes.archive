/**
 * @file
 * This file contains the implementation of Matrix,
 * a container type used to hold value matrices.
 *
 * @brief Implementation of Matrix
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-08-20, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#include "Matrix.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "VectorInteger.h"
#include "VectorString.h"

#include <sstream>


/** Default constructor for matrix */
Matrix::Matrix(const TypeSpec& elemType) : Vector( elemType ) {
}

/** Default constructor for matrix */
Matrix::Matrix(const TypeSpec& elemType, const MemberRules& memberRules) : Vector( elemType ) {
}


/** We only delegate to the base class */
Matrix::Matrix(const Matrix &m) : Vector(m) {
    
}


/** Get class name of object */
const std::string& Matrix::getClassName(void) { 
    
    static std::string rbClassName = "Matrix";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Matrix::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Vector::getClassTypeSpec() ) );
    
	return rbClass; 
}


size_t Matrix::getNumberOfColumns(void) const {
    return static_cast<const Container*>( elements[0] )->size();
}


size_t Matrix::getNumberOfRows(void) const {
    return elements.size();
}


/** Generic transpose function for all matrices */
void Matrix::transpose(void) {

}

