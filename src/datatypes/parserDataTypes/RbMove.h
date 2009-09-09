/*!
 * @file
 * This file contains the declaration of RbMove, which is
 * a REvBayes wrapper around an STL vector of Move.
 *
 * @brief Declaration of RbMove
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @implements RbDataType
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#ifndef RbMove_H
#define RbMove_H

#include "Move.h"
#include "RbDataType.h"
#include <vector>

class RbMove : public RbDataType, public std::vector<Move> {

    public:
        RbMove() : RbDataType("RbMove"), std::vector<Move>() {}     //!< Default constructor
	    RbMove(const Move v) : RbDataType("RbMove"), std::vector<Move>(1, v) {}             //!< Construct from Move
	    RbMove(const vector<Move> &v) : RbDataType("RbMove"), std::vector<Move>(v) {}    //!< Construct from vector<Move>
        RbMove(const RbMove& s);    //!< Copy constructor

	RbMove* copy(void) const;                           //!< Get copy
	void    print(std::ostream &c=std::cout) const;     //!< Print the value to ostream c
};

#endif
