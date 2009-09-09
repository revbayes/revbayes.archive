/*!
 * @file
 * This file contains the declaration of RbReal, which is
 * a REvBayes wrapper around an STL vector of doubles.
 *
 * @brief Declaration of RbReal
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

#ifndef RbReal_H
#define RbReal_H

#include "RbAbstractDataType.h"
#include <vector>


class RbReal : public RbAbstractDataType, public std::vector<double> {

    public:
        RbReal() : RbAbstractDataType(RbReal::dataType), std::vector<double>() {}    //!< Default constructor
	    RbReal(const double v) : RbAbstractDataType(RbReal::dataType), std::vector<double>(1, v) {}          //!< Constructor from double
	    RbReal(const std::vector<double>& v) : RbAbstractDataType(RbReal::dataType), std::vector<double>(v) {}    //!< Constructor from vector<double>
        RbReal(const RbReal& s);    //!< Copy constructor

	RbReal* copy(void) const;                       //!< Copy
    int     getSize(void) const { return size(); }  //!< Get vector size
	void    print(std::ostream& c=cout) const;      //!< Print the value to ostream c

    static const std::string    dataType;           //!< The data type
};

#endif
