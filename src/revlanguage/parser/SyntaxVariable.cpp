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


/**
 * Get constant (static) value of index.
 *
 * @todo Add conversion to Natural when needed
 */
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
 * Evaluate the dynamic content of the index variables.
 *
 * @todo Replace static conversion with dynamic conversion
 */
std::vector< RevPtr<Variable> > SyntaxVariable::computeDynamicIndex( Environment& env )
{
    std::vector< RevPtr<Variable> > indexVars;

    std::list<SyntaxElement*>::const_iterator it;
    for ( it = index->begin(); it != index->end(); ++it )
    {
        RevPtr<Variable> theIndex = ( *it )->evaluateDynamicContent( env );
    
        // We ensure that the variables are of type Natural or can be converted to Natural numbers
        // No sense in checking indices against permissible range here; errors are thrown later if
        // we are out of range by the container or member object
        // TODO: This is static conversion, should be replaced by dynamic conversion
        if ( !theIndex->getRevObject().isTypeSpec( Natural::getClassTypeSpec() ) )
        {
            if (theIndex->getRevObject().isConvertibleTo( Natural::getClassTypeSpec() ) )
            {
                RevObject* convObj = theIndex->getRevObject().convertTo( Natural::getClassTypeSpec() );
                
                theIndex = new Variable( new Natural() );
                theIndex->setRevObject( convObj );
            }
            else
                throw RbException( "No known conversion of type '" + theIndex->getRevObject().getType() + "' to Natural, required for index");
        }
    }
    
    return indexVars;
}

        
/**
 * @brief Evaluate semantic content (r-value) (general case)
 *
 * The variable can either be a member or a base variable. In the latter
 * case, its "baseVariable" member is NULL. If the element is a base variable,
 * we get the semantic value of the element by looking it up in the frame.
 * If it is a member variable, we try to find it in the member variable
 * frame of a composite variable found by another SyntaxVariable element.
 *
 * A function call may replace the identifier. If so, then there will always
 * be a list of indices, whereas the list of indices is optional for an
 * identifier.
 */
RevPtr<Variable> SyntaxVariable::evaluateContent( Environment& env) {
    
    // test whether this variable was replace inside a loop. TODO: REMOVE!!!
    if ( replacementValue != NULL )
    {
        RevPtr<Variable> theVar = RevPtr<Variable>( new Variable( replacementValue->clone() ) );
        return theVar;
    }
    
    RevPtr<Variable> theVar;
    
    if ( baseVariable == NULL )
    {
        if ( functionCall == NULL )
        {
            // Get variable from the environment
            theVar = env.getVariable( identifier );
        }
        else
        {
            // Get the return variable of the function call
            theVar = functionCall->evaluateContent( env );
        }
    }
    else {
        
        // Get the base variable
        theVar = baseVariable->evaluateContent( env );
        
        if ( functionCall == NULL )
        {
            // Find member variable
            theVar = theVar->getRevObject().getMember( identifier );
        }
        else
        {
            // Get the return variable of the function call
            functionCall->setBaseVariable( baseVariable );
            theVar = functionCall->evaluateContent( env );
        }
    }
    
    // Get dynamic index
    std::vector< RevPtr<Variable> > indices = computeDynamicIndex( env );
    
    // Get dynamic element of a container if indices are provided.
    if ( !indices.empty() )
        theVar = theVar->getRevObject().getDynamicElement( indices );
    
    // Return the variable for assignment
    return theVar;
}


/**
 * @brief Evaluate left-hand-side content
 *
 * This function is similar to evaluateContent(). However, we
 * do not throw an error if the variable does not exist in the
 * frame; instead, we create and return a new null variable.
 */
RevPtr<Variable> SyntaxVariable::evaluateLHSContent( Environment& env)
{
    // Get index
    std::vector<int> indices = computeIndex( env );
    
    RevPtr<Variable> theVar;
    
    if ( baseVariable == NULL )
    {
        if ( functionCall == NULL )
        {
            // Find or create the variable.
            theVar = env.findOrCreateVariable( identifier );
        }
        else
        {
            // Get the return variable of the function call
            theVar = functionCall->evaluateContent( env );
        }
    }
    else {

        // Get the base variable. Note that we do not create the variable in this case.
        theVar = baseVariable->evaluateContent( env );

        if ( functionCall == NULL )
        {
            // Find member variable
            theVar = theVar->getRevObject().getMember( identifier );
        }
        else
        {
            // Get the return variable of the function call
            functionCall->setBaseVariable( baseVariable );
            theVar = functionCall->evaluateContent( env );
        }
    }
    
    // Get element of a container if indices are provided
    if ( !indices.empty() )
        theVar = theVar->getRevObject().findOrCreateElement( indices );

    // Return the variable for assignment
    return theVar;
}


/**
 * @brief Evaluate indirect reference content
 *
 * This function returns the semantic value of the variable expression
 * when it is part of a an indirect reference deterministic assignment.
 * These are assignments of the type:
 *
 *    a := b
 *
 * where we deal here with the 'b' part.
 *
 * In this case, we should not return the variable or a variable element
 * itself but an indirect reference to it. Among other things, this takes
 * care of reassignment and proper updating of the variable that is returned
 * and will be assigned to the lhs variable.
 */
RevPtr<Variable> SyntaxVariable::evaluateIndirectReferenceContent( Environment& env) {
    
    // @todo Fredrik: Deal with control variables and loop variables. Do we want to throw
    // an error or generate references to immutable constants?
    if ( replacementValue != NULL )
        throw RbException( "Loop variables in deterministic expressions not supported (yet)" );
    
    RevPtr<Variable> theVar;
    
    if ( baseVariable == NULL )
    {
        if ( functionCall == NULL )
        {
            // Get variable from the environment
            theVar = env.getVariable( identifier );
        }
        else
        {
            // Get the return variable of the function call
            theVar = functionCall->evaluateContent( env );
        }
    }
    else {
        
        // Get the base variable
        theVar = baseVariable->evaluateContent( env );
        
        if ( functionCall == NULL )
        {
            // Find member variable
            theVar = theVar->getRevObject().getMember( identifier );
        }
        else
        {
            // Get the return variable of the function call
            functionCall->setBaseVariable( baseVariable );
            theVar = functionCall->evaluateContent( env );
        }
    }
    
    // Get index
    std::vector< RevPtr<Variable> > indices = computeDynamicIndex( env );
    
    // Get dynamic element of a container if indices are provided. Otherwise we
    // need an indirect reference. Note that if we have a function call, then we
    // always have indices; this is a syntactic requirement because otherwise the
    // syntax element will be a function call and not a variable. This means that
    // we do not have to test for functionCall being NULL in the second branch.
    if ( !indices.empty() )
        theVar = theVar->getRevObject().getDynamicElement( indices );
    else
        theVar = new Variable( theVar->getRevObject().makeDagReference() );
    
    // Return the variable for assignment
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

