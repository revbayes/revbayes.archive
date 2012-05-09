/**
 * @file
 * This file contains the implemetation of RbMatrix,
 * a container type used to hold value matrices.
 *
 * @brief Implementation of RbMatrix
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-10 12:55:11 +0100 (Sat, 10 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-08-20, version 1.0
 *
 * $Id: RbMatrix.cpp 1327 2012-03-10 11:55:11Z hoehna $
 */

#include "RbMatrix.h"
#include "RbException.h"


/** Default constructor for RbMatrix */
RbMatrix::RbMatrix( const TypeSpec &elemType ) : RbVector( elemType ) {
}


/** Constructor with dimension (n,k) and NULL pointers to every object */
RbMatrix::RbMatrix(const TypeSpec &elemType, size_t n, size_t k) : RbVector( elemType ) {
    
    for (size_t i = 0; i < n; i++) {
        this->push_back( new RbVector(elemType, k) );
    }
}


/** Constructor with dimension (n,k) and NULL pointers to every object */
RbMatrix::RbMatrix(const TypeSpec &elemType, size_t n, size_t k, RbObject *x) : RbVector( elemType ) {
    
    for (size_t i = 0; i < n; i++) {
        this->push_back( new RbVector(elemType, k, x->clone() ) );
    }
    
    delete x;
}


/** Clone the RbMatrix. We rely on the copy constructor. */
RbMatrix* RbMatrix::clone( void ) const {
    return new RbMatrix( *this );
}

/** Get class name of object */
const std::string& RbMatrix::getClassName(void) { 
    
    static std::string rbClassName = "RbMatrix";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RbMatrix::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbVector::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get element of row and col. */
const RbObject& RbMatrix::getElement(size_t row, size_t col) const {
    return (this->elements[row])->getElement(col);
}


/** Get element of row and col. */
RbObject& RbMatrix::getElement(size_t row, size_t col) {
    return (this->elements[row])->getElement(col);
}

size_t RbMatrix::getNumberOfColumns(void) const {
    return static_cast<const Container*>( this->elements[0] )->size();
}


size_t RbMatrix::getNumberOfRows(void) const {
    return this->elements.size();
}


/** Print value for user */
void RbMatrix::printValue(std::ostream& o) const {
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i<this->size(); i++) {
        if (i == 0)
            o << "[ ";
        else 
            o << "  ";
        
        this->elements[i]->printValue(o);
        
        if (i == this->size()-1)
            o << " ]";
        else 
            o << " ,\n";
        
        //RBOUT(lineStr);
    }
}


/** Resize RbMatrix */
void RbMatrix::resize( size_t n, size_t k ) {
    
    if ( n < this->elements.size() )
        throw RbException( "Invalid attempt to shrink RbMatrix" );
    
    if ( k != static_cast<Container *>( this->elements[0] )->size() ) {
        for ( size_t i = 0; i < this->elements.size(); i++ )
            static_cast<RbVector *>( this->elements[i] )->resize( k );
    }
    for ( size_t i = this->elements.size(); i < n; i++ )
        this->elements.push_back( new RbVector(elementType, k) );
}


/** Set element of row and col. */
void RbMatrix::setElement(size_t row, size_t col, RbObject* x) {
    RbVector* r = static_cast<RbVector *>( this->elements[row] );
    r->setElement(col,x);
}


/** Generic transpose function for all matrices */
void RbMatrix::transpose(void) {
    throw RbException("Transpose not yet implemented in RbMatrix.");
}
