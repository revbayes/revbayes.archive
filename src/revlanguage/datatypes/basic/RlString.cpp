/**
 * @file
 * This file contains the implementation of RlString, which is
 * a RevBayes wrapper around a regular RlString.
 *
 * @brief Implementation of RlString
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */
 
#include "ConstantNode.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RlString.h"
#include "TypeSpec.h"

using namespace RevLanguage;

/** Constructor from empty RlString */
RlString::RlString( void ) : RlModelVariableWrapper<std::string>() {
}


/** Constructor from RlString */
RlString::RlString(const std::string& v) : RlModelVariableWrapper<std::string>( new std::string(v) ) {
}


/** Constructor from int */
RlString::RlString(int i) : RlModelVariableWrapper<std::string>() {

    std::ostringstream o;
    o << i;
    value = new RevBayesCore::ConstantNode<std::string>("", new std::string(o.str()) );
}



/** Constructor from RlString */
RlString::RlString(double x) : RlModelVariableWrapper<std::string>() {

    std::ostringstream o;
    o << x;
    value = new RevBayesCore::ConstantNode<std::string>("", new std::string(o.str()) );
}


/** Clone function */
RlString* RevLanguage::RlString::clone() const {

	return new RlString(*this);
}


/** Get class name of object */
const std::string& RlString::getClassName(void) { 
    
    static std::string rbClassName = "String";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RlString::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& RlString::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Print value */
void RlString::printValue(std::ostream& o) const {

//	o << "\"" << value << "\"";
	o << value->getValue();
}

