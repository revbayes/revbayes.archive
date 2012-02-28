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
#include "RbString.h"
#include "SyntaxVariableDecl.h"
#include "Workspace.h"

#include <iostream>
#include <list>
#include <sstream>


/** Construct from operator type, variable and expression */
SyntaxVariableDecl::SyntaxVariableDecl( RbString* typeName, std::list<SyntaxElement*>* lengths, RbString* referenceChar, RbString* varName) 
    : SyntaxElement(), elementTypeName(typeName), lengthExpr(lengths), referenceSymbol(referenceChar), variableName(varName) {
}


/** Deep copy constructor */
SyntaxVariableDecl::SyntaxVariableDecl(const SyntaxVariableDecl& x) : SyntaxElement(x) {
    
    elementTypeName = x.elementTypeName->clone();
    referenceSymbol = x.referenceSymbol->clone();
    variableName    = x.variableName->clone();
    
    lengthExpr = new std::list<SyntaxElement*>();
    
    for ( std::list<SyntaxElement*>::const_iterator i=x.lengthExpr->begin(); i!=x.lengthExpr->end(); i++ )
        lengthExpr->push_back( (*i)->clone() );
}


/** Destructor deletes operands */
SyntaxVariableDecl::~SyntaxVariableDecl() {
    
    delete elementTypeName;
    delete referenceSymbol;
    delete variableName;
    
    for (std::list<SyntaxElement*>::iterator i=lengthExpr->begin(); i!=lengthExpr->end(); i++) {
        delete *i;
    }
    delete lengthExpr;
}


/** Assignment operator */
SyntaxVariableDecl& SyntaxVariableDecl::operator=(const SyntaxVariableDecl& x) {
    
    if ( this != &x ) {
        
        lengthExpr->clear();

        elementTypeName = x.elementTypeName->clone();
        referenceSymbol = x.referenceSymbol->clone();
        variableName    = x.variableName->clone();
        
        for ( std::list<SyntaxElement*>::const_iterator i=x.lengthExpr->begin(); i!=x.lengthExpr->end(); i++ )
            lengthExpr->push_back( (*i)->clone() );
    }
    
    return (*this);
}



/** Clone syntax element */
SyntaxVariableDecl* SyntaxVariableDecl::clone() const {
    
    return new SyntaxVariableDecl( *this );
}


/** Get class name of object */
const std::string& SyntaxVariableDecl::getClassName(void) { 
    
    static std::string rbClassName = "Variable declaration";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& SyntaxVariableDecl::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( SyntaxElement::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& SyntaxVariableDecl::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get semantic value: insert symbol and return the rhs value of the assignment */
RbVariablePtr SyntaxVariableDecl::evaluateContent( Environment& env ) {
    
    PRINTF( "Evaluating variable declaration\n" );
    
    // Check if variable exists
    if ( env.existsVariable( *variableName ) )
        throw RbException( "Illegal attempt to redefine variable " + *variableName );
    
    // Check if type exists
    if ( !Workspace::userWorkspace().existsType( TypeSpec(elementTypeName->getValue()) ) )
        throw RbException( "Type " + *elementTypeName + " does not exist" );

    // Evaluate length specification
    std::vector<int> length;
    for ( std::list<SyntaxElement*>::iterator i=lengthExpr->begin(); i!=lengthExpr->end(); i++ ) {
        
        if ( (*i) == NULL ) {

            length.push_back( -1 );
        }
        else {
            
            DAGNode*            temp    = (*i)->evaluateContent( env )->getDagNode();
            const RbLanguageObject&   value   = temp->getValue();
            
            if ( value.isTypeSpec( Integer::getClassTypeSpec() ) )
                length.push_back( static_cast<const Integer&>( value ).getValue() );
            else
                throw RbException( "Expression in length specification of variable declaration does not evaluate to an integer" );
        }
    }
    
    // Make type specification
    TypeSpec typeSpec( *elementTypeName );

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
        env.addVariable( *variableName );
    }
    
    return RbVariablePtr( NULL );
}


/** Print info about the syntax element */
void SyntaxVariableDecl::printValue(std::ostream& o) const {
    
    o << "SyntaxVariableDecl: " << *elementTypeName;
    for ( std::list<SyntaxElement*>::const_iterator i=lengthExpr->begin(); i!=lengthExpr->end(); i++ ) {
        if ( (*i) == NULL )
            o << "[]";
        else
            o << "[<expr>]";
    }
    
    o << *referenceSymbol;
    
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

