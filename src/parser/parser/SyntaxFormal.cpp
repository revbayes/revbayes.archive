/**
 * @file
 * This file contains the implementation of SyntaxFormal, which is
 * used to hold formal argument specifications in the syntax tree.
 *
 * @brief Implementation of SyntaxFormal
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "Environment.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "SyntaxFormal.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "Workspace.h"

#include <sstream>


// Definition of the static type spec member
const TypeSpec SyntaxFormal::typeSpec(SyntaxFormal_name);

/** Constructor with implicit type */
SyntaxFormal::SyntaxFormal(RbString* id, SyntaxElement* defaultVal) : SyntaxElement(), argType(new TypeSpec(RbObject_name)), label(id), defaultExpr(defaultVal) {
    
    // Make argument rule from element
    if (defaultExpr == NULL)
        argRule = new ValueRule(*label, *argType);
    else
        argRule = new ValueRule(*label, *argType, defaultExpr->evaluateContent()->getDagNode() );

}


/** Constructor with explicit type */
SyntaxFormal::SyntaxFormal(RbString* typeSpec, RbString* id, SyntaxElement* defaultVal) : SyntaxElement(), argType(NULL), label(id), defaultExpr(defaultVal) {

    // Convert to standard string
    const std::string typeString = *typeSpec;

    // Count dimensions and check if reference
    int         nDim        = 0;
    std::string tpName      = std::string();
    for (std::string::const_iterator i=typeString.begin(); i!=typeString.end(); i++) {
        if ((*i) == '[')
            nDim++;
        else if ((*i) != ']')
            tpName += (*i);
    }

    // Create the type specification
    argType = new TypeSpec(tpName);
    
    // Make argument rule from element
    if (defaultExpr == NULL)
        argRule = new ValueRule(*label, *argType);
    else
        argRule = new ValueRule(*label, *argType, defaultExpr->evaluateContent()->getDagNode());

}


/** Deep copy constructor */
SyntaxFormal::SyntaxFormal(const SyntaxFormal& x) : SyntaxElement(x) {

    argRule     = x.argRule->clone();
    argType     = new TypeSpec(*x.argType);
    label       = new RbString(*(x.label));
    defaultExpr = x.defaultExpr->clone();
    
}


/** Destructor deletes pointer members */
SyntaxFormal::~SyntaxFormal() {
    
    delete argRule;
    delete argType;
    delete label;
    delete defaultExpr;
}


/** Assignment operator */
SyntaxFormal& SyntaxFormal::operator=(const SyntaxFormal& x) {

    if (&x != this) {

        SyntaxElement::operator=(x);

        argRule     = x.argRule->clone();
        argType     = new TypeSpec(*x.argType);
        label       = x.label->clone();
        defaultExpr = x.defaultExpr->clone();
    }

    return (*this);
}


/** Clone syntax element */
SyntaxFormal* SyntaxFormal::clone () const {

    return new SyntaxFormal(*this);
}



const ArgumentRule* SyntaxFormal::getArgumentRule(void ) const {
    return argRule;
}


ArgumentRule* SyntaxFormal::getArgumentRule(void ) {
    return argRule;
}


const TypeSpec* SyntaxFormal::getArgumentTypeSpec(void) const {
    return argType;
}


const RbString* SyntaxFormal::getLabel(void) const {
    return label;
}



/** Get class vector describing type of object */
const VectorString& SyntaxFormal::getClass(void) const { 
    
    static VectorString rbClass = VectorString(SyntaxFormal_name) + SyntaxElement::getClass();
	return rbClass; 
}


/** Get semantic value (not applicable so return NULL) */
Variable* SyntaxFormal::evaluateContent(void) {

    return NULL;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& SyntaxFormal::getTypeSpec(void) const {
    return typeSpec;
}


/** Print info about the syntax element */
void SyntaxFormal::printValue(std::ostream& o) const {

    o << "SyntaxFormal:" << std::endl;
    o << "type        = " << argType->toString() << std::endl;
    o << "label       = " << *label << std::endl;
    o << "defaultExpr = ";
    defaultExpr->printValue(o);
    o << std::endl;
}


