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

#include "RbNumeric.h"
#include <vector>


class RbSimplex : public RbNumeric {

    public:
        RbSimplex();     //! Default constructor
	    RbSimplex(const RbSimplex& s);    //!< Copy constructor

};

#endif
