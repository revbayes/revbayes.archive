/**
 * @file
 * This file contains the partial implementation of RbComplex,
 * which is the RevBayes abstract base class for complex objects.
 *
 * @brief Partial implementation of RbComplex
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-08-23 15:04:35 +0200 (Sön, 23 Aug 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 *
 * $Id: RbDataType.h 9 2009-08-23 13:04:35Z ronquist $
 */

#include "ContainerIterator.h"
#include "DAGNode.h"
#include "RbComplex.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "VectorInteger.h"
#include "VectorString.h"

#include <map>
#include <string>
#include <ostream>
#include <sstream>
#include <vector>


/** Get class vector describing type of object */
const VectorString& RbComplex::getClass(void) const { 

    static VectorString rbClass = VectorString(RbComplex_name) + RbObject::getClass();
	return rbClass;
}


/** Are we of specified object type? We override the function in RbObject because
 *  we are not of scalar type, that is, there is no match for Scalar_name.
 */
bool RbComplex::isType(const std::string& type) const {

    const VectorString& classVec = getClass();

    for (size_t i=0; i<classVec.size(); i++) {
        if (type == classVec[i])
            return true;
    }

	return false;
}

