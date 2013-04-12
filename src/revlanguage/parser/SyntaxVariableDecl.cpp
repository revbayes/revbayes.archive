/**
 * @file
 * This file contains the implementation of SyntaxVariableDecl, which is
 * used to hold binary expressions in the syntax tree.
 *
 * @brief Implementation of SyntaxVariableDecl
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */


#include "Environment.h"
#include "Integer.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "RlString.h"
#include "SyntaxVariableDecl.h"
#include "Workspace.h"

#include <iostream>
#include <list>
#include <sstream>

using namespace RevLanguage;

/** Construct from operator type, variable and expression */
SyntaxVariableDecl::SyntaxVariableDecl( const std::string &typeName, std::list<SyntaxElement*>* lengths, const std::string &referenceChar, const std::string &varName) : SyntaxElement(), elementTypeName(typeName), lengthExpr(lengths), referenceSymbol(referenceChar), variableName(varName) {
}


/** Deep copy constructor */
SyntaxVariableDecl::SyntaxVariableDecl(const SyntaxVariableDecl& x) : SyntaxElement(x) {
    
    elementTypeName = x.elementTypeName;
    referenceSymbol = x.referenceSymbol;
    variableName    = x.variableName;
    
    lengthExpr = new std::list<SyntaxElement*>();
    
    for ( std::list<SyntaxElement*>::const_iterator i=x.lengthExpr->begin(); i!=x.lengthExpr->end(); i++ )
        lengthExpr->push_back( (*i)->clone() );
}


/** Destructor deletes operands */
SyntaxVariableDecl::~SyntaxVariableDecl() {
    
    for (std::list<SyntaxElement*>::iterator i=lengthExpr->begin(); i!=lengthExpr->end(); i++) {
        delete *i;
    }
    delete lengthExpr;
}


/** Assignment operator */
SyntaxVariableDecl& SyntaxVariableDecl::operator=(const SyntaxVariableDecl& x) {
    
    if ( this != &x ) {
        
        lengthExpr->clear();

        elementTypeName = x.elementTypeName;
        referenceSymbol = x.referenceSymbol;
        variableName    = x.variableName;
        
        for ( std::list<SyntaxElement*>::const_iterator i=x.lengthExpr->begin(); i!=x.lengthExpr->end(); i++ )
            lengthExpr->push_back( (*i)->clone() );
    }
    
    return (*this);
}



/** Clone syntax element */
SyntaxVariableDecl* SyntaxVariableDecl::clone() const {
    
    return new SyntaxVariableDecl( *this );
}


/** Get semantic value: insert symbol and return the rhs value of the assignment */
RbPtr<Variable> SyntaxVariableDecl::evaluateContent( Environment& env ) {
    
#ifdef DEBUG_PARSER
    printf( "Evaluating variable declaration\n" );
#endif
    
    // Check if variable exists
    if ( env.existsVariable( variableName ) )
        throw RbException( "Illegal attempt to redefine variable " + variableName );
    
    // Check if type exists
    if ( !Workspace::userWorkspace().existsType( TypeSpec(elementTypeName) ) )
        throw RbException( "Type " + elementTypeName + " does not exist" );

    // Evaluate length specification
    std::vector<int> length;
    for ( std::list<SyntaxElement*>::iterator i=lengthExpr->begin(); i!=lengthExpr->end(); i++ ) {
        
        if ( (*i) == NULL ) {

            length.push_back( -1 );
        }
        else {
            
            RbPtr<Variable>         temp    = (*i)->evaluateContent( env );
            const RbLanguageObject& value   = temp->getValue();
            
            if ( value.isTypeSpec( Integer::getClassTypeSpec() ) )
                length.push_back( static_cast<const Integer&>( value ).getValue() );
            else
                throw RbException( "Expression in length specification of variable declaration does not evaluate to an integer" );
        }
    }
    
    // Make type specification
    TypeSpec typeSpec( elementTypeName );

    // Check if we have some positive lengths
    bool positiveLengthSpec = false;
    std::vector<size_t> len;
    for ( std::vector<int>::iterator i=length.begin(); i!=length.end(); i++ ) {
        if ( (*i) >= 1 )
            positiveLengthSpec = true;
    }

    // Create and add variable
    if ( positiveLengthSpec ) {

        std::vector<size_t> containerLength;
        for ( std::vector<int>::iterator i=length.begin(); i!=length.end(); i++ ) {
            if ( (*i) >= 1 )
                containerLength.push_back( size_t( *i ) );
            else
                containerLength.push_back( 1 );
        }
        
    }
    else {

        // Create new slot in frame with null variable
        env.addVariable( variableName );
    }
    
    return NULL;
}


/** Print info about the syntax element */
void SyntaxVariableDecl::printValue(std::ostream& o) const {
    
    o << "SyntaxVariableDecl: " << elementTypeName;
    for ( std::list<SyntaxElement*>::const_iterator i=lengthExpr->begin(); i!=lengthExpr->end(); i++ ) {
        if ( (*i) == NULL )
            o << "[]";
        else
            o << "[<expr>]";
    }
    
    o << referenceSymbol;
    
}


/**
 * Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
 * We just delegate that to the elements.
 */
void SyntaxVariableDecl::replaceVariableWithConstant(const std::string& name, const RbLanguageObject& c) {
    
    // the first set of statements
    for (std::list<SyntaxElement*>::iterator i = lengthExpr->begin(); i != lengthExpr->end(); i++) {
        (*i)->replaceVariableWithConstant(name, c);
    }
}

