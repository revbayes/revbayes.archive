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
#include "RbNames.h"
#include "SyntaxClassDef.h"
#include "UserFunction.h"
#include "VectorString.h"
#include "Workspace.h"

#include <sstream>


/** Construct definition from class name, base class name, and variable and function definitions */
SyntaxClassDef::SyntaxClassDef( RbString* name, RbString* base, std::list<SyntaxElement*>* defs) : SyntaxElement(), className(name), baseClass(base), definitions(defs) {
    className->retain();
    baseClass->retain();
    for (std::list<SyntaxElement*>::iterator it=definitions->begin(); it!=definitions->end(); it++) {
        (*it)->retain();
    }
}


/** Deep copy constructor */
SyntaxClassDef::SyntaxClassDef(const SyntaxClassDef& x) : SyntaxElement(x) {

    className   = new RbString(*className);
    baseClass   = new RbString(*baseClass);
    definitions = new std::list<SyntaxElement*>();
    for (std::list<SyntaxElement*>::const_iterator i=x.definitions->begin(); i!=x.definitions->end(); i++)
        definitions->push_back((*i)->clone());
    
    className->retain();
    baseClass->retain();
    for (std::list<SyntaxElement*>::iterator it=definitions->begin(); it!=definitions->end(); it++) {
        (*it)->retain();
    }
 
}


/** Destructor deletes members */
SyntaxClassDef::~SyntaxClassDef() {
    
    // delete className;
    if (className != NULL) {
        className->release();
        if (className->isUnreferenced()) {
            delete className;
        }
    }
    
    // delete baseClass;
    if (baseClass != NULL) {
        baseClass->release();
        if (baseClass->isUnreferenced()) {
            delete baseClass;
        }
    }

    for (std::list<SyntaxElement*>::iterator i=definitions->begin(); i!=definitions->end(); i++)
        delete (*i);
    delete definitions;
}


/** Assignment operator */
SyntaxClassDef& SyntaxClassDef::operator=(const SyntaxClassDef& x) {

    if (&x != this) {
    
        SyntaxElement::operator=(x);

        delete className;
        delete baseClass;

        for (std::list<SyntaxElement*>::iterator i=definitions->begin(); i!=definitions->end(); i++)
            delete (*i);
        delete definitions;

        className   = new RbString(*className);
        baseClass   = new RbString(*baseClass);
        definitions = new std::list<SyntaxElement*>();
     
        for (std::list<SyntaxElement*>::const_iterator i=x.definitions->begin(); i!=x.definitions->end(); i++)
            definitions->push_back((*i)->clone());
    }

    return *this;
}


/** Return brief info about object */
std::string SyntaxClassDef::briefInfo () const {

    std::ostringstream   o;
    o << "SyntaxClassDef:  definition of a class called  " << std::string(*className);

    return o.str();
}


/** Clone syntax element */
SyntaxElement* SyntaxClassDef::clone () const {

    return (SyntaxElement*)(new SyntaxClassDef(*this));
}


/** Get semantic value: insert a user-defined class in the user workspace */
Variable* SyntaxClassDef::getContentAsVariable(Environment* env) const {

    std::cerr << "Sorry, user-defined classes are not implemented yet" << std::endl;

    // No return value 
    return NULL;
}


/** Print info about the syntax element */
void SyntaxClassDef::print(std::ostream& o) const {

    o << "SyntaxClassDef:" << std::endl;
    o << "className   = ";
    className->printValue(o);
    o << std::endl;
    o << "baseClass   = ";
    baseClass->printValue(o);
    o << std::endl;
    o << "defintions  = <" << definitions->size() << " definitions of member variables and functions>" << std::endl;
}


