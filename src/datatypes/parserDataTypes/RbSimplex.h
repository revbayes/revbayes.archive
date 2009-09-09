/*!
 * @file
 * This file contains the declaration of RbSimplex, which is
 * a REvBayes wrapper around an STL vector of Simplex.
 *
 * @brief Declaration of RbSimplex
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @implements RbDataType, RbAbstractDataType
 * @package datatypes
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#ifndef RbSimplex_H
#define RbSimplex_H

#include "RbAbstractDataType.h"
#include "Simplex.h"
#include <vector>


class RbSimplex : public RbAbstractDataType, public std::vector<Simplex> {

    public:
        RbSimplex() : RbAbstractDataType("RbSimplex"), vector<Simplex>() {}     //! Default constructor
	    RbSimplex(const Simplex& v) : RbAbstractDataType("RbSimplex"), vector<Simplex>(1, v) {}      //!< Constructor from Simplex
	    RbSimplex(const vector<Simplex> &v) : RbAbstractDataType("RbSimplex"), vector<Simplex>(v) {} //!< Constructor from vector<Simplex>
        RbSimplex(const RbSimplex& s);    //!< Copy constructor

	RbSimplex*  copy(void) const;                       //!< Copy
    int         getSize(void) const { return size(); }  //!< Get vector size
	void        print(ostream &c=cout) const;           //!< Print the value to ostream c
};

#endif
