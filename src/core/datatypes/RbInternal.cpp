/**
 * @file
 * This file contains the implementation of some of the functions
 * in the abstract base class for internal language objects, RbInternal.
 *
 * @brief Partial implementation of RbInternal
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-02, version 1.0
 * @extends RbInternal
 *
 * $Id$
 */

#include "RbUtil.h"
#include "RbInternal.h"
#include "VectorString.h"

#include <sstream>


/** Get class vector describing type of object */
const VectorString& RbInternal::getClass(void) const { 

    static VectorString rbClass = VectorString(RbInternal_name);
	return rbClass; 
}


/** Print info about object simply by using richInfo function */
void RbInternal::printValue(std::ostream& o) const {
    
    o << "Instance of RbInternal:" << std::endl;
    o << getType() << std::endl;
}







