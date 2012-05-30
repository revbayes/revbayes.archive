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
#include "ConstArgument.h"
#include "DeterministicNode.h"
#include "Environment.h"
#include "Integer.h"
#include "MemberObject.h"
#include "Natural.h"
#include "Plate.h"
#include "RbException.h"
#include "RbLanguageObject.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "RlVector.h"
#include "SyntaxFunctionCall.h"
#include "Variable.h"
#include "SyntaxVariable.h"


/** Construct from identifier and index */
SyntaxVariable::SyntaxVariable(RbString* id, std::list<SyntaxElement*>* indx) :
    SyntaxElement(), identifier(id), functionCall(NULL), index(indx), baseVariable(NULL), replacementValue( NULL ) {
}


/** Construct from function call and index */
SyntaxVariable::SyntaxVariable(SyntaxFunctionCall* fxnCall, std::list<SyntaxElement*>* indx) :
    SyntaxElement(), identifier(NULL), functionCall(fxnCall), index(indx), baseVariable(NULL), replacementValue( NULL ) {
}


/** Construct from base variable (member object), identifier and index */
SyntaxVariable::SyntaxVariable(SyntaxVariable* var, RbString* id, std::list<SyntaxElement*>* indx) :
    SyntaxElement(), identifier(id), functionCall(NULL), index(indx), baseVariable(var), replacementValue( NULL ) {
}


/** Construct from base variable (member object), function call and index */
SyntaxVariable::SyntaxVariable(SyntaxVariable* var, SyntaxFunctionCall* fxnCall, std::list<SyntaxElement*>* indx) :
    SyntaxElement(), identifier(NULL), functionCall(fxnCall), index(indx), baseVariable(var), replacementValue( NULL ) {
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
    
    if (x.replacementValue != NULL) {
        replacementValue = x.replacementValue->clone();
    }
    else {
        replacementValue = NULL;
    }
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
    
    delete replacementValue;
}


/** Assignment operator */
SyntaxVariable& SyntaxVariable::operator=(const SyntaxVariable& x) {

    if (&x != this) {

        SyntaxElement::operator=(x);

        identifier = x.identifier;
        functionCall = x.functionCall;
        baseVariable = x.baseVariable;

        index = x.index;
        
        delete replacementValue;
        if (x.replacementValue != NULL) {
            replacementValue = x.replacementValue->clone();
        }
        else {
            replacementValue = NULL;
        }
    }

    return (*this);
}



/** Clone syntax element */
SyntaxVariable* SyntaxVariable::clone () const {

    return new SyntaxVariable(*this);
}


/** Get class name of object */
const std::string& SyntaxVariable::getClassName(void) { 
    
    static std::string rbClassName = "Syntax variable";
    
	return rbClassName; 
}


/** Get index */
RlVector<Natural> SyntaxVariable::computeIndex( Environment& env ) {
    
    RlVector<Natural> theIndex;
    
    int count = 1;
    for ( std::list<SyntaxElement*>::iterator i=index->begin(); i!=index->end(); i++, count++ ) {
        
        if ( (*i) == NULL )
            
            theIndex.push_back( Natural(-1) );
        
        else {
            
            RbPtr<DAGNode> indexVar = (*i)->evaluateContent( env )->getDagNode();
            
            if ( indexVar->getValue().isTypeSpec( Integer::getClassTypeSpec() ) ) {
                
                // Calculate (or get) an integer index
                int intIndex = static_cast<Integer *>( (RbLanguageObject *) indexVar->getValue().getSingleValue() )->getValue(); 
                
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
                theIndex.push_back( Natural(intIndex-1) );
            }
            
            else if ( indexVar->getValue().isTypeSpec( RbString::getClassTypeSpec() ) ) {
                
                // Push string index onto index vector
//                theIndex.push_back( indexVar->getValue()->clone() );
            }
            
            else {
                
                std::ostringstream msg;
                msg << "Index " << count << " for ";
                if ( baseVariable != NULL )
                    msg << baseVariable->getFullName( env ) << ".";
                msg << *identifier;
                msg << " of wrong type (neither " << Integer::getClassName() << " nor " << RbString::getClassName() << ")";
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
RbPtr<Variable> SyntaxVariable::createVariable( Environment& env) {
    
    /* Get index */
    RlVector<Natural> indices = computeIndex(env);
    
    VariableSlot* theSlot = NULL;
    
    /* Get variable */
    RbPtr<DAGNode> theDagNode = NULL;
    if ( baseVariable == NULL ) {
        
        if ( functionCall == NULL ) {

            if ( !env.existsVariable( identifier->getValue() ) ) {
                // create a new slot
                RbPtr<Variable> theVar = RbPtr<Variable>( new Variable( RbObject::getClassTypeSpec() ) );
                env.addVariable(identifier->getValue(),theVar);
            }
            
            // get the slot and variable
            theSlot                 = &env[ identifier->getValue() ]; // \TODO: We should not allow dereferencing!!!
            
            if (!indices.empty() && theSlot != NULL) {
                
                if ( !theSlot->doesVariableExist( indices.getValue() ) ) {
                    theSlot->createVariable( indices.getValue() );
                }
            }
            
            RbPtr<Variable> theVar  = theSlot->getVariable( indices.getValue() );
            theDagNode              = theVar->getDagNode();
                
        }
        else {

            throw RbException("Unexpected syntax for creating a variable. Left-hand-side should be a variable and cannot be a function call!");
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

    return theSlot->getVariable( indices.getValue() );
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
RbPtr<Variable> SyntaxVariable::evaluateContent( Environment& env) {
    
    // test whether this variable was replace inside a loop
    if ( replacementValue != NULL ) {
        RbPtr<Variable> theVar = RbPtr<Variable>( new Variable( new ConstantNode( RbPtr<RbLanguageObject>( replacementValue->clone() ) ) ) );
        return theVar;
    }

    /* Get variable */
    RbPtr<Variable> theVar = NULL;
    
    size_t usedIndices = 0;
    
    // if the base variable is not set we have a simple object, otherwise a member object 
    if ( baseVariable == NULL ) {
        
        if ( functionCall == NULL ) {
            // we test whether this variable exists
            if ( env.existsVariable( identifier->getValue() ) ) {
                VariableSlot& theSlot = env[ identifier->getValue() ];
                
                // 
                std::vector<int> slotIndices;
                std::list<SyntaxElement*>::const_iterator it=index->begin();
                for (size_t i = 0; i < theSlot.getDim() && i < index->size(); ++i, ++it, ++usedIndices) {
                    SyntaxElement*         indexSyntaxElement     = *it;
                    RbPtr<Variable>        indexVar               = indexSyntaxElement->evaluateContent(env);
                    
                    // we assume that the indices have to be natural values
                    if ( indexVar->getValue().isTypeSpec( Natural::getClassTypeSpec() ) ) {
                        Natural *n = static_cast<Natural *>( (RbLanguageObject *) indexVar->getValue().getSingleValue() );
                        slotIndices.push_back( n->getValue()-1 );
                    } else if (indexVar->getValue().isConvertibleTo( Natural::getClassTypeSpec() ) ) {
                        RlValue<RbObject> convObj = indexVar->getValue().convertTo( Natural::getClassTypeSpec() );
                        Natural *n = static_cast<Natural *>( (RbObject *) convObj.getSingleValue() );
                        int tmp = n->getValue()-1;
                        slotIndices.push_back( tmp );
                    } else {
                        throw RbException("Only natural numbers are allowed as indices for variable slots.");
                    }
                }
                
                theVar = theSlot.getVariable(slotIndices);
            } 
            else if ( env.existsFunction( identifier->getValue() ) ) {
                const RbFunction& theFunction = env.getFunction( identifier->getValue() );
                theVar = RbPtr<Variable>( new Variable( new ConstantNode( RbPtr<RbLanguageObject>( theFunction.clone() ) ) ) );
            } 
            else {
                // there is no variable with that name and also no function
                throw RbException("No variable or function with name \"" + identifier->getValue() + "\" found!");
            }
        } else {
            theVar = functionCall->evaluateContent( env );
        }
    }
    else {
        
        if ( functionCall == NULL ) {

            // The call to getValue of baseVariable either returns
            // a value or results in the throwing of an exception
            const RbPtr<Variable>& baseVar = baseVariable->evaluateContent( env );
            if ( !baseVar->getValue().isTypeSpec( MemberObject::getClassTypeSpec() ) )
                throw RbException( "Variable " + baseVariable->getFullName( env ) + " does not have members" );       
        
            if ( identifier == NULL )
                throw RbException( "Member variable identifier missing" );

            const MemberObject * theMemberObject = static_cast<const MemberObject *>( (const RbLanguageObject *) baseVar->getDagNode()->getValue().getSingleValue() );
            // \TODO:
//            const std::map<std::string, const Variable*>& members = theMemberObject.getMembers();
//            
//            const std::map<std::string, const Variable*>::const_iterator& i = members.find( identifier->getValue() );
//            // test whether we actually got a variable back
//            if ( i != members.end() ) {
//                const Variable* memberAttribute = i->second;
//                theVar->setDagNode( memberAttribute->getDagNode()->clone() );
//            }
//            else {
//                throw RbException("Cannot find member '" + identifier->getValue() + "' of variable '" + baseVar->getDagNode()->getName() + "'.");
//            }
        }
        else {
            
            functionCall->setBaseVariable( baseVariable->clone() );
            theVar = functionCall->evaluateContent( env );
        }
    }
    
    // test whether we want to directly assess the variable or if we want to assess subelement of this container
    if (!index->empty()) {
        
        // iterate over the each index
        std::list<SyntaxElement*>::const_iterator it= index->begin();
        for (size_t i = 0; i < usedIndices; ++i) {
            ++it;
        }
        for (; it!=index->end(); it++) {
            SyntaxElement*         indexSyntaxElement     = *it;
            RbPtr<Variable>        indexVar               = indexSyntaxElement->evaluateContent(env);
            
            
            //theVar = new Variable( new ConstantNode( static_cast<RbLanguageObject*>( subElement.clone() ) ) );
                
            // convert the value into a member object
            MemberObject *mObject = static_cast<MemberObject *>( (RbLanguageObject*) theVar->getValue().getSingleValue() );
            
            // get the method table for this member object
            // \TODO: We should not allow const casts
            MethodTable& mt = const_cast<MethodTable&>( mObject->getMethods() );
            
            // create the arguments which consist only of the single paramater inside the square brackets
            std::vector<RbPtr<Argument> > args;
            args.push_back( new ConstArgument( RbPtr<const Variable>( (Variable *)indexVar ) ) );
            
            // get the member function with name "[]"
            SimpleMemberFunction* theMemberFunction = static_cast<SimpleMemberFunction*>( mt.getFunction( "[]", args ).clone() );
            theMemberFunction->processArguments( args );
            // We need to clone because otherwise we overwrite all methods for this object
            
            // set the member object for the member function
            theMemberFunction->setMemberObject(mObject);
//            RbPtr<RbFunction> func( theMemberFunction );
            
            theVar = RbPtr<Variable>( new Variable( new DeterministicNode( theMemberFunction, NULL ) ) );
        }
    }
        
    return theVar;
    
}

/** Get class type spec describing type of object */
const TypeSpec& SyntaxVariable::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( SyntaxElement::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& SyntaxVariable::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Return nice representation of the syntax element */
std::string SyntaxVariable::getFullName(Environment& env) const {
    
    std::ostringstream theName;
    if (baseVariable != NULL)
        theName << baseVariable->getFullName(env) << ".";
    
    theName << identifier;
    
    return theName.str();
}


/* Does this variable have a function call (e.g a.xxx())?
 * If so, then this will be a function call, otherwise just a variable.
 */
bool SyntaxVariable::hasFunctionCall( void ) const {
    return functionCall != NULL;
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


/**
 * Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
 * 
 */
void SyntaxVariable::replaceVariableWithConstant(const std::string& name, const RbLanguageObject& c) {
    // test whether this variable is the one we are looking for
    if ( identifier != NULL && name == identifier->getValue() ) {
        delete replacementValue;
        replacementValue = c.clone();
    }
    
    // also change the indices if we had some
    if ( index != NULL) {
        for (std::list<SyntaxElement*>::iterator i = index->begin(); i != index->end(); i++) {
            (*i)->replaceVariableWithConstant(name, c);
        }
    }
    
    // also change the function call if we had some
    if ( functionCall != NULL) {
        functionCall->replaceVariableWithConstant(name, c);
    }
    
    // also change the base variable if we had some
    if ( baseVariable != NULL) {
        baseVariable->replaceVariableWithConstant(name, c);
    }
}

