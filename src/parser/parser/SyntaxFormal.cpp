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
SyntaxFormal::SyntaxFormal(RbPtr<RbString> id, RbPtr<SyntaxElement> defaultVal) : SyntaxElement(), argType(new TypeSpec(RbObject_name)), label(id), defaultExpr(defaultVal) {
    
    // Make argument rule from element
    if (defaultExpr == NULL)
        argRule = RbPtr<ArgumentRule>( new ValueRule(*label, *argType) );
    else
        argRule = RbPtr<ArgumentRule>( new ValueRule(*label, *argType, defaultExpr->evaluateContent( NULL )->getDagNode()) );

}


/** Constructor with explicit type */
SyntaxFormal::SyntaxFormal(RbPtr<RbString> typeSpec, RbPtr<RbString> id, RbPtr<SyntaxElement> defaultVal) : SyntaxElement(), argType(NULL), label(id), defaultExpr(defaultVal) {

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
    argType = RbPtr<TypeSpec>(new TypeSpec(tpName));
    
    // Make argument rule from element
    if (defaultExpr == NULL)
        argRule = RbPtr<ArgumentRule>( new ValueRule(*label, *argType) );
    else
        argRule = RbPtr<ArgumentRule>( new ValueRule(*label, *argType, defaultExpr->evaluateContent( NULL )->getDagNode()) );

}


/** Deep copy constructor */
SyntaxFormal::SyntaxFormal(const SyntaxFormal& x) : SyntaxElement(x) {

    argRule     = RbPtr<ArgumentRule>( x.argRule->clone() );
    argType     = RbPtr<TypeSpec>(new TypeSpec(*x.argType));
    label       = RbPtr<RbString>(new RbString(*(x.label)));
    defaultExpr = RbPtr<SyntaxElement>( x.defaultExpr->clone() );
    
}


/** Destructor deletes pointer members */
SyntaxFormal::~SyntaxFormal() {
    
}


/** Assignment operator */
SyntaxFormal& SyntaxFormal::operator=(const SyntaxFormal& x) {

    if (&x != this) {

        SyntaxElement::operator=(x);

        argRule     = x.argRule;
        argType     = x.argType;
        label       = x.label;
        defaultExpr = x.defaultExpr;
    }

    return (*this);
}


/** Return brief info about object */
std::string SyntaxFormal::briefInfo () const {

    std::ostringstream   o;

    if (defaultExpr == NULL)
        o << "SyntaxFormal:  '" << argType->toString() << " " << std::string(*label);
    else {
        o << "SyntaxFormal:  '" << argType->toString() << " " << std::string(*label) << " = ";
        o << defaultExpr->briefInfo();
    }

    return o.str();
}


/** Clone syntax element */
SyntaxFormal* SyntaxFormal::clone () const {

    return new SyntaxFormal(*this);
}



RbPtr<const ArgumentRule> SyntaxFormal::getArgumentRule(void ) const {
    return RbPtr<const ArgumentRule>( argRule );
}


RbPtr<const TypeSpec> SyntaxFormal::getArgumentTypeSpec(void) const {
    return RbPtr<const TypeSpec>( argType );
}


RbPtr<const RbString> SyntaxFormal::getLabel(void) const {
    return RbPtr<const RbString>( label );
}



/** Get class vector describing type of object */
const VectorString& SyntaxFormal::getClass(void) const { 
    
    static VectorString rbClass = VectorString(SyntaxFormal_name) + SyntaxElement::getClass();
	return rbClass; 
}


/** Get semantic value (not applicable so return NULL) */
RbPtr<Variable> SyntaxFormal::evaluateContent(RbPtr<Environment> env) {

    return RbPtr<Variable>::getNullPtr();
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& SyntaxFormal::getTypeSpec(void) const {
    return typeSpec;
}


/** Print info about the syntax element */
void SyntaxFormal::print(std::ostream& o) const {

    o << "SyntaxFormal:" << std::endl;
    o << "type        = " << argType->toString() << std::endl;
    o << "label       = " << *label << std::endl;
    o << "defaultExpr = " << defaultExpr->briefInfo() << std::endl;
}


