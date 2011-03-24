/**
 * @file
 * This file contains the implementation of SyntaxVariable, which is
 * used to hold variable references in the syntax tree.
 *
 * @brief Implementation of SyntaxVariable
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
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
#include "MemberNode.h"
#include "MemberObject.h"
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
    std::vector<DAGNode*> indexArgs;
    for (std::list<SyntaxElement*>::iterator i=index->begin(); i!=index->end(); i++)
        indexArgs.push_back((*i)->getDAGNodeExpr(frame));

    /* Check whether it is safe to return the variable itself or whether we need a lookup node */
    bool indexLookupNeeded = false;
    for (std::vector<DAGNode*>::iterator i=indexArgs.begin(); i!=indexArgs.end(); i++) {
        if ( !(*i)->isConstExpr() ) {
            indexLookupNeeded = true;
            break;
        }
    }

    DAGNode* baseVarNode = NULL;
    if ( baseVariable != NULL )
        baseVarNode = baseVariable->getDAGNodeExpr( frame );

    if ( indexLookupNeeded == false && ( baseVarNode == NULL || baseVarNode->isDAGType( MemberNode_name ) ) ) {
        
        /* Avoid memory leak by deleting the index arguments */
        for (std::vector<DAGNode*>::iterator i=indexArgs.begin(); i!=indexArgs.end(); i++) {
            if ( (*i)->numRefs() == 0 )
                delete (*i);
        }
        if ( baseVarNode != NULL )
            delete baseVarNode;

        /* Return value, which is a direct reference to the variable */
        return getValue( frame );
    }
    else {
        if ( baseVarNode == NULL )
            return new LookupNode(frame->getReference(*identifier), indexArgs);
        else if ( !baseVarNode->isDAGType( LookupNode_name ) ) // In principle, this indirection is not needed but current code relies on it
            return new LookupNode( new LookupNode( baseVarNode, std::vector<DAGNode*>() ), identifier, indexArgs );
        else
            return new LookupNode( (LookupNode *)(baseVarNode), identifier, indexArgs);
    }
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

    VectorInteger   theIndex;

    int count = 1;
    for (std::list<SyntaxElement*>::iterator i=index->begin(); i!=index->end(); i++, count++) {

        if ((*i) == NULL)
            theIndex.push_back(-1);
        else {

            DAGNode* indexVar = (*i)->getValue(frame);
            if (indexVar->getValue()->isType(Integer_name)) {

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
            else if (indexVar->getValue()->isType(RbString_name)) {

                throw RbException( "String index not implemented yet" );
                
                // Use variable to convert string index to integer index
                // int intIndex = theVariable->getElementIndex((RbString*)(indexVar));

                // If success, then we have a zero-based integer index
                // theIndex.push_back(intIndex);
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

    /* Get base variable */
    DAGNode* baseVar;
    if ( baseVariable == NULL )
        baseVar = frame->getVariableSlot(*identifier).getReference();
    else {
        DAGNode* theMemberNode = baseVariable->getValue(frame);

        // Test that it is a member object
        if ( !theMemberNode->isDAGType( MemberNode_name ) )
            throw RbException( getFullName(frame) + " does not have members" );

        // Find the member variable
        const MemberObject* theMemberObject = const_cast<const MemberObject*>( ((MemberNode*)(theMemberNode))->getMemberObject() );
        baseVar = const_cast<DAGNode*>( theMemberObject->getVariable(*identifier) );
    }

    /* Get value */
    if (theIndex.size() == 0)
        return baseVar;
    else
        return const_cast<DAGNode*>( baseVar->getVarElement( theIndex ) );
}


/** Get variable slot */
VariableSlot& SyntaxVariable::getVariableSlot(Frame* frame) const {

    const Frame& baseFrame;
    if (baseVariable == NULL)
        baseFrame = *frame;
    else {
        DAGNode*        theVar   = baseVariable->getValue(frame);
        const RbObject* theValue = theVar->getValue();
        if (theValue == NULL)
            throw RbException( "Variable expression evaluates to NULL" );
        const MemberObject* theMemberObject = dynamic_cast<const MemberObject*>(theValue);
        if (theMemberObject == NULL)
            throw RbException( "Variable '" + baseVariable->getFullName(frame) + "' does not have members" );
        baseFrame = theMemberObject->getMembers();
    }

    if (!baseFrame.existsVariable(*identifier))
        return NULL;
    else
        return baseFrame.getVariableSlot(*identifier);
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

