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

#include "Integer.h"
#include "LookupNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "VariableSlot.h"
#include "VectorInteger.h"
#include "VectorString.h"
#include "SyntaxVariable.h"


/** Construct from identifier and index */
SyntaxVariable::SyntaxVariable(RbString* id, std::list<SyntaxElement*>* indx) :
    SyntaxElement(), identifier(id), index(indx), baseVariable(NULL) {
}


/** Construct from base variable (composite node), identifier and index */
SyntaxVariable::SyntaxVariable(SyntaxVariable* var, RbString* id, std::list<SyntaxElement*>* indx) :
    SyntaxElement(), identifier(id), index(indx), baseVariable(var) {
}


/** Deep copy constructor */
SyntaxVariable::SyntaxVariable(const SyntaxVariable& x)
    : SyntaxElement(x) {

    identifier      = new RbString(*x.identifier);
    baseVariable    = new SyntaxVariable(*x.baseVariable);
    for (std::list<SyntaxElement*>::iterator i=x.index->begin(); i!=x.index->end(); i++) {
        index->push_back((*i)->clone());
    }
}


/** Destructor deletes variable, identifier and index */
SyntaxVariable::~SyntaxVariable() {
    
    delete identifier;
    delete baseVariable;
    for (std::list<SyntaxElement*>::iterator i=(*index).begin(); i!=(*index).end(); i++)
        delete (*i);
    delete index;
}


/** Assignment operator */
SyntaxVariable& SyntaxVariable::operator=(const SyntaxVariable& x) {

    if (&x != this) {
    
        delete identifier;
        delete baseVariable;
        for (std::list<SyntaxElement*>::iterator i=index->begin(); i!=index->end(); i++)
            delete (*i);
        delete index;

        SyntaxElement::operator=(x);

        identifier      = new RbString(*x.identifier);
        baseVariable    = new SyntaxVariable(*x.baseVariable);
        for (std::list<SyntaxElement*>::iterator i=x.index->begin(); i!=x.index->end(); i++) {
            index->push_back((*i)->clone());
        }
    }

    return (*this);
}


/** Return brief info about object */
std::string SyntaxVariable::briefInfo () const {

    std::ostringstream   o;

    if (baseVariable == NULL)
        o << "SyntaxVariable: " << std::string(*identifier);
    else
        o << "SyntaxVariable: <base>." << std::string(*identifier);

    for (std::list<SyntaxElement*>::iterator i=index->begin(); i!=index->end(); i++)
        o << "[<" << (*i) << ">]";

    return o.str();
}


/** Clone syntax element */
SyntaxVariable* SyntaxVariable::clone () const {

    return new SyntaxVariable(*this);
}


/** Get class vector describing type of object */
const VectorString& SyntaxVariable::getClass(void) const { 

    static VectorString rbClass = VectorString(SyntaxVariable_name) + SyntaxElement::getClass();
	return rbClass; 
}


/** Convert element to DAG node expression */
DAGNode* SyntaxVariable::getDAGNodeExpr(Frame* frame) const {

    /* Package index arguments */
    std::vector<Argument> indexArgs;
    for (std::list<SyntaxElement*>::iterator i=index->begin(); i!=index->end(); i++)
        indexArgs.push_back(Argument("", (*i)->getDAGNodeExpr(frame)));

    /* Return lookup node */
    if (baseVariable == NULL)
        return new LookupNode(frame->getVariableSlot(*identifier).getReference(), indexArgs);
    else
        return new LookupNode(baseVariable->getDAGNodeExpr(frame), identifier, indexArgs);
}


/** Return nice representation of the syntax element */
std::string SyntaxVariable::getFullName(Frame* frame) const {

    std::ostringstream theName;
    if (baseVariable != NULL)
        theName << baseVariable->getFullName(frame) << ".";

    theName << std::string(*identifier);

    VectorInteger theIndex = getIndex(frame);
    for (size_t i=0; i<theIndex.size(); i++)
        theName << "[" << theIndex[i] << "]";

    return theName.str();
}


/** Get index */
VectorInteger SyntaxVariable::getIndex(Frame* frame) const {

    DAGNode*        theVariable = getVariableReference(frame);
    VectorInteger   theIndex;

    int count = 1;
    for (std::list<SyntaxElement*>::iterator i=index->begin(); i!=index->end(); i++, count++) {

        if ((*i) == NULL)
            theIndex.push_back(-1);
        else {

            DAGNode* indexVar = (*i)->getValue(frame);
            if (indexVar->isType(Integer_name)) {

                // Calculate (or get) an integer index
                int intIndex = ((Integer*)(indexVar))->getValue(); 
                if (intIndex < 1) {
                    std::ostringstream msg;
                    msg << "Index " << count << " for ";
                    if (baseVariable != NULL)
                        msg << baseVariable->getFullName(frame) << ".";
                    msg << *identifier;
                    msg << " smaller than 1";
                    throw RbException(msg);
                }

                // Get zero-based value corresponding to integer index
                theIndex.push_back(intIndex-1);
            }
            else if (indexVar->isType(RbString_name)) {

                // Use variable to convert string index to integer index
                int intIndex = theVariable->getElementIndex((RbString*)(indexVar));

                // If success, then we have a zero-based integer index
                theIndex.push_back(intIndex);
            }
            else {
                delete indexVar;
                std::ostringstream msg;
                msg << "Index " << count << " for ";
                if (baseVariable != NULL)
                    msg << baseVariable->getFullName(frame) << ".";
                msg << *identifier;
                msg << " of wrong type (neither " << Integer_name << " nor " << RbString_name << ")";
                throw RbException(msg);
            }

            // Avoid memory leak
            delete indexVar;
        }
    }

    // Return index
    return theIndex;
}


/**
 * @brief Get semantic value
 *
 * The variable can either be a member or a base variable. In the latter
 * case, its "baseVariable" member is NULL. If the element is a base variable,
 * we get the semantic value of the element by looking it up in the frame.
 * If it is a member variable name, we try to find it in the member variable
 * frame of a composite variable found by another SyntaxVariable element.
 *
 */
DAGNode* SyntaxVariable::getValue(Frame* frame) const {

    /* Get subscript */
    VectorInteger theIndex = getIndex(frame);

    /* Get base frame of variable */
    Frame* varFrame = getBaseFrame(frame);

    /* Get value */
    if (theIndex.size() == 0)
        return varFrame->getVariableSlot(*identifier).getValue()->clone();
    else
        return varFrame->getVariableSlot(*identifier).getElement(theIndex)->clone();
}


/** Return base frame of variable */
Frame* SyntaxVariable::getBaseFrame(Frame *frame) const {

    if (baseVariable == NULL)
        return frame;
    else
        return baseVariable->getVariableMemberFrame(frame);
}


/**
 * Get pointer to member frame of variable. This evaluates the index and is appropriate
 * only if appearing in an lhs or value expression, not in an equation expression, in
 * which case the base variable needs to be represented by a lookup node, obtained
 * through a call to getDAGNodeExpr(frame).
 *
 * @Note This function gets the member variable frame of the variable or variable
 * element itself, so it should only be called on base variable syntax elements.
 * Use getBaseFrame if you wish to get the base frame of the variable.
 */
Frame* SyntaxVariable::getVariableMemberFrame(Frame* frame) const {

    /* Get the base frame */
    Frame* baseFrame;
    if (baseVariable != NULL)
        baseFrame = baseVariable->getVariableMemberFrame(frame);
    else
        baseFrame = frame;

    // Get index
    VectorInteger intIndex = getIndex(frame);

    // Get reference to the variable
    DAGNode* theVariable;
    if (index->size() == 0)
        theVariable = baseFrame->getVariableSlot(*identifier).getReference();
    else
        theVariable = baseFrame->getVariableSlot(*identifier).getReference(intIndex);

    // Test that it is a composite node
    if (!theVariable->isType(CompositeNode_name))
        throw RbException(getFullName(frame) + " does not have members");

    // Return the frame
    return ((CompositeNode*)(theVariable))->getMemberFrame();
 }


/** Get variable reference */
DAGNode* SyntaxVariable::getVariableReference(Frame* frame) const {

    /* Get the frame of the variable */
    Frame* varFrame;
    if (baseVariable == NULL)
        varFrame = frame;
    else
        varFrame = baseVariable->getVariableMemberFrame(frame);
    
    /* Return the variable reference */
    return varFrame->getVariableSlot(*identifier).getReference();
}


/** Print info about the syntax element */
void SyntaxVariable::print(std::ostream& o) const {

    o << "<" << this << "> SyntaxVariable:" << std::endl;
    o << "identifier = " << *identifier << std::endl;
    if (baseVariable != NULL)
        o << "base variable   = <" << baseVariable << "> " << baseVariable->briefInfo() << std::endl;
    int count = 1;
    for (std::list<SyntaxElement*>::iterator i=(*index).begin(); i!=(*index).end(); i++, count++) {
        o << "index " << count << " = <" << (*i) << "> " << (*i)->briefInfo();
    }
    o << std::endl;

    for (std::list<SyntaxElement*>::iterator i=index->begin(); i!=index->end(); i++)
        (*i)->print(o);
}

