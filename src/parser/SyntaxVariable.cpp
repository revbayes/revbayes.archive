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
#include "DeterministicNode.h"
#include "RbInt.h"
#include "RbList.h"
#include "RbNames.h"
#include "RbException.h"
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

    // The following loop works because SyntaxElemen is the base class
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


/** Convert element to DAG node */
DAGNode* SyntaxVariable::getDAGNode(Frame* frame) const {

    std::vector<Argument> args;
    args.push_back(Argument("variable", variable->getDAGNode()));
    args.push_back(Argument("id", new ConstantNode(identifier)));

    RbFunction *varFunc = Workspace::globalWorkspace().getFunction(".lookup", args);
    DeterministicNode* root = new DeterministicNode((RbFunction*)(varFunc->clone()), args);

    for (std::list<SyntaxElement*>::iterator i=(*index).begin(); i!=(*index).end(); i++) {
        args.clear();
        args.push_back(Argument("object", root));
        args.push_back(Argument("index", (*i)->getDAGNode(frame)));

        RbFunction* elemFunc = Workspace::globalWorkspace().getFunction(".element", args);
        root = new DeterministicNode((RbFunction*)(elemFunc->clone()), args);
    }

    return root;
}


/** Get identifier */
const RbString* SyntaxVariable::getIdentifier() const {

    return identifier;
}


/**
 * @brief Get semantic value
 *
 * The variable can either be a member or a base variable. In the latter
 * case, its "variable" member is NULL. If the element is a base variable,
 * we get the semantic value of the element by looking it up in the frameiron-
 * ment. If it is a base variable name, we try to find it as a member of the
 * "variable" found by another SyntaxVariable element.
 *
 */
RbObject* SyntaxVariable::getValue(Frame* frame) {

    // Value pointer
    const RbObject* value = NULL;

    // Get variable; call getValue to pass through DAGNode
    if (variable == NULL) {
        value = frame->getVariable(*identifier);
        if (value->isType(RbNames::DAGNode::name))
            value = ((DAGNode*)(value))->getValue();
    }
    else {
        // Get object of which we are a member
        RbObject*   obj = variable->getValue(frame);

        // Get member
        RbComplex*  complexObj = dynamic_cast<RbComplex*>(obj);
        if (complexObj == NULL) {
            throw (RbException("Variable " + std::string(*(variable->getIdentifier())) +
                                " does not have members"));
        }
    
        // Get value; call getValue to pass through DAGNode
        value = complexObj->getMember(*identifier);
        if (value->isType(RbNames::DAGNode::name))
            value = ((DAGNode*)(value))->getValue();
        if (value == NULL) {
            throw (RbException("Variable " + std::string(*(variable->getIdentifier())) +
                                " does not have a member called " + std::string(*identifier)));
        }
    }

    // Get element if we have element index/indices (list of lists model)
    for (std::list<SyntaxElement*>::iterator i=(*index).begin(); i!=(*index).end(); i++) {

        // Check that it is a list object with elements
        const RbList* listObj = dynamic_cast<const RbList*>(value);
        if (listObj == NULL) {
            throw (RbException(*identifier + RbString(" does not have elements ")));
        }
            
        // Get index
        RbObject* indexObj = (*i)->getValue(frame);
        if (indexObj == NULL) {
            throw (RbException("Erroneous index expression of variable " + std::string(*identifier)));
        }
        RbInt*  intIndex = dynamic_cast<RbInt*>(indexObj);
        if (indexObj == NULL) {
            throw (RbException("Index of variable " + std::string(*identifier) + " does not evaluat to an int"));
        }

        // Get value corresponding to index; call getValue to pass through DAGNode
        value = listObj->getElement(*intIndex);
        if (value->isType(RbNames::DAGNode::name))
            value = ((DAGNode*)(value))->getValue();
    }

    // Return value (we pass on management responsibility to caller)
    return value->clone();
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


