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
#include <sstream>

#include "ConstantNode.h"
#include "DAGNode.h"
#include "DAGNodeContainer.h"
#include "DeterministicNode.h"
#include "IntVector.h"
#include "RbInt.h"
#include "RbNames.h"
#include "RbException.h"
#include "StringVector.h"
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
const StringVector& SyntaxVariable::getClass(void) const { 

    static StringVector rbClass = StringVector(SyntaxVariable_name) + SyntaxElement::getClass();
	return rbClass; 
}


/** Convert element to DAG node */
DAGNode* SyntaxVariable::getDAGNode(Frame* frame) const {

    std::vector<Argument> args;
    if (variable == NULL)
        args.push_back(Argument("variable", NULL));
    else
        args.push_back(Argument("variable", variable->getDAGNode()));

    args.push_back(Argument("id", new ConstantNode(identifier)));

    if (index->size() > 0) {
        DAGNodeContainer* indexArgs = new DAGNodeContainer(index->size(), (DAGNode*)(NULL));
        std::list<SyntaxElement*>::iterator j = index->begin();
        for (ContainerIterator i=indexArgs->begin(); i!=indexArgs->end(); i++, j++) {
            (*indexArgs)[i] = (*j)->getDAGNode(frame);
        }
        args.push_back(Argument("index", indexArgs));
    }
    else
        args.push_back(Argument("index", (DAGNode*)(NULL)));

    RbFunction *varFunc     = Workspace::globalWorkspace().getFunction(".lookup", args);
    DeterministicNode* root = new DeterministicNode(varFunc);

    return root;
}


/** Get identifier */
const RbString* SyntaxVariable::getIdentifier() const {

    return identifier;
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
    if (variable == NULL) {
        if (theIndex.size() == 0)
            return frame->getValue(*identifier)->clone();
        else
            return frame->getValElement(*identifier, theIndex)->clone();
    }
    else {
        RbComplex* complexObj = (RbComplex*)(variable->getValue());
        if (complexObj == NULL) {
            throw RbException("Variable " + variable->getFullName(frame) + " does not have members");
        }
        const DAGNode* theVar = complexObj->getVariable(*identifier);
        if (theIndex.size() == 0)
            return theVar->getValue()->clone();
        else
            return theVar->getValElement(theIndex)->clone();
   }
}


/** Print info about the syntax element */
void SyntaxVariable::print(std::ostream& o) const {

    o << "SyntaxVariable:" << std::endl;
    o << "identifier = " << identifier->briefInfo() << std::endl;
    o << "variable   = " << variable->briefInfo() << std::endl;
    o << "index      = [";
    for (std::list<SyntaxElement*>::iterator i=(*index).begin(); i!=(*index).end(); i++) {
        if (i != (*index).begin())
            o << ", ";
        (*i)->getValue()->printValue(o);
    }
    o << "]" << std::endl;
}


