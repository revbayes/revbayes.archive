/**
 * @file
 * This file contains the declaration of MatrixReal, a complex type
 * used to hold double matrices.
 *
 * @brief Declaration of MatrixReal
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-08-20, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#ifndef MatrixReal_H
#define MatrixReal_H

#include "RbComplex.h"
#include "VectorInteger.h"

#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Real matrix (vector of vectors)
 *
 * This real matrix class is implemented as a vector of vectors.
 *
 */
class MatrixReal : public RbComplex {

    public:
                                    MatrixReal(void) : RbComplex(), nRows(0), nCols(0) {}                       //!< Default constructor (empty matrix)
                                    MatrixReal(const size_t nRows, const size_t nCols, const double x = 0.0);   //!< Construct matrix containing double x

        // Basic utility functions
        virtual MatrixReal*         clone(void) const;                                          //!< Clone object
        virtual bool                equals(const RbObject* obj) const;                          //!< Equals comparison
        virtual const VectorString& getClass(void) const;                                       //!< Get class vector
        void                        printValue(std::ostream& o) const;                          //!< Print value for user
        virtual std::string         toString(void) const;                                       //!< Complete info about object

        // Element access functions for parser
	    int                         getDim(void) const { return 1; }                            //!< Get subscript dimensions
        virtual const std::string&  getElementType(void) const;                                 //!< Get element type
        const RbObject*             getElement(const VectorInteger& index) const;               //!< Get element (read-only)
        const VectorInteger&        getLength(void) const;                                      //!< Get length in each dim
        virtual void                resize(const VectorInteger& len);                           //!< Resize
        virtual void                setElement(const VectorInteger& index, RbObject* val);      //!< Set element
        void                        setLength(const VectorInteger& len);                        //!< Set length in each dim

        // Overloaded operators and built-in functions
        std::vector<double>&        operator[](size_t i) { return value[i]; }                   //!< Index op allowing change
        const std::vector<double>&  operator[](size_t i) const { return value[i]; }             //!< Const index op

	protected:
        virtual RbObject*           convertTo(const std::string& type) const;                  //!< Convert to type
        virtual bool                isConvertibleTo(const std::string& type) const;            //!< Is convertible to type and dim?

    private:
        std::vector<std::vector<double> >   value;                                             //!< Vector of vector of values
        size_t                              nRows;                                             //!< Number of rows
        size_t                              nCols;                                             //!< Number of columns
};

#endif
