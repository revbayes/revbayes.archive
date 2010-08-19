/**
 * @file
 * This file contains the implementation of SyntaxVariable, which is
 * used to hold variable references in the syntax tree.
 *
 * @brief Implementation of SyntaxVariable
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#include <iostream>
#include <list>
#include <string>
#include <sstream>

#include "Argument.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "DAGNodeContainer.h"
#include "DeterministicNode.h"
#include "Func__lookup.h"
#include "IntVector.h"
#include "MemberObject.h"
#include "RbInt.h"
#include "RbNames.h"
#include "RbException.h"
#include "VectorString.h"
#include "SyntaxVariable.h"
#include "Workspace.h"


/** Construct from identifier and index */
SyntaxVariable::SyntaxVariable(RbString* id, std::list<SyntaxElement*>* indx) :
    SyntaxElement(), identifier(id), index(indx), variable(NULL) {
}


/** Construct from wrapping variable, identifier and index */
SyntaxVariable::SyntaxVariable(SyntaxVariable* var, RbString* id, std::list<SyntaxElement*>* indx) :
    SyntaxElement(), identifier(id), index(indx), variable(var) {
}


/** Deep copy constructor */
SyntaxVariable::SyntaxVariable(const SyntaxVariable& sv)
    : SyntaxElement(sv) {

    identifier = new RbString(*sv.identifier);
    variable   = new SyntaxVariable(*sv.variable);

    // The following loop works because SyntaxElement is the base class
    for (std::list<SyntaxElement*>::iterator i=(*sv.index).begin(); i!=(*sv.index).end(); i++) {
        index->push_back((*i)->clone());
    }
}


/** Destructor deletes variable, identifier and index */
SyntaxVariable::~SyntaxVariable() {
    
    delete identifier;
    delete variable;
    for (std::list<SyntaxElement*>::iterator i=(*index).begin(); i!=(*index).end(); i++)
        delete (*i);
    delete index;
}


/** Return brief info about object */
std::string SyntaxVariable::briefInfo () const {

    std::ostringstream   o;

    if (variable == NULL)
        o << "SyntaxVariable; id = " << std::string(*identifier) <<  " -- index = ";
    else
        o << "SyntaxVariable; <member> id = " << std::string(*identifier) <<  " -- index = ";

    o << "[";
    for (std::list<SyntaxElement*>::iterator i=(*index).begin(); i!=(*index).end(); i++) {
        if (i != (*index).begin())
            o << ", ";
        (*i)->getValue()->printValue(o);
    }
    o << "]";

    return o.str();
}


/** Clone syntax element */
SyntaxElement* SyntaxVariable::clone () const {

    return (SyntaxElement*)(new SyntaxVariable(*this));
}


/** Equals comparison */
bool SyntaxVariable::equals(const SyntaxElement* elem) const {

	const SyntaxVariable* p = dynamic_cast<const SyntaxVariable*>(elem);

    if (p == NULL)
        return false;

    bool result = true;
    result = result && variable->equals(p->variable);
    result = result && identifier->equals(p->identifier);

    if (index->size() != p->index->size())
        return false;
    std::list<SyntaxElement*>::iterator i, j;
    for (i=index->begin(), j=p->index->begin(); i!=index->end(); i++, j++) {
        result = result && (*i)->equals(*j);
    }

    return result;
}


/** Get class vector describing type of object */
const VectorString& SyntaxVariable::getClass(void) const { 

    static VectorString rbClass = VectorString(SyntaxVariable_name) + SyntaxElement::getClass();
	return rbClass; 
}


/** Convert element to DAG node */
DAGNode* SyntaxVariable::getDAGNode(Frame* frame) const {

    if (variable == NULL) {
        Func__lookup* lookup = new Func__lookup(frame->getType(*identifier), frame->getDim(*identifier));
        DAGNode* theVar = frame->getVariable(*identifier);
        std::vector<Argument> args;
        args.push_back(Argument("", theVar));
        for (std::list<SyntaxElement*>::iterator i=index->begin(); i!=index->end(); i++) {
            args.push_back(Argument("", (*i)->getDAGNode(frame)));
        }
        lookup->processArguments(args);
        return new DeterministicNode(lookup);
    }
    else {
        // TODO: do this properly
        return NULL;
    }
}


/** Return nice representation of the syntax element */
std::string SyntaxVariable::getFullName(Frame* frame) const {

    std::ostringstream theName;
    if (variable != NULL)
        theName << variable->getFullName(frame) << ".";

    theName << std::string(*identifier);

    IntVector theIndex = getIndex(frame);
    for (size_t i=0; i<theIndex.size(); i++)
        theName << "[" << theIndex[i] << "]";

    return theName.str();
}


/** Get identifier */
const RbString* SyntaxVariable::getIdentifier() const {

    return identifier;
}


/** Get index */
IntVector SyntaxVariable::getIndex(Frame* frame) const {

    IntVector theIndex;

    for (std::list<SyntaxElement*>::iterator i=(*index).begin(); i!=(*index).end(); i++) {

        // Get index
        RbObject* indexObj = (*i)->getValue(frame);
        if (indexObj == NULL) {
            throw (RbException("Erroneous index expression for " + std::string(*identifier)));
        }
        RbInt*  intIndex = dynamic_cast<RbInt*>(indexObj);
        if (indexObj == NULL) {
            throw (RbException("Index expression for " + std::string(*identifier) + " does not evaluat to int"));
        }
        if (*intIndex < 1) {
            throw (RbException("Index expression for " + std::string(*identifier) + " smaller than 1"));
        }

        // Get zero-based value corresponding to index
        theIndex.push_back((*intIndex)-1);

        // Delete the index object
        delete indexObj;
    }

    // Return index
    return theIndex;
}


/**
 * @brief Get semantic value
 *
 * The variable can either be a member or a base variable. In the latter
 * case, its "variable" member is NULL. If the element is a base variable,
 * we get the semantic value of the element by looking it up in the frame.
 * If it is a member variable name, we try to find it as a member of the
 * complex language object found by another SyntaxVariable element.
 *
 */
RbObject* SyntaxVariable::getValue(Frame* frame) const {

    /* Get subscript */
    IntVector theIndex = getIndex(frame);

    /* Get value */
    if (variable == NULL) 
		{
        if (theIndex.size() == 0)
            return frame->getValue(*identifier)->clone();
        else
            return frame->getValElement(*identifier, theIndex)->clone();
		}

    const MemberObject* object = dynamic_cast<const MemberObject*>(variable->getValuePtr(frame));
    if (object == NULL)
        throw RbException("Object " + variable->getFullName(frame) + " does not have members");

    if (theIndex.size() == 0)
        return object->getValue(*identifier)->clone();

    const DAGNodeContainer* container = dynamic_cast<const DAGNodeContainer*>(object->getVariable(*identifier));
    if (container == NULL)
        throw RbException("Object " + variable->getFullName(frame) + "." + std::string(*identifier) + " does not have elements");

    return container->getValElement(theIndex)->clone();
}


/** Get pointer to variable value */
const RbObject* SyntaxVariable::getValuePtr(Frame* frame) const {

    /* Get subscript */
    IntVector theIndex = getIndex(frame);

    /* Get value */
    if (variable == NULL) {
        if (theIndex.size() == 0)
            return frame->getValue(*identifier);
        else
            return frame->getValElement(*identifier, theIndex);
    }

    const MemberObject* object = dynamic_cast<const MemberObject*>(variable->getValuePtr(frame));
    if (object == NULL)
        throw RbException("Object " + variable->getFullName(frame) + " does not have members");

    if (theIndex.size() == 0)
        return object->getValue(*identifier);

    const DAGNodeContainer* container = dynamic_cast<const DAGNodeContainer*>(object->getVariable(*identifier));
    if (container == NULL)
        throw RbException("Object " + variable->getFullName(frame) + "." + std::string(*identifier) + " does not have elements");

    return container->getValElement(theIndex);
}


/** Print info about the syntax element */
void SyntaxVariable::print(std::ostream& o) const {

    o << "SyntaxVariable:" << std::endl;
    o << "identifier = " << identifier->briefInfo() << std::endl;
    if (variable == NULL)
        o << "variable   = NULL" << std::endl;
    else
        o << "variable   = " << variable->briefInfo() << std::endl;
    o << "index      = [";
    for (std::list<SyntaxElement*>::iterator i=(*index).begin(); i!=(*index).end(); i++) {
        if (i != (*index).begin())
            o << ", ";
        (*i)->getValue()->printValue(o);
    }
    o << "]" << std::endl;
}


