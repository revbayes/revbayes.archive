/**
 * @file
 * This file contains the partial implementation of Distribution, the atbstract
 * base class for distributions in RevBayes.
 *
 * @brief Partial implementation of Distribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @interface Distribution
 *
 * $Id$
 */

#include "Distribution.h"
#include "RbDouble.h"
#include "RbNames.h"
#include "StringVector.h"


/** Constructor passes member rules to base class */
Distribution::Distribution(const ArgumentRule** memberRules)
    : MemberObject(memberRules) {
}


/** Get class vector describing type of object */
const StringVector& Distribution::getClass(void) const {

    static StringVector rbClass = StringVector(Distribution_name) + MemberObject::getClass();
    return rbClass;
}


