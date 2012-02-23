/**
 * @file
 * This file contains the implementation of SyntaxClassDef, which is
 * used to hold the definitions of user-defined types.
 *
 * @brief Implementation of SyntaxClassDef
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "RbException.h"
#include "RbUtil.h"
#include "SyntaxClassDef.h"
#include "Workspace.h"

#include <sstream>


/** Construct definition from class name, base class name, and variable and function definitions */
SyntaxClassDef::SyntaxClassDef( RbString* name, RbString* base, std::list<SyntaxElement*>* defs) : SyntaxElement(), className(name), baseClass(base), definitions(defs) {
    
}


/** Deep copy constructor */
SyntaxClassDef::SyntaxClassDef(const SyntaxClassDef& x) : SyntaxElement(x) {

    className = new RbString(*className);
    baseClass = new RbString(*baseClass);
    definitions = new std::list<SyntaxElement*>();
    for (std::list<SyntaxElement*>::const_iterator i=x.definitions->begin(); i!=x.definitions->end(); i++)
        definitions->push_back((*i)->clone());
 
}


/** Destructor deletes members */
SyntaxClassDef::~SyntaxClassDef() {
    
}


/** Assignment operator */
SyntaxClassDef& SyntaxClassDef::operator=(const SyntaxClassDef& x) {

    if (&x != this) {
    
        SyntaxElement::operator=(x);

        className   = x.className;
        baseClass   = x.baseClass;
        definitions = x.definitions;
     
    }

    return *this;
}


/** Clone syntax element */
SyntaxElement* SyntaxClassDef::clone () const {

    return (new SyntaxClassDef(*this));
}


/** Get class name of object */
const std::string& SyntaxClassDef::getClassName(void) { 
    
    static std::string rbClassName = "Class definition";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& SyntaxClassDef::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( SyntaxElement::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& SyntaxClassDef::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get semantic value: insert a user-defined class in the user workspace */
RbVariablePtr SyntaxClassDef::evaluateContent( Environment& env ) {

    std::cerr << "Sorry, user-defined classes are not implemented yet" << std::endl;

    // No return value 
    return NULL;
}


/** Print info about the syntax element */
void SyntaxClassDef::printValue(std::ostream& o) const {

    o << "SyntaxClassDef:" << std::endl;
    o << "className   = ";
    className->printValue(o);
    o << std::endl;
    o << "baseClass   = ";
    baseClass->printValue(o);
    o << std::endl;
    o << "defintions  = <" << definitions->size() << " definitions of member variables and functions>" << std::endl;
}


