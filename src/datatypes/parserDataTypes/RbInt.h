/*!
 * @file
 * This file contains the declaration of RbInt, which is
 * a REvBayes wrapper around an STL vector of int.
 *
 * @brief Declaration of RbInt
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @implements RbDataType, RbAbstractDataType
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#ifndef RbInt_H
#define RbInt_H

#include "RbAbstractDataType.h"
#include <vector>

class RbInt : public RbAbstractDataType, public std::vector<int> {

    public:
        RbInt() : RbAbstractDataType(RbInt::dataType), std::vector<int>() {}     //!< Default constructor
	    RbInt(const int v) : RbAbstractDataType(RbInt::dataType), std::vector<int>(v) {}    //!< Construct from int
	    RbInt(const vector<int> &v) : RbAbstractDataType(RbInt::dataType), std::vector<int>(v) {}   //!< Construct from vector<int>
        RbInt(const RbInt& s);    //!< Copy constructor

	RbInt*  copy(void) const;                           //!< Copy
    int     getSize(void) const { return size(); }      //!< Get vector size
	void    print(std::ostream& c=std::cout) const;     //!< Print the value to ostream c

    static const string     dataType;                   //!< The data type
};

#endif
