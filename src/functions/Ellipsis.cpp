/**
 * @file
 * This file contains the implementation of Ellipsis, which is
 * used to describe a dot dot dot formal (...).
 *
 * @brief Implementation of Ellipsis 
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#include "Ellipsis.h"

#include <sstream>


/** Provide complete information about object */
std::string Ellipsis::toString(void) const {

    std::ostringstream o;

    o << "Ellipsis: ";

    return o.str();
}


/** Print value for user (in descriptions of functions, for instance */
void Ellipsis::printValue(std::ostream &o) const {

    o << ". . .";
}


