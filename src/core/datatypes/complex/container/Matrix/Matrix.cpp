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
Matrix::Matrix(const std::string& elemType) : Vector(TypeSpec(elemType)) {
}

/** Default constructor for matrix */
Matrix::Matrix(const std::string& elemType, const MemberRules& memberRules) : Vector(TypeSpec(elemType)) {
}


/** We only delegate to the base class */
Matrix::Matrix(const Matrix &m) : Vector(m) {
    
}


/** Get class vector describing type of object */
const VectorString& Matrix::getClass(void) const {

    static VectorString rbClass = VectorString(Matrix_name) + AbstractVector::getClass();
    return rbClass;
}


size_t Matrix::getNumberOfColumns(void) const {
    return static_cast<const Container*>(elements[0])->size();
}


size_t Matrix::getNumberOfRows(void) const {
    return elements.size();
}


/** Generic transpose function for all matrices */
void Matrix::transpose(void) {

}

