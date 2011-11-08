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


// Definition of the static type spec member
const TypeSpec SyntaxConstant::typeSpec(SyntaxConstant_name);

/** Construct from value */
SyntaxConstant::SyntaxConstant(RbPtr<RbLanguageObject> val) : SyntaxElement(), value(val) {
}


/** Deep copy constructor */
SyntaxConstant::SyntaxConstant(const SyntaxConstant& x) : SyntaxElement(x), value(NULL) {

    value = RbPtr<RbLanguageObject>(x.value->clone());
}


/** Destructor deletes value */
SyntaxConstant::~SyntaxConstant(void) {
    
}


/** Assignment operator deletes value and makes a clone of the value */
SyntaxConstant& SyntaxConstant::operator=(const SyntaxConstant& x) {

    if (this != &x) {

        SyntaxElement::operator=(x);
        
        value = RbPtr<RbLanguageObject>( x.value->clone() );
        
    }

    return (*this);
}


/** Return brief info about object */
std::string SyntaxConstant::briefInfo (void) const {

    return "SyntaxConstant: value = " + value->briefInfo();
}


/** Clone syntax element */
SyntaxConstant* SyntaxConstant::clone (void) const {

    return new SyntaxConstant(*this);
}


/** Get class vector describing type of object */
const VectorString& SyntaxConstant::getClass(void) const { 
    
    static VectorString rbClass = VectorString(SyntaxConstant_name) + SyntaxElement::getClass();
	return rbClass; 
}


/** Get semantic value of element */
RbPtr<Variable> SyntaxConstant::evaluateContent(RbPtr<Environment> env) {

    // We return a clone in case this function is called repeatedly.
    if (value == NULL)
        return RbPtr<Variable>( new Variable(RbPtr<DAGNode>( new ConstantNode( RbPtr<RbObject>( NULL ) ) ) ) );
    else
        return RbPtr<Variable>( new Variable(RbPtr<DAGNode>( new ConstantNode(RbPtr<RbObject>( value ) ) ) ) );
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& SyntaxConstant::getTypeSpec(void) const {
    return typeSpec;
}


/** Print info about the syntax element */
void SyntaxConstant::print(std::ostream& o) const {

    o << "SyntaxConstant: value = " << value->briefInfo() << std::endl;
}

