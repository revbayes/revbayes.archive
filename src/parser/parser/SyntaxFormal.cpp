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


/** Constructor with implicit type */
SyntaxFormal::SyntaxFormal(RbString* id, SyntaxElement* defaultVal) : SyntaxElement(), argType(new TypeSpec(RbObject::getClassTypeSpec())), label(id), defaultExpr(defaultVal) {
    
    // Make argument rule from element
    if (defaultExpr == NULL)
        argRule = new ValueRule(label->getValue(), *argType);
    else {
        Environment env = Environment();
        argRule = new ValueRule(label->getValue(), *argType, defaultExpr->evaluateContent( env )->getDagNode() );
    }
}


/** Constructor with explicit type */
SyntaxFormal::SyntaxFormal(RbString* typeSpec, RbString* id, SyntaxElement* defaultVal) : SyntaxElement(), argType(NULL), label(id), defaultExpr(defaultVal) {

    // Convert to standard string
    const std::string typeString = typeSpec->getValue();

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
    argType = new TypeSpec( Workspace::userWorkspace().getClassTypeSpecOfType( typeString ) );
    
    // Make argument rule from element
    if (defaultExpr == NULL)
        argRule = new ValueRule(label->getValue(), *argType);
    else {
        Environment env = Environment();
        argRule = new ValueRule(label->getValue(), *argType, defaultExpr->evaluateContent(env)->getDagNode());
    }
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


/** Get class name of object */
const std::string& SyntaxFormal::getClassName(void) { 
    
    static std::string rbClassName = "Argument specification";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& SyntaxFormal::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( SyntaxElement::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& SyntaxFormal::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get semantic value (not applicable so return NULL) */
RbVariablePtr SyntaxFormal::evaluateContent( Environment& env ) {

    return RbVariablePtr( NULL );
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


/**
 * Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
 * We only change the default expression.
 * Some crazy kids might declare functions inside loops!?!
 */
void SyntaxFormal::replaceVariableWithConstant(const std::string& name, const RbLanguageObject& c) {
    
    defaultExpr->replaceVariableWithConstant(name, c);
    
}


