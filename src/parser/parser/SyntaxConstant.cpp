/**
 * @file
 * This file contains the implementation of SyntaxConstant, which is
 * used to hold constants in the syntax tree.
 *
 * @brief Implementation of SyntaxConstant
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include <iostream>

#include "ConstantNode.h"
#include "RbLanguageObject.h"
#include "SyntaxConstant.h"
#include "VectorString.h"


/** Construct from value */
SyntaxConstant::SyntaxConstant(RbLanguageObject* val) : SyntaxElement(), value(val) {
}


/** Deep copy constructor */
SyntaxConstant::SyntaxConstant(const SyntaxConstant& x) : SyntaxElement(x), value(NULL) {

    value = x.value->clone();
}


/** Destructor deletes value */
SyntaxConstant::~SyntaxConstant(void) {
    
    delete value;
}


/** Assignment operator deletes value and makes a clone of the value */
SyntaxConstant& SyntaxConstant::operator=(const SyntaxConstant& x) {

    if (this != &x) {

        SyntaxElement::operator=(x);
        
        delete value;
        value = x.value->clone();
        
    }

    return (*this);
}


/** Clone syntax element */
SyntaxConstant* SyntaxConstant::clone (void) const {

    return new SyntaxConstant(*this);
}


/** Get class name of object */
const std::string& SyntaxConstant::getClassName(void) { 
    
    static std::string rbClassName = "Constant";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& SyntaxConstant::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( SyntaxElement::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& SyntaxConstant::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get semantic value of element */
RbVariablePtr SyntaxConstant::evaluateContent( Environment& env ) {

    // We return a clone in case this function is called repeatedly.
    if (value == NULL)
        return RbVariablePtr( new Variable(new ConstantNode( NULL ) ) );
    else
        return RbVariablePtr( new Variable(new ConstantNode( value->clone() ) ) );
}


/** Is the expression constant?
 *  Of course we are!!!
 */
bool SyntaxConstant::isConstExpression(void) const {
    return true;
}


/** Print info about the syntax element */
void SyntaxConstant::printValue(std::ostream& o) const {

    o << "SyntaxConstant: value = ";
    value->printValue(o);
    o << std::endl;
}

