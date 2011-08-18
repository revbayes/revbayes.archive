/**
 * @file
 * This file contains the implementation of some of the functions
 * in the abstract base class for syntax elements.
 *
 * @brief Partial implementation of SyntaxElement
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-02, version 1.0
 *
 * $Id$
 */

#include "RbUtil.h"
#include "SyntaxElement.h"
#include "VectorString.h"


/** Get class vector describing type of object */
const VectorString& SyntaxElement::getClass(void) const { 

    static VectorString rbClass = VectorString(SyntaxElement_name) + RbInternal::getClass();
	return rbClass; 
}


