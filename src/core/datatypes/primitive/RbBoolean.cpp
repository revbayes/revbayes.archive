/**
 * @file
 * This file contains the implementation of RbBoolean, which is
 * a RevBayes wrapper around a regular bool.
 *
 * @brief Implementation of RbBoolean
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */


#include "RbBoolean.h"
#include "Real.h"
#include "Integer.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>


/** Default constructor */
RbBoolean::RbBoolean(void) : RbLanguageObject(), value(false) {

}

/** Construct from bool */
RbBoolean::RbBoolean(const bool v) : RbLanguageObject(), value(v) {

}


/** Overloaded equals operator */
bool RbBoolean::operator==(const RbBoolean &x) const {
    
    return value == x.value;
}


/** Clone object */
RbBoolean* RbBoolean::clone(void) const {

	return new RbBoolean(*this);
}


/** Convert to type. The caller manages the returned object. */
RbObject* RbBoolean::convertTo(const TypeSpec& type) const {

    if (type == Integer::getClassTypeSpec()) {
        if (value)
            return new Integer(1);
        else 
            return new Integer(0);
    }
    else if (type == Real::getClassTypeSpec()) {
        if (value)
            return new Real(1.0);
        else 
            return new Real(0.0);
    }

    return RbLanguageObject::convertTo(type);
}


/** Get class name of object */
const std::string& RbBoolean::getClassName(void) { 
    
    static std::string rbClassName = "Boolean";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RbBoolean::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& RbBoolean::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Is convertible to type? */
bool RbBoolean::isConvertibleTo(const TypeSpec& type) const {

    if ( type == Integer::getClassTypeSpec() )
        return true;
    else if ( type == Real::getClassTypeSpec() )
        return true;

    return RbLanguageObject::isConvertibleTo(type);
}


/** Print value for user */
void RbBoolean::printValue(std::ostream &o) const {

    o << (value ? "true" : "false");
}

