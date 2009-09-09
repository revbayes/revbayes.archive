/*!
 * @file
 * This file contains the declaration of RbLogic, which is
 * a REvBayes wrapper around an stl vector of bool.
 *
 * @brief Declaration of RbLogic
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

#ifndef RbLogic_H
#define RbLogic_H

#include "RbAbstractDataType.h"
#include <vector>

using namespace std;

class RbLogic : public RbAbstractDataType, public std::vector<bool> {

    public:
        RbLogic() : RbAbstractDataType("RbLogic"), vector<bool>() {}    //!< Default constructor
	    RbLogic(const bool v) : RbAbstractDataType("RbLogic"), vector<bool>(v) {}   //!< Constructor from bool
	    RbLogic(const vector<bool> &v) : RbAbstractDataType("RbLogic"), vector<bool>(v) {}  //!< Constructor from vector<bool>
        RbLogic(const RbLogic& s);    //!< Copy constructor

	RbLogic*    copy(void) const;                       //!< Copy
    int         getSize(void) const { return size(); }  //!< Get size of vector
	void        print(ostream &c=cout) const;           //!< Print the value to ostream c
};

#endif
