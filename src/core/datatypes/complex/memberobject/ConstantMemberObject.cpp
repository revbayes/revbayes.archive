/**
 * @file
 * This file contains the partial implementation of ConstantMemberObject, an abstract
 * base class for complex objects with member methods and variables.
 *
 * @brief Partial implementation of ConstantMemberObject
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "ConstantMemberObject.h"
#include "ConstantNode.h"
#include "Integer.h"
#include "MemberFunction.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbUtil.h"
#include "VectorString.h"
#include "VariableNode.h"


/** Constructor: we set member variables here from member rules */
ConstantMemberObject::ConstantMemberObject() : MemberObject() {
    
}

/** Constructor: we set member variables here from member rules */
ConstantMemberObject::ConstantMemberObject(const MemberRules& memberRules) : MemberObject(memberRules) {
    
}


/** Get class vector describing type of object */
const VectorString& ConstantMemberObject::getClass(void) const {
    
    static VectorString rbClass = VectorString(ConstantMemberObject_name) + MemberObject::getClass();
    return rbClass;
}

/** Is the object constant? */
bool ConstantMemberObject::isConstant( void ) const {
    
    return true;
}




