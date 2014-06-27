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

#include "ArgumentRule.h"
#include "Environment.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RlString.h"
#include "SyntaxFormal.h"
#include "TypeSpec.h"
#include "Workspace.h"

#include <sstream>

using namespace RevLanguage;

/** Constructor with implicit type */
SyntaxFormal::SyntaxFormal(const std::string &lbl, SyntaxElement* defaultVal) : SyntaxElement() , argType( TypeSpec(RevObject::getClassTypeSpec()) ), label(lbl), defaultExpr(defaultVal) {
    
    // Make argument rule from element
    if (defaultExpr == NULL)
        argRule = new ArgumentRule(label, true, argType);
    else {
        Environment env = Environment();
        argRule = new ArgumentRule(label, true, argType, defaultExpr->evaluateContent( env )->getValue().clone() );
    }
}


/** Constructor with explicit type */
SyntaxFormal::SyntaxFormal(const std::string &type, const std::string &lbl, SyntaxElement* defaultVal) : SyntaxElement(), argType( TypeSpec(RevObject::getClassTypeSpec()) ), label(lbl), defaultExpr(defaultVal) {

    // Count dimensions and check if reference
    int         nDim        = 0;
    std::string tpName      = std::string();
    for (std::string::const_iterator i=type.begin(); i!=type.end(); i++) {
        if ((*i) == '[')
            nDim++;
        else if ((*i) != ']')
            tpName += (*i);
    }

    // Create the type specification
    argType = TypeSpec( Workspace::userWorkspace().getClassTypeSpecOfType( type ) );
    
    // Make argument rule from element
    if (defaultExpr == NULL)
        argRule = new ArgumentRule(label, true, argType);
    else {
        Environment env = Environment();
        argRule = new ArgumentRule(label, true, argType, defaultExpr->evaluateContent(env)->getValue().clone() );
    }
}


/** Deep copy constructor */
SyntaxFormal::SyntaxFormal(const SyntaxFormal& x) : SyntaxElement(x) , argType( x.argType ){

    argRule     = x.argRule->clone();
    label       = x.label;
    defaultExpr = x.defaultExpr->clone();
    
}


/** Destructor deletes pointer members */
SyntaxFormal::~SyntaxFormal() {
    
    delete argRule;
    delete defaultExpr;
}


/** Assignment operator */
SyntaxFormal& SyntaxFormal::operator=(const SyntaxFormal& x) {

    if (&x != this) {

        SyntaxElement::operator=(x);

        argRule     = x.argRule->clone();
        argType     = x.argType;
        label       = x.label;
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


const TypeSpec& SyntaxFormal::getArgumentTypeSpec(void) const {
    return argType;
}


const std::string& SyntaxFormal::getLabel(void) const {
    return label;
}


/** Get semantic value (not applicable so return NULL) */
RbPtr<Variable> SyntaxFormal::evaluateContent( Environment& env ) {

    return NULL;
}


/** Print info about the syntax element */
void SyntaxFormal::printValue(std::ostream& o) const {

    o << "SyntaxFormal:" << std::endl;
    o << "type        = " << argType.toString() << std::endl;
    o << "label       = " << label << std::endl;
    o << "defaultExpr = ";
    defaultExpr->printValue(o);
    o << std::endl;
}


/**
 * Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
 * We only change the default expression.
 * Some crazy kids might declare functions inside loops!?!
 */
void SyntaxFormal::replaceVariableWithConstant(const std::string& name, const RevObject& c) {
    
    defaultExpr->replaceVariableWithConstant(name, c);
    
}


