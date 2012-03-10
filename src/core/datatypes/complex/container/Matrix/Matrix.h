/**
 * @file
 * This file contains the declaration of Matrix,
 * a container type used to hold value matrices.
 *
 * @brief Declaration of Matrix
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-08-20, version 1.0
 *
 * $Id$
 */

#ifndef Matrix_H
#define Matrix_H

#include "RbVector.h"
#include "Complex.h"

#include <iostream>
#include <string>
#include <vector>


/**
 * @brief Container class for matrices
 *
 * To allow convenient programmer access to
 * matrices, it may be best to implement them
 * as vector of vectors. A number of functions
 * that are inherited from Container and
 * Valuecontainer assume that the content of the
 * container is stored in the single elements
 * vector. To force programmers deriving from
 * the Matrix class to override these functions
 * if they decide to use vector of vectors to
 * represent the values internally, these
 * functions are declared as abstract here.
 */

template <typename valueType>
class Matrix : public RbVector<RbVector<valueType> > {

    public:
                                        Matrix();                                                           //!< Default constructor
                                        Matrix(size_t n, size_t k);                                         //!< Constructor for matrix with of dimension (n,k) filled with NULL pointers 
                                        Matrix(size_t n, size_t k, valueType* x);                           //!< Constructor for matrix with of dimension (n,k) filled with copies of element x
        virtual                        ~Matrix(void) {}                                                     //!< Virtual destructor
    
        // Basic utility functions you have to override
        virtual Matrix<valueType>*      clone(void) const;                                                  //!< Clone object
        static const std::string&       getClassName(void);                                                 //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                                             //!< Get class type spec
        virtual void                    printValue(std::ostream& o) const;                                  //!< Print value for user
   
        // Matrix functions you have to override
        virtual const RbObject&         getElement(size_t row, size_t col) const;                           //!< Get element or subcontainer
        virtual RbObject&               getElement(size_t row, size_t col);                                 //!< Get element or subcontainer (non-const to return non-const element)
        virtual void                    setElement(size_t row, size_t col, valueType* var);                 //!< set element
        virtual void                    resize(size_t rows, size_t cols);                                   //!< Resize to new length vector
        virtual void                    transpose(void);                                                    //!< Transpose matrix
    
        // Matrix functions you do not have to override
        size_t                          getNumberOfColumns(void) const;                                     //!< Get the number of columns in the matrix
        size_t                          getNumberOfRows(void) const;                                        //!< Get the number of rows in the matrix


};



/** Default constructor for matrix */
template <typename valueType>
Matrix<valueType>::Matrix( void ) : RbVector<RbVector<valueType> >(  ) {
}


/** Constructor with dimension (n,k) and NULL pointers to every object */
template <typename valueType>
Matrix<valueType>::Matrix(size_t n, size_t k) : RbVector<RbVector<valueType> >(  ) {
    
    for (size_t i = 0; i < n; i++) {
        this->push_back( new RbVector<valueType>(k) );
    }
}


/** Constructor with dimension (n,k) and NULL pointers to every object */
template <typename valueType>
Matrix<valueType>::Matrix(size_t n, size_t k, valueType* x) : RbVector<RbVector<valueType> >(  ) {
    
    for (size_t i = 0; i < n; i++) {
        this->push_back( new RbVector<valueType>(k, x->clone() ) );
    }
    
    delete x;
}


/** Clone the matrix. We rely on the copy constructor. */
template <typename valueType>
Matrix<valueType>* Matrix<valueType>::clone( void ) const {
    return new Matrix<valueType>( *this );
}

/** Get class name of object */
template <typename valueType>
const std::string& Matrix<valueType>::getClassName(void) { 
    
    static std::string rbClassName = "Matrix";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
template <typename valueType>
const TypeSpec& Matrix<valueType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbVector<RbVector<valueType> >::getClassTypeSpec() ), new TypeSpec( valueType::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get element of row and col. */
template <typename valueType>
const RbObject& Matrix<valueType>::getElement(size_t row, size_t col) const {
    return (this->elements[row])->getElement(col);
}


/** Get element of row and col. */
template <typename valueType>
RbObject& Matrix<valueType>::getElement(size_t row, size_t col) {
    return (this->elements[row])->getElement(col);
}

template <typename valueType>
size_t Matrix<valueType>::getNumberOfColumns(void) const {
    return static_cast<const Container*>( this->elements[0] )->size();
}


template <typename valueType>
size_t Matrix<valueType>::getNumberOfRows(void) const {
    return this->elements.size();
}


/** Print value for user */
template <typename valueType>
void Matrix<valueType>::printValue(std::ostream& o) const {
    
    // print the matrix with each column of equal width and each column centered on the decimal
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


/** Resize matrix */
template <typename valueType>
void Matrix<valueType>::resize( size_t n, size_t k ) {
    
    if ( n < this->elements.size() )
        throw RbException( "Invalid attempt to shrink matrix" );
    
    if ( k != this->elements[0]->size() ) {
        for ( size_t i = 0; i < this->elements.size(); i++ )
            this->elements[i]->resize( k );
    }
    for ( size_t i = this->elements.size(); i < n; i++ )
        this->elements.push_back( new RbVector<valueType>(k) );
}


/** Set element of row and col. */
template <typename valueType>
void Matrix<valueType>::setElement(size_t row, size_t col, valueType* x) {
    RbVector<valueType>* r = this->elements[row];
    r->setElement(col,x);
}


/** Generic transpose function for all matrices */
template <typename valueType>
void Matrix<valueType>::transpose(void) {
    
}


#endif
