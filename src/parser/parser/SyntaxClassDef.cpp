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
#include "UserFunction.h"
#include "VectorString.h"
#include "Workspace.h"

#include <sstream>


// Definition of the static type spec member
const TypeSpec SyntaxClassDef::typeSpec(SyntaxClassDef_name);

/** Construct definition from class name, base class name, and variable and function definitions */
SyntaxClassDef::SyntaxClassDef( RbPtr<RbString> name, RbPtr<RbString> base, RbPtr<std::list<RbPtr<SyntaxElement> > > defs) : SyntaxElement(), className(name), baseClass(base), definitions(defs) {
    
}


/** Deep copy constructor */
SyntaxClassDef::SyntaxClassDef(const SyntaxClassDef& x) : SyntaxElement(x) {

    className = RbPtr<RbString>(new RbString(*className));
    baseClass = RbPtr<RbString>(new RbString(*baseClass));
    definitions = RbPtr<std::list<RbPtr<SyntaxElement> > >(new std::list<RbPtr<SyntaxElement> >() );
    for (std::list<RbPtr<SyntaxElement> >::const_iterator i=x.definitions->begin(); i!=x.definitions->end(); i++)
        definitions->push_back(RbPtr<SyntaxElement>( (*i)->clone() ));
 
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


/** Get class vector describing type of object */
const VectorString& SyntaxClassDef::getClass(void) const { 
    
    static VectorString rbClass = VectorString(SyntaxClassDef_name) + SyntaxElement::getClass();
	return rbClass; 
}


/** Get semantic value: insert a user-defined class in the user workspace */
RbPtr<Variable> SyntaxClassDef::evaluateContent(const RbPtr<Environment>& env) {

    std::cerr << "Sorry, user-defined classes are not implemented yet" << std::endl;

    // No return value 
    return RbPtr<Variable>::getNullPtr();
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& SyntaxClassDef::getTypeSpec(void) const {
    return typeSpec;
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


