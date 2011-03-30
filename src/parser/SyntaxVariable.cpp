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
#include "SyntaxFunctionCall.h"
#include "VariableSlot.h"
#include "VectorInteger.h"
#include "VectorString.h"
#include "SyntaxVariable.h"


/** Construct from identifier and index */
SyntaxVariable::SyntaxVariable(RbString* id, std::list<SyntaxElement*>* indx) :
    SyntaxElement(), identifier(id), index(indx), functionCall(NULL), baseVariable(NULL) {
}


/** Construct from function call and index */
SyntaxVariable::SyntaxVariable(SyntaxFunctionCall* fxnCall, std::list<SyntaxElement*>* indx) :
    SyntaxElement(), identifier(NULL), index(indx), functionCall(fxnCall), baseVariable(NULL) {
}


/** Construct from base variable (member node), identifier and index */
SyntaxVariable::SyntaxVariable(SyntaxVariable* var, RbString* id, std::list<SyntaxElement*>* indx) :
    SyntaxElement(), identifier(id), index(indx), functionCall(NULL), baseVariable(var) {
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


/**
 * Convert element to DAG node expression. This function differs from
 * getValue primarily in that it uses a different method to get the
 * variable from the variable slot, and in that it produces a lookup
 * node if the index expression is not a constant expression.
 *
 * The variable extraction is the opposite to what you might expect
 * from a call to getValue. If the slot is a reference to a temp
 * variable, you get a copy of that variable (a temp). If the slot is
 * a value slot, you get the variable itself rather than a copy of it.
 * A regular reference slot will return the referenced variable, which
 * can be the result of a lookup or a function call. See VariableSlot
 * for more detail; the functinality is in its getVariableRef function.
 */
DAGNode* SyntaxVariable::getDAGNodeExpr( Frame* frame ) const {

    /* Package index arguments */
    std::vector<DAGNode*> indexArgs;
    for ( std::list<SyntaxElement*>::iterator i = index->begin(); i != index->end(); i++ )
        indexArgs.push_back( (*i)->getDAGNodeExpr(frame) );

    /* Check whether it is safe to return the variable itself or whether we need a lookup node */
    bool lookupNeeded = false;
    for ( std::vector<DAGNode*>::iterator i = indexArgs.begin(); i != indexArgs.end(); i++ ) {
        if ( !(*i)->isConstExpr() ) {
            lookupNeeded = true;
            break;
        }
    }

    /* Get variable */
    DAGNode* theVar;
    if ( baseVariable == NULL ) {
        
        if ( functionCall == NULL )
            theVar = frame->getVariableSlot( *identifier ).getParserVariableRef();
        else
            theVar = functionCall->getValue( frame );
    }
    else {

        // The call to getValue of baseVariable either returns
        // a value or results in the throwing of an exception
        DAGNode* baseVar = baseVariable->getValue( frame );
        if ( !baseVar->isDAGType( MemberNode_name ) )
            throw RbException( "Variable " + baseVariable->getFullName( frame ) + " does not have members" );       
    
        if ( identifier == NULL )
            throw RbException( "Member variable identifier missing" );

        MemberObject* theMemberObject = static_cast<MemberNode*>( baseVar )->getMemberObject();
        Frame& members = const_cast<Frame&>( theMemberObject->getMembers() );
        theVar = members.getVariableSlot( *identifier ).getParserVariableRef();
    }

    /* Find return value; easy if not an element */
    if ( indexArgs.size() == 0 )
        return theVar;

    /* We need to either find the element or return a lookup node */
    if ( !lookupNeeded ) {

        for ( std::vector<DAGNode*>::iterator i = indexArgs.begin(); i != indexArgs.end(); i++ ) {
            if ( (*i)->numRefs() == 0 )
                delete (*i);
        }
        VectorInteger elemIndex = getIndex( frame );

        /* We need to find the element. First ask if it exists to avoid a memory leak. */
        if ( !theVar->existsElement( elemIndex ) )
            throw RbException( getFullName( frame ) + " does not exist" );

        DAGNode* elem = theVar->getElement( elemIndex );
        if ( theVar->numRefs() == 0 )
            delete theVar;
        return elem;
    }
    else {
    
        return new LookupNode( theVar, indexArgs );
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

                if ( indexVar->numRefs() == 0 )
                    delete indexVar;
                
                throw RbException( "String index not implemented yet" );
                
                // Use variable to convert string index to integer index
                // int intIndex = theVariable->getElementIndex((RbString*)(indexVar));

                // If success, then we have a zero-based integer index
                // theIndex.push_back(intIndex);
            }
            else {
                if ( indexVar->numRefs() == 0 )
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
            if ( indexVar->numRefs() == 0 )
                delete indexVar;
        }
    }

    // Return index
    return theIndex;
}


/**
 * @brief Get semantic value (l-value)
 *
 * This function is similar to getValue(), but it is used
 * when the variable expression is on the lhs of an assignment
 * expression. An important difference is in the retrieval of
 * element variables, where we do not go into the last container.
 * The reason is that we may want to do container or subcontainer
 * assignment, and we may want give the container a chance to mutate
 * from a value to a variable container (or in the other direction).
 * During the retrieval of an element, the elemIndex is updated to
 * reflect the appropriate index into the returned variable.
 *
 * Another difference is that we do not throw an error if the
 * variable does not exist in the frame; instead, we return a NULL
 * pointer and set theSlot pointer to NULL as well. If the variable
 * exists in a workspace or a member variable frame, and we retrieve
 * it through an identifier, theSlot pointer is set to the slot from
 * which we retrieve the variable. This is important in determining
 * whether the lhs of an assignment expression is valid for a reference
 * assignment. If we retrieve the variable through a function call,
 * it can never be valid for a reference assignment. We also set the
 * slot to NULL in these cases.
 */
DAGNode* SyntaxVariable::getLValue(Frame* frame, VariableSlot*& theSlot, VectorInteger& elemIndex) const {

    /* Get index */
    elemIndex = getIndex(frame);

    /* Get variable */
    DAGNode* theVar;
    if ( baseVariable == NULL ) {
        
        if ( functionCall == NULL ) {

            if ( !frame->existsVariable( *identifier ) ) {
                
                theSlot = NULL;
                theVar  = NULL;
            }
            else {
                
                theSlot = &frame->getVariableSlot( *identifier );
                theVar  = theSlot->getParserVariable();
            }
        }
        else {

            theSlot = NULL;
            theVar  = functionCall->getValue( frame );
        }
    }
    else {

        // The call to getValue of baseVariable either returns
        // a value or results in the throwing of an exception
        DAGNode* baseVar = baseVariable->getValue( frame );
        if ( !baseVar->isDAGType( MemberNode_name ) )
            throw RbException( "Variable " + baseVariable->getFullName( frame ) + " does not have members" );       
    
        if ( identifier == NULL )
            throw RbException( "Member variable identifier missing" );

        MemberObject* theMemberObject = static_cast<MemberNode*>( baseVar )->getMemberObject();
        if ( !theMemberObject->getMembers().existsVariable( *identifier ) )
            throw RbException( "Variable " + baseVariable->getFullName( frame ) + " does not have a member called '" + *identifier + "'" );       

        Frame& members = const_cast<Frame&>( theMemberObject->getMembers() );
        theSlot = &members.getVariableSlot( *identifier );
        theVar  = theSlot->getParserVariable();
    }

    /* Find return value; easy if not an element or if a new variable */
    if ( elemIndex.size() == 0 || theVar == NULL )
        return theVar;

    /* We need to find the element. First ask if it exists to avoid a memory leak. */
    if ( !theVar->existsElement( elemIndex ) )
        throw RbException( getFullName( frame ) + " does not exist" );

    DAGNode* elem = theVar->getElement( elemIndex );
    if ( theVar->numRefs() == 0 )
        delete theVar;
    return elem;
}


/**
 * @brief Get semantic value (r-value)
 *
 * The variable can either be a member or a base variable. In the latter
 * case, its "baseVariable" member is NULL. If the element is a base variable,
 * we get the semantic value of the element by looking it up in the frame.
 * If it is a member variable, we try to find it in the member variable
 * frame of a composite variable found by another SyntaxVariable element.
 *
 * The function call is NULL unless we have a base variable, in which case
 * the function call can replace the identifier.
 */
DAGNode* SyntaxVariable::getValue(Frame* frame) const {

    /* Get index */
    VectorInteger elemIndex = getIndex(frame);

    /* Get variable */
    DAGNode* theVar;
    if ( baseVariable == NULL ) {
        
        if ( functionCall == NULL )
            theVar = frame->getVariableSlot( *identifier ).getParserVariable();
        else
            theVar = functionCall->getValue( frame );
    }
    else {

        // The call to getValue of baseVariable either returns
        // a value or results in the throwing of an exception
        DAGNode* baseVar = baseVariable->getValue( frame );
        if ( !baseVar->isDAGType( MemberNode_name ) )
            throw RbException( "Variable " + baseVariable->getFullName( frame ) + " does not have members" );       
    
        if ( identifier == NULL )
            throw RbException( "Member variable identifier missing" );

        MemberObject* theMemberObject = static_cast<MemberNode*>( baseVar )->getMemberObject();
        Frame& members = const_cast<Frame&>( theMemberObject->getMembers() );
        theVar = members.getVariableSlot( *identifier ).getParserVariable();
    }

    /* Find return value; easy if not an element */
    if ( elemIndex.size() == 0 )
        return theVar;

    /* We need to find the element. First ask if it exists to avoid a memory leak. */
    if ( !theVar->existsElement( elemIndex ) )
        throw RbException( getFullName( frame ) + " does not exist" );

    DAGNode* elem = theVar->getElement( elemIndex );
    if ( theVar->numRefs() == 0 )
        delete theVar;
    return elem;
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

