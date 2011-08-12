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
#include "RbNames.h"
#include "RbString.h"
#include "SyntaxFormal.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "Workspace.h"

#include <sstream>


/** Constructor with implicit type */
SyntaxFormal::SyntaxFormal(RbString* id, SyntaxElement* defaultVal)
    : SyntaxElement(), argType(new TypeSpec(RbObject_name)), label(id), defaultExpr(defaultVal) {
}


/** Constructor with explicit type */
SyntaxFormal::SyntaxFormal(RbString* typeSpec, RbString* id, SyntaxElement* defaultVal)
: SyntaxElement(), argType(NULL), label(id), defaultExpr(defaultVal) {

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
    
    // Avoid memory leak
    delete typeSpec;
}


/** Deep copy constructor */
SyntaxFormal::SyntaxFormal(const SyntaxFormal& x)
    : SyntaxElement(x) {

    argType     = new TypeSpec(x.argType->getType());
    label       = new RbString(*(x.label));
    defaultExpr = x.defaultExpr->clone();
}


/** Destructor deletes pointer members */
SyntaxFormal::~SyntaxFormal() {
    
    delete argType;
    delete label;
    delete defaultExpr;
}


/** Assignment operator */
SyntaxFormal& SyntaxFormal::operator=(const SyntaxFormal& x) {

    if (&x != this) {

        delete argType;
        delete label;
        delete defaultExpr;

        SyntaxElement::operator=(x);

        argType        = new TypeSpec(x.argType->getType());
        label       = new RbString(*(x.label));
        defaultExpr = x.defaultExpr->clone();
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


/** Make argument rule from element */
ArgumentRule* SyntaxFormal::getArgumentRule(Environment* env) const {


    if (defaultExpr == NULL)
        return new ValueRule(*label, *argType);
    else
        return new ValueRule(*label, *argType, defaultExpr->getContentAsVariable(env)->getDagNodePtr());

}



/** Get semantic value (not applicable so return NULL) */
Variable* SyntaxFormal::getContentAsVariable(Environment* env) const {

    return NULL;
}


/** Print info about the syntax element */
void SyntaxFormal::print(std::ostream& o) const {

    o << "SyntaxFormal:" << std::endl;
    o << "type        = " << argType->toString() << std::endl;
    o << "label       = " << *label << std::endl;
    o << "defaultExpr = " << defaultExpr->briefInfo() << std::endl;
}


