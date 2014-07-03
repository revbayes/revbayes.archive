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

#include "Argument.h"
#include "Environment.h"
#include "Integer.h"
#include "Natural.h"
#include "RbException.h"
#include "RevObject.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "Vector.h"
#include "SyntaxFunctionCall.h"
#include "Variable.h"
#include "SyntaxVariable.h"

using namespace RevLanguage;

/** Construct from identifier and index */
SyntaxVariable::SyntaxVariable(const std::string &n, std::list<SyntaxElement*>* indx) : SyntaxElement(), identifier(n), functionCall(NULL), index(indx), baseVariable(NULL), replacementValue( NULL ) {

}


/** Construct from function call and index */
SyntaxVariable::SyntaxVariable(SyntaxFunctionCall* fxnCall, std::list<SyntaxElement*>* indx) : SyntaxElement(), identifier(""), functionCall(fxnCall), index(indx), baseVariable(NULL), replacementValue( NULL ) {

}


/** Construct from base variable (member object), identifier and index */
SyntaxVariable::SyntaxVariable(SyntaxVariable* var, const std::string &n, std::list<SyntaxElement*>* indx) : SyntaxElement(), identifier(n), functionCall(NULL), index(indx), baseVariable(var), replacementValue( NULL ) {

}


/** Construct from base variable (member object), function call and index */
SyntaxVariable::SyntaxVariable(SyntaxVariable* var, SyntaxFunctionCall* fxnCall, std::list<SyntaxElement*>* indx) : SyntaxElement(), identifier(""), functionCall(fxnCall), index(indx), baseVariable(var), replacementValue( NULL ) {

}


/** Deep copy constructor */
SyntaxVariable::SyntaxVariable(const SyntaxVariable& x) : SyntaxElement(x) {

    identifier = x.identifier;
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


/** Get index */
std::vector<int> SyntaxVariable::computeIndex( Environment& env ) {
    
    std::vector<int> theIndex;
    
    int count = 1;
    for ( std::list<SyntaxElement*>::iterator i=index->begin(); i!=index->end(); i++, count++ ) {
        
        if ( (*i) == NULL )
            
            theIndex.push_back( -1 );
        
        else {
            
            RevPtr<Variable> indexVar = (*i)->evaluateContent( env );
            
            if ( indexVar->getRevObject().isTypeSpec( Integer::getClassTypeSpec() ) ) {
                
                // Calculate (or get) an integer index
                int intIndex = static_cast<Integer &>( indexVar->getRevObject() ).getValue(); 
                
                if ( intIndex < 1 ) {
                    
                    std::ostringstream msg;
                    msg << "Index " << count << " for ";
                    if ( baseVariable != NULL )
                        msg << baseVariable->getFullName( env ) << ".";
                    msg << identifier;
                    msg << " smaller than 1";
                    throw RbException( msg );
                }
                
                // Get zero-based value corresponding to integer index
                theIndex.push_back( intIndex-1 );
            }
            
            else if ( indexVar->getRevObject().isTypeSpec( RlString::getClassTypeSpec() ) ) {
                
                // Push RlString index onto index vector
//                theIndex.push_back( indexVar->getValue()->clone() );
            }
            
            else {
                
                std::ostringstream msg;
                msg << "Index " << count << " for ";
                if ( baseVariable != NULL )
                    msg << baseVariable->getFullName( env ) << ".";
                msg << identifier;
                msg << " of wrong type (neither " << Integer::getClassName() << " nor " << RlString::getClassName() << ")";
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
RevPtr<Variable> SyntaxVariable::createVariable( Environment& env) {
    
    /* Get index */
    std::vector<int> indices = computeIndex(env);
    
    VariableSlot* theSlot = NULL;
    
    if ( baseVariable == NULL ) {
        
        if ( functionCall == NULL ) {

            if ( !env.existsVariable( identifier ) ) {
                // create a new slot
                RevPtr<Variable> theVar = RevPtr<Variable>( new Variable( NULL ) );
                env.addVariable(identifier,theVar);
                theVar->setName( identifier );
            }
            
            // get the slot and variable
            theSlot                 = &env[ identifier ]; // \TODO: We should not allow dereferencing!!!
            
            if (!indices.empty() && theSlot != NULL) {
                
                if ( !theSlot->doesVariableExist( indices ) ) {
                    theSlot->createVariable( indices );
                }
            }
            
            RevPtr<Variable> theVar  = theSlot->getVariable( indices );
                
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

    return theSlot->getVariable( indices );
}


/**
 * @brief Get semantic value (r-value) of deterministic expression
 *
 * This function returns the semantic value of the variable expression
 * when it is part of a deterministic expression. In this case, we
 * should not return the variable or a variable element itself but an
 * indirect reference to it. Among other things, this takes care of
 * reassignment and proper updating of the variable that is returned.
 */
RevPtr<Variable> SyntaxVariable::evaluateDeterministicExpressionContent( Environment& env) {
    
    // @todo Fredrik: Deal with control variables and loop variables. Do we want to throw
    // an error or generate references to immutable constants?
    if ( replacementValue != NULL )
        throw RbException( "Loop variables in deterministic expressions not supported (yet)" );
    
    /* Get variable */
    RevPtr<Variable> theVar = NULL;
    
    size_t usedIndices = 0;
    
    // if the base variable is not set we have a simple object, otherwise a member object
    if ( baseVariable == NULL ) {
        
        if ( functionCall == NULL ) {
            // we test whether this variable exists
            if ( env.existsVariable( identifier ) ) {
                VariableSlot& theSlot = env[ identifier ];
                
                std::vector<int> slotIndices;
                // @todo Fredrik: This does not work for dynamic lookups. Wait to see if we can get rid of variable fields
                // before trying to correct this.
                std::list<SyntaxElement*>::const_iterator it=index->begin();
                for (size_t i = 0; i < theSlot.getDim() && i < index->size(); ++i, ++it, ++usedIndices) {
                    SyntaxElement*         indexSyntaxElement     = *it;
                    RevPtr<Variable>        indexVar               = indexSyntaxElement->evaluateContent(env);
                    
                    // we assume that the indices have to be natural values
                    if ( indexVar->getRevObject().isTypeSpec( Natural::getClassTypeSpec() ) ) {
                        Natural &n = static_cast<Natural &>( indexVar->getRevObject() );
                        slotIndices.push_back( n.getValue()-1 );
                    } else if (indexVar->getRevObject().isConvertibleTo( Natural::getClassTypeSpec() ) ) {
                        RevObject* convObj = indexVar->getRevObject().convertTo( Natural::getClassTypeSpec() );
                        Natural *n = static_cast<Natural *>( convObj );
                        int tmp = n->getValue()-1;
                        slotIndices.push_back( tmp );
                        
                        delete n;
                    } else {
                        throw RbException("Only natural numbers are allowed as indices for variable slots.");
                    }
                }
                
                if ( slotIndices.size() != 0 )
                    throw RbException( "Dynamic reference to field variables not supported yet" );

                // First get the variable we want to reference
                RevObject* theObj = &(theSlot.getVariable(slotIndices)->getRevObject());
                RevObject* theReference = theObj->makeDagReference();

                // Now make a new variable, which is a reference to that variable
                theVar = RevPtr<Variable>( new Variable( theReference, "" ) );
            }
            else if ( env.existsFunction( identifier ) ) {
                const Function& theFunction = env.getFunction( identifier );
                theVar = RevPtr<Variable>( new Variable( theFunction.clone() ) );
            }
            else {
                // there is no variable with that name and also no function
                throw RbException("No variable or function with name \"" + identifier + "\" found!");
            }
        } else {
            theVar = functionCall->evaluateContent( env );
        }
    }
    else {
        
        if ( functionCall == NULL ) {
            
            // The call to getValue of baseVariable either returns
            // a value or results in the throwing of an exception
            const RevPtr<Variable>& baseVar = baseVariable->evaluateContent( env );
            
            if ( identifier == "" )
                throw RbException( "Member variable identifier missing" );
            
            const RevObject &theMemberObject = baseVar->getRevObject();
            const RevObject* member = theMemberObject.getMember( identifier );
            
            // test whether we actually got a variable back
            if ( member != NULL ) {
                theVar = new Variable( member->clone(), "" );
            }
            else {
                throw RbException("Cannot find member '" + identifier + "' of variable '" + baseVar->getName() + "'.");
            }
        }
        else {
            
            functionCall->setBaseVariable( baseVariable->clone() );
            theVar = functionCall->evaluateContent( env );
        }
    }
    
    if (!index->empty()) {
        
        /* We want to create a dynamic lookup to an element of a container variable */
        std::list<SyntaxElement*>::const_iterator it= index->begin();
        for (size_t i = 0; i < usedIndices; ++i) {
            ++it;
        }
        for (; it!=index->end(); it++)
        {
            /* The index variables are going to be inserted as arguments in the index operator function, so we can use standard evaluateContent fxn */
            SyntaxElement*         indexSyntaxElement     = *it;
            RevPtr<Variable>        indexVar               = indexSyntaxElement->evaluateContent(env);
            
            //theVar = new Variable( new ConstantNode( static_cast<RevObject*>( subElement.clone() ) ) );
            
            // create the new variable name
            std::string varName = theVar->getName() + "[" + indexVar->getRevObject().toString() + "]";
            
            // convert the value into a member object
            RevObject &mObject = theVar->getRevObject();
            
            // get the method table for this member object
            // \TODO: We should not allow const casts
            MethodTable& mt = const_cast<MethodTable&>( mObject.getMethods() );
            
            // create the arguments which consist only of the single paramater inside the square brackets
            std::vector<Argument> args;
            args.push_back( Argument( indexVar ) );
            
            // get the member function with name "[]"
            MemberFunction* theMemberFunction = static_cast<MemberFunction*>( mt.getFunction( "[]", args ).clone() );
            theMemberFunction->processArguments( args );
            // We need to clone because otherwise we overwrite all methods for this object
            
            // set the member object for the member function
            theMemberFunction->setMemberObject( theVar );
            //            RevPtr<Function> func( theMemberFunction );
            
            RevObject* subElement = theMemberFunction->execute();
            delete theMemberFunction;
            
            theVar = RevPtr<Variable>( new Variable( subElement, varName ) );
        }
    }

    return theVar;
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
RevPtr<Variable> SyntaxVariable::evaluateContent( Environment& env) {
    
    // test whether this variable was replace inside a loop
    if ( replacementValue != NULL ) 
    {
        RevPtr<Variable> theVar = RevPtr<Variable>( new Variable( replacementValue->clone() ) );
        return theVar;
    }

    /* Get variable */
    RevPtr<Variable> theVar = NULL;
    
    size_t usedIndices = 0;
    
    // if the base variable is not set we have a simple object, otherwise a member object 
    if ( baseVariable == NULL ) {
        
        if ( functionCall == NULL ) {
            // we test whether this variable exists
            if ( env.existsVariable( identifier ) ) {
                VariableSlot& theSlot = env[ identifier ];
                
                // 
                std::vector<int> slotIndices;
                std::list<SyntaxElement*>::const_iterator it=index->begin();
                for (size_t i = 0; i < theSlot.getDim() && i < index->size(); ++i, ++it, ++usedIndices) {
                    SyntaxElement*         indexSyntaxElement     = *it;
                    RevPtr<Variable>        indexVar               = indexSyntaxElement->evaluateContent(env);
                    
                    // we assume that the indices have to be natural values
                    if ( indexVar->getRevObject().isTypeSpec( Natural::getClassTypeSpec() ) ) {
                        Natural &n = static_cast<Natural &>( indexVar->getRevObject() );
                        if ( n.getValue() <= 0 )
                            throw RbException("Only natural numbers are allowed as indices for variable slots. Also, we start counting indices from 1.");
                        
                        slotIndices.push_back( n.getValue()-1 );
                    } else if (indexVar->getRevObject().isConvertibleTo( Natural::getClassTypeSpec() ) ) {
                        RevObject* convObj = indexVar->getRevObject().convertTo( Natural::getClassTypeSpec() );
                        Natural *n = static_cast<Natural *>( convObj );
                        int tmp = n->getValue()-1;
                        slotIndices.push_back( tmp );
                        
                        delete n;
                    } else {
                        throw RbException("Only natural numbers without 0 are allowed as indices for variable slots.");
                    }
                }
                
                theVar = theSlot.getVariable(slotIndices);
            } 
            else if ( env.existsFunction( identifier ) ) {
                const Function& theFunction = env.getFunction( identifier );
                theVar = RevPtr<Variable>( new Variable( theFunction.clone() ) );
            } 
            else {
                // there is no variable with that name and also no function
                throw RbException("No variable or function with name \"" + identifier + "\" found!");
            }
        } else {
            theVar = functionCall->evaluateContent( env );
        }
    }
    else {
        
        if ( functionCall == NULL ) {

            // The call to getValue of baseVariable either returns
            // a value or results in the throwing of an exception
            const RevPtr<Variable>& baseVar = baseVariable->evaluateContent( env );
        
            if ( identifier == "" )
                throw RbException( "Member variable identifier missing" );

            const RevObject &theMemberObject = baseVar->getRevObject();
            const RevObject* member = theMemberObject.getMember( identifier );

            // test whether we actually got a variable back
            if ( member != NULL ) {
                theVar = new Variable( member->clone(), "" );
            }
            else {
                throw RbException("Cannot find member '" + identifier + "' of variable '" + baseVar->getName() + "'.");
            }
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
        for (; it!=index->end(); it++) 
        {
            SyntaxElement*         indexSyntaxElement     = *it;
            RevPtr<Variable>        indexVar               = indexSyntaxElement->evaluateContent(env);
            
            
            //theVar = new Variable( new ConstantNode( static_cast<RevObject*>( subElement.clone() ) ) );
            
            // create the new variable name
            std::string varName = theVar->getName() + "[" + indexVar->getRevObject().toString() + "]";
            
            // convert the value into a member object
            RevObject &mObject = theVar->getRevObject();
            
            // get the method table for this member object
            // \TODO: We should not allow const casts
            MethodTable& mt = const_cast<MethodTable&>( mObject.getMethods() );
            
            // create the arguments which consist only of the single paramater inside the square brackets
            std::vector<Argument> args;
            args.push_back( Argument( indexVar ) );
            
            // get the member function with name "[]"
            MemberFunction* theMemberFunction = static_cast<MemberFunction*>( mt.getFunction( "[]", args ).clone() );
            theMemberFunction->processArguments( args );
            // We need to clone because otherwise we overwrite all methods for this object
            
            // set the member object for the member function
            theMemberFunction->setMemberObject( theVar );
//            RevPtr<Function> func( theMemberFunction );
            
            RevObject* subElement = theMemberFunction->execute();
            delete theMemberFunction;
            
            theVar = RevPtr<Variable>( new Variable( subElement, varName ) );
        }
    }
        
    return theVar;
    
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
    o << "identifier = " << identifier << std::endl;
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
void SyntaxVariable::replaceVariableWithConstant(const std::string& name, const RevObject& c) {
    // test whether this variable is the one we are looking for
    if ( identifier != "" && name == identifier ) {
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

