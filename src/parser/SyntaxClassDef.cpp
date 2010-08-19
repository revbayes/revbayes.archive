/**
 * @file
 * This file contains the implementation of SyntaxClassDef, which is
 * used to hold the definitions of user-defined types.
 *
 * @brief Implementation of SyntaxClassDef
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#include "DAGNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "VectorString.h"
#include "SyntaxClassDef.h"
#include "UserFunction.h"
#include "Workspace.h"

#include <sstream>


/** Construct definition from class name, base class name, and variable and function definitions */
SyntaxClassDef::SyntaxClassDef( RbString*                   name,
                                RbString*                   base,
                                std::list<SyntaxElement*>*  defs)
    : SyntaxElement(), className(name), baseClass(base), definitions(defs) {
}


/** Deep copy constructor */
SyntaxClassDef::SyntaxClassDef(const SyntaxClassDef& x)
    : SyntaxElement(x) {

    className   = new RbString(*className);
    baseClass   = new RbString(*baseClass);
 
    for (std::list<SyntaxElement*>::const_iterator i=x.definitions->begin(); i!=x.definitions->end(); i++)
        definitions->push_back((*i)->clone());
}


/** Destructor deletes members */
SyntaxClassDef::~SyntaxClassDef() {
    
    delete className;
    delete baseClass;

    for (std::list<SyntaxElement*>::iterator i=definitions->begin(); i!=definitions->end(); i++)
        delete (*i);
    delete definitions;
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


/** Equals comparison */
bool SyntaxClassDef::equals(const SyntaxElement* elem) const {

	const SyntaxClassDef* p = dynamic_cast<const SyntaxClassDef*>(elem);
    if (p == NULL)
        return false;

    if (definitions->size() != p->definitions->size())
        return false;

    bool result = true;
    result = result && className->equals(p->className);
    result = result && baseClass->equals(p->baseClass);

    std::list<SyntaxElement*>::iterator i, j;
    for (i=definitions->begin(), j=p->definitions->begin(); i!=definitions->end(); i++, j++)
        result = result && (*i)->equals(*j);
    
    return result;
}


/** Convert element to DAG node; return NULL since it is not applicable */
DAGNode* SyntaxClassDef::getDAGNode(Frame* frame) const {

    return NULL;
}


/** Get semantic value: insert a user-defined class in the user workspace */
RbObject* SyntaxClassDef::getValue(Frame* frame) const {

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


