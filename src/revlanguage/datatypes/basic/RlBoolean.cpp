/**
 * @file
 * This file contains the implementation of RlBoolean, which is
 * a RevBayes wrapper around a regular bool.
 *
 * @brief Implementation of RlBoolean
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


#include "ConstantNode.h"
#include "RlBoolean.h"
#include "Real.h"
#include "Integer.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
RlBoolean::RlBoolean(void) : RlModelVariableWrapper<bool>() {

}

/** Construct from bool */
RlBoolean::RlBoolean(RevBayesCore::TypedDagNode<bool> *v) : RlModelVariableWrapper<bool>( v ) {
    
}

/** Construct from bool */
RlBoolean::RlBoolean(bool v) : RlModelVariableWrapper<bool>( new bool(v) ) {

}


/** Clone object */
RlBoolean* RlBoolean::clone(void) const {

	return new RlBoolean(*this);
}


/** Convert to type. The caller manages the returned object. */
RbLanguageObject* RlBoolean::convertTo(const TypeSpec& type) const {

    if (type == Integer::getClassTypeSpec()) {
        if (value->getValue())
            return new Integer(1);
        else 
            return new Integer(0);
    }
    else if (type == Real::getClassTypeSpec()) {
        if (value->getValue())
            return new Real(1.0);
        else 
            return new Real(0.0);
    }

    return RbLanguageObject::convertTo(type);
}


/** Get class name of object */
const std::string& RlBoolean::getClassName(void) { 
    
    static std::string rbClassName = "bool";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RlBoolean::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& RlBoolean::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Is convertible to type? */
bool RlBoolean::isConvertibleTo(const TypeSpec& type) const {

    if ( type == Integer::getClassTypeSpec() )
        return true;
    else if ( type == Real::getClassTypeSpec() )
        return true;

    return RbLanguageObject::isConvertibleTo(type);
}


/** Print value for user */
void RlBoolean::printValue(std::ostream &o) const {

    o << (value->getValue() ? "true" : "false");
}

