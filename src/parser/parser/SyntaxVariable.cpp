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

#include "ConstantNode.h"
#include "DagNodeContainer.h"
#include "DeterministicNode.h"
#include "Environment.h"
#include "Integer.h"
#include "MemberFunction.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbLanguageObject.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "SyntaxFunctionCall.h"
#include "Variable.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include "SyntaxVariable.h"


// Definition of the static type spec member
const TypeSpec SyntaxVariable::typeSpec(SyntaxVariable_name);

/** Construct from identifier and index */
SyntaxVariable::SyntaxVariable(RbString* id, std::list<SyntaxElement*>* indx) :
    SyntaxElement(), identifier(id), functionCall(NULL), index(indx), baseVariable(NULL) {
}


/** Construct from function call and index */
SyntaxVariable::SyntaxVariable(SyntaxFunctionCall* fxnCall, std::list<SyntaxElement*>* indx) :
    SyntaxElement(), identifier(NULL), functionCall(fxnCall), index(indx), baseVariable(NULL) {
}


/** Construct from base variable (member object), identifier and index */
SyntaxVariable::SyntaxVariable(SyntaxVariable* var, RbString* id, std::list<SyntaxElement*>* indx) :
    SyntaxElement(), identifier(id), functionCall(NULL), index(indx), baseVariable(var) {
}


/** Construct from base variable (member object), function call and index */
SyntaxVariable::SyntaxVariable(SyntaxVariable* var, SyntaxFunctionCall* fxnCall, std::list<SyntaxElement*>* indx) :
    SyntaxElement(), identifier(NULL), functionCall(fxnCall), index(indx), baseVariable(var) {
}


/** Deep copy constructor */
SyntaxVariable::SyntaxVariable(const SyntaxVariable& x) : SyntaxElement(x) {

    identifier = x.identifier->clone();
    if ( x.baseVariable != NULL )
        baseVariable = x.baseVariable->clone();
    else
        baseVariable = NULL;
    if ( x.functionCall != NULL )
        functionCall = x.functionCall->clone();
    else
        functionCall = NULL;
    if ( x.index != NULL ) {
        index = new std::list<SyntaxElement*>();
        for (std::list<SyntaxElement*>::const_iterator i=x.index->begin(); i!=x.index->end(); i++) {
            index->push_back( (*i)->clone() );
        }
    }
    else
        index = NULL;
}


/** Destructor deletes variable, identifier and index */
SyntaxVariable::~SyntaxVariable() {
    
    delete identifier;
    delete functionCall;
    
    if (baseVariable != NULL) {
        delete baseVariable;
    }
    
    
    for (std::list<SyntaxElement*>::iterator i=index->begin(); i!=index->end(); i++) {
        delete *i;
    }
    delete index;
}


/** Assignment operator */
SyntaxVariable& SyntaxVariable::operator=(const SyntaxVariable& x) {

    if (&x != this) {

        SyntaxElement::operator=(x);

        identifier = x.identifier;
        functionCall = x.functionCall;
        baseVariable = x.baseVariable;

        index = x.index;
    }

    return (*this);
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


/** Return nice representation of the syntax element */
std::string SyntaxVariable::getFullName(Environment& env) const {

    std::ostringstream theName;
    if (baseVariable != NULL)
        theName << baseVariable->getFullName(env) << ".";

    theName << std::string(*identifier);

    return theName.str();
}


/** Get index */
VectorNatural SyntaxVariable::computeIndex( Environment& env ) {
    
    VectorNatural   theIndex;
    
    int count = 1;
    for ( std::list<SyntaxElement*>::iterator i=index->begin(); i!=index->end(); i++, count++ ) {
        
        if ( (*i) == NULL )
            
            theIndex.push_back( -1 );
        
        else {
            
            DAGNode* indexVar = (*i)->evaluateContent( env )->getDagNode();
            
            if ( indexVar->getValue().isTypeSpec( TypeSpec(Integer_name) ) ) {
                
                // Calculate (or get) an integer index
                int intIndex = static_cast<const Integer&>( indexVar->getValue() ).getValue(); 
                
                if ( intIndex < 1 ) {
                    
                    std::ostringstream msg;
                    msg << "Index " << count << " for ";
                    if ( baseVariable != NULL )
                        msg << baseVariable->getFullName( env ) << ".";
                    msg << *identifier;
                    msg << " smaller than 1";
                    throw RbException( msg );
                }
                
                // Get zero-based value corresponding to integer index
                theIndex.push_back(intIndex-1);
            }
            
            else if ( indexVar->getValue().isTypeSpec( TypeSpec(RbString_name) ) ) {
                
                // Push string index onto index vector
//                theIndex.push_back( indexVar->getValue()->clone() );
            }
            
            else {
                
                std::ostringstream msg;
                msg << "Index " << count << " for ";
                if ( baseVariable != NULL )
                    msg << baseVariable->getFullName( env ) << ".";
                msg << *identifier;
                msg << " of wrong type (neither " << Integer_name << " nor " << RbString_name << ")";
                throw RbException( msg );
            }
        }
    }
    
    // Return index
    return theIndex;
}


/**
 * @brief Get semantic value (l-value)
 *
 * This function is similar to getValue(), but we only get the
 * slot and index of the variable. Another difference is that we
 * do not throw an error if the variable does not exist in the
 * frame; instead, we return a NULL pointer and set theSlot pointer
 * to NULL as well.
 */
VariableSlot& SyntaxVariable::createVariable( Environment& env) {
    
    /* Get index */
    VectorNatural indices = computeIndex(env);
    
    VariableSlot* theSlot = NULL;
    
    /* Get variable */
    DAGNode* theDagNode = NULL;
    if ( baseVariable == NULL ) {
        
        if ( functionCall == NULL ) {

            if ( !env.existsVariable( *identifier ) ) {
                // create a new slot
                Variable* theVar = new Variable( *identifier );
                env.addVariable(*identifier,theVar);
            }
            
            // get the slot and variable
            theSlot          = &env[ (*identifier) ]; // TODO: We should not allow dereferencing!!!
            Variable& theVar = theSlot->getVariable();
            theDagNode       = theVar.getDagNode();
                
        }
        else {

//            theDagNode  = functionCall->getContentAsDagNode( env );
//            theVar      = theDagNode->getVariable();
        }
    }
    else {

        // The call to getValue of baseVariable either returns
        // a value or results in the throwing of an exception
//        DAGNode* baseVar = baseVariable->getContentAsVariable( env )->getDagNodePtr();
//        if ( !baseVar->isType( MemberNode_name ) )
            throw RbException( "Variable " + baseVariable->getFullName( env ) + " does not have members. Missing implementation in SyntaxVariable::getLValue()" );       
    
//        if ( identifier == NULL )
//            throw RbException( "Member variable identifier missing" );
//
//        MemberObject* theMemberObject = static_cast<MemberNode*>( baseVar )->getMemberObject();
//        if ( !theMemberObject->getMembers().existsVariable( *identifier ) )
//            throw RbException( "Variable " + baseVariable->getFullName( frame ) + " does not have a member called '" + *identifier + "'" );       
//
//        MemberFrame& members = const_cast<MemberFrame&>( theMemberObject->getMembers() );
//        theSlot = &members[ (*identifier) ];
//        theVar  = theSlot->getParserVariable();
    }
    
    std::string name = identifier->getValue();
    
    if (!indices.empty() && theSlot != NULL) {
        
        // iterate over the each index
        while (!indices.empty()) {
            // test whether the value of the DAG node allows assignment of variable to its elemens
            // e.g.: A simplex might not allow assignment of its elements whereas a DagNodeContainer does
            if (theDagNode != NULL && !theDagNode->getValue().allowsVariableInsertion()) {
                // throw expection because we don't allow RbDagNodePtrion of variable
                std::ostringstream msg;
                msg << "Object of type " << theDagNode->getValue().getType() << " does not allow RbDagNodePtrion of variables.";
                throw RbException(msg);
            }
            
            // test whether this element support subscipting
            if (theDagNode != NULL && !theDagNode->getValue().supportsIndex()) {
                throw RbException("DAG node does not support indexing.");
            }
            
            // take the first index and remove it
            size_t                      indexValue              = indices[0];
            indices.pop_front();
            
            // add the index to the name
            std::stringstream out;
            out << (indexValue+1);
            name += "[" + out.str() + "]";
            
            // get the element at the index
            if (theDagNode == NULL) {
                theDagNode = new ConstantNode( new DagNodeContainer(indexValue+1) );
                theSlot->getVariable().setDagNode(theDagNode);
            }
            
            Container& con = static_cast<Container&>( theDagNode->getValue() );
            // test if the container is large enough
            if (con.size() <= indexValue) {
                con.resize(indexValue);
            }
            RbObject& subElement = con.getElement(indexValue);
            
            // test whether the element needs type conversion
            if (subElement.isTypeSpec( TypeSpec(VariableSlot_name) )) {
                theSlot = &dynamic_cast<VariableSlot&>(subElement);
                theDagNode = theSlot->getDagNode();
                theSlot->getVariable().setName(name);
            }

        }
    }

    return *theSlot;
}


/** We cannot perform this function and throw and error */
Variable* SyntaxVariable::evaluateContent( void ) {
    throw RbException("Cannot evaluate the content in SyntaxVariable without environment!");
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
Variable* SyntaxVariable::evaluateContent( Environment& env) {

    /* Get variable */
    Variable* theVar = NULL;
    
    // if the base variable is not set we have a simple object, otherwise a member object 
    if ( baseVariable == NULL ) {
        
        if ( functionCall == NULL ) {
            theVar = env[ (*identifier) ].getVariable().clone();
        } else {
            theVar = functionCall->evaluateContent( env );
        }
    }
    else {
        
        if ( functionCall == NULL ) {

            // The call to getValue of baseVariable either returns
            // a value or results in the throwing of an exception
            Variable* baseVar = baseVariable->evaluateContent( env );
            if ( !baseVar->getValue().isTypeSpec( TypeSpec(MemberObject_name) ) )
                throw RbException( "Variable " + baseVariable->getFullName( env ) + " does not have members" );       
        
            if ( identifier == NULL )
                throw RbException( "Member variable identifier missing" );

            MemberObject* theMemberObject = static_cast<const MemberObject&>( baseVar->getDagNode()->getValue() ).clone();
            const Environment& members = theMemberObject->getMembers();
            theVar = members[ (*identifier) ].getVariable().clone();
        }
        else {
            
            functionCall->setBaseVariable( baseVariable );
            theVar = functionCall->evaluateContent( env );
        }
    }
    
    // test whether we want to directly assess the variable or if we want to assess subelement of this container
    if (!index->empty()) {
        // iterate over the each index
        for (std::list<SyntaxElement*>::const_iterator it=index->begin(); it!=index->end(); it++) {
            SyntaxElement*         indexSyntaxElement     = *it;
            Variable*              indexVar               = indexSyntaxElement->evaluateContent(env);
            
            if (theVar->getValue().isTypeSpec( TypeSpec(DagNodeContainer_name) )) {
                RbLanguageObject&   theValue               = indexVar->getValue();
                size_t              indexValue             = 0;
                if ( !theValue.isTypeSpec(Natural_name) ) {
                    if (theValue.isConvertibleTo(Natural_name)) {
                        Natural* convertedValue = static_cast<Natural*>( theValue.convertTo(Natural_name) );
                        indexValue = convertedValue->getValue() - 1;
                        delete convertedValue;
                    }
                    else { 
                        throw RbException("Could not access index with type xxx because only natural indices are supported!");
                    }
                }
                else {
                    indexValue = dynamic_cast<const Natural&>( theValue ).getValue() - 1;
                }
                
                RbObject&   subElement  = theVar->getDagNode()->getElement(indexValue);
                            theVar      = dynamic_cast<VariableSlot&>( subElement ).getVariable().clone();
            }
            else {
                //theVar = new Variable( new ConstantNode( static_cast<RbLanguageObject*>( subElement.clone() ) ) );
                
                // convert the value into a member object
                MemberObject& mObject = static_cast<MemberObject&>( theVar->getValue() );
            
                // get the method table for this member object
                // TODO: We should not allow const casts
                MethodTable& mt = const_cast<MethodTable&>( mObject.getMethods() );
            
                // create the arguments which consist only of the single paramater inside the square brackets
                std::vector<Argument> args;
                args.push_back( Argument( indexVar ) );
            
                // get the member function with name "[]"
                MemberFunction* theMemberFunction = static_cast<MemberFunction*>( mt.getFunction( "[]", args ) )->clone(); 
                // We need to clone because otherwise we overwrite all methods for this object
            
                // set the member object for the member function
                theMemberFunction->setMemberObject(mObject);
//            RbPtr<RbFunction> func( theMemberFunction );
            
                theVar = new Variable( new DeterministicNode( theMemberFunction ) );
            }
        }
    }
        
    return theVar;
    
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& SyntaxVariable::getTypeSpec(void) const {
    return typeSpec;
}


/** Print info about the syntax element */
void SyntaxVariable::printValue(std::ostream& o) const {

    o << "<" << this << "> SyntaxVariable:" << std::endl;
    o << "identifier = " << *identifier << std::endl;
    if (baseVariable != NULL) {
        o << "base variable   = <" << baseVariable << "> ";
        baseVariable->printValue(o);
        o << std::endl;
    }
    int count = 1;
    for (std::list<SyntaxElement*>::const_iterator i=(*index).begin(); i!=(*index).end(); i++, count++) {
        if ( (*i) != NULL ) {
            o << "index " << count << " = <" << (*i) << "> ";
            (*i)->printValue(o);
            o << std::endl;
        }
        else {
            o << "index " << count << " = < NULL >" << std::endl;
        }
    }
    o << std::endl;

    for (std::list<SyntaxElement*>::const_iterator i=index->begin(); i!=index->end(); i++)
        (*i)->printValue(o);
}

