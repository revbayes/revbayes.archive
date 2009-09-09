/*!
 * @file
 * This file contains the declaration of RbRateMatrix, which is
 * a REvBayes wrapper around an STL vector of RateMatrix.
 *
 * @brief Declaration of RbRateMatrix
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @implements RbDataType
 * @package datatypes
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#ifndef RbRateMatrix_H
#define RbRateMatrix_H

#include "RateMatrix.h"
#include "RbAbstractDataType.h"
#include <vector>

class RbRateMatrix : public RbAbstractDataType, public std::vector<RateMatrix> {

    public:
        RbRateMatrix() : RbAbstractDataType("RbRateMatrix"), std::vector<RateMatrix>() {}  //!< Default constructor
	    RbRateMatrix(const RateMatrix& v) : RbAbstractDataType("RbRateMatrix"), std::vector<RateMatrix>(1, v) {}             //!< Construct from RateMatrix
	    RbRateMatrix(const vector<RateMatrix>& v) : RbAbstractDataType("RbRateMatrix"), std::vector<RateMatrix>(v) {}    //!< Construct from vector<RateMatrix>
        RbRateMatrix(const RbRateMatrix& s);    //!< Copy constructor

	RbRateMatrix*   copy(void) const;                           //!< Copy
    int             getSize(void) const { return size(); }      //!< Get vector size
	void            print(std::ostream &c=std::cout) const;     //!< Print the value to ostream c
};

#endif
