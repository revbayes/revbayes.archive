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
#include "VectorString.h"
#include "Workspace.h"

#include <iostream>
#include <list>
#include <sstream>


/** Construct from operator type, variable and expression */
SyntaxVariableDecl::SyntaxVariableDecl( RbString* typeName, std::list<SyntaxElement*>* lengths, RbString* referenceChar, RbString* varName) 
    : SyntaxElement(), elementTypeName(typeName), lengthExpr(lengths), referenceSymbol(referenceChar), variableName(varName) {
}


/** Deep copy constructor */
SyntaxVariableDecl::SyntaxVariableDecl(const SyntaxVariableDecl& x)
: SyntaxElement(x) {
    
    elementTypeName = x.elementTypeName->clone();
    referenceSymbol = x.referenceSymbol->clone();
    variableName    = x.variableName->clone();
    
    lengthExpr = new std::list<SyntaxElement*>();
    
    for ( std::list<SyntaxElement*>::iterator i=x.lengthExpr->begin(); i!=x.lengthExpr->end(); i++ )
        lengthExpr->push_back( (*i)->clone() );
}


/** Destructor deletes operands */
SyntaxVariableDecl::~SyntaxVariableDecl() {
    
    delete elementTypeName;
    delete referenceSymbol;
    delete variableName;

    for ( std::list<SyntaxElement*>::iterator i=lengthExpr->begin(); i!=lengthExpr->end(); i++ )
        delete (*i);
    delete lengthExpr;
}


/** Assignment operator */
SyntaxVariableDecl& SyntaxVariableDecl::operator=(const SyntaxVariableDecl& x) {
    
    if ( this != &x ) {
        
        delete elementTypeName;
        delete referenceSymbol;
        delete variableName;
        
        for ( std::list<SyntaxElement*>::iterator i=lengthExpr->begin(); i!=lengthExpr->end(); i++ )
            delete (*i);
        lengthExpr->clear();

        elementTypeName = x.elementTypeName->clone();
        referenceSymbol = x.referenceSymbol->clone();
        variableName    = x.variableName->clone();
        
        for ( std::list<SyntaxElement*>::iterator i=x.lengthExpr->begin(); i!=x.lengthExpr->end(); i++ )
            lengthExpr->push_back( (*i)->clone() );
    }
    
    return (*this);
}


/** Return brief info about object */
std::string SyntaxVariableDecl::briefInfo() const {
    
    std::ostringstream   o;
    o << "SyntaxVariableDecl: " << *elementTypeName;
    for ( std::list<SyntaxElement*>::iterator i=lengthExpr->begin(); i!=lengthExpr->end(); i++ ) {
        if ( (*i) == NULL )
            o << "[]";
        else
            o << "[<expr>]";
    }

    o << *referenceSymbol;
    
    return o.str();
}


/** Clone syntax element */
SyntaxVariableDecl* SyntaxVariableDecl::clone() const {
    
    return new SyntaxVariableDecl( *this );
}


/** Get class vector describing type of object */
const VectorString& SyntaxVariableDecl::getClass( void ) const { 
    
    static VectorString rbClass = VectorString( SyntaxVariableDecl_name ) + SyntaxElement::getClass();
	return rbClass; 
}


/** Get semantic value: insert symbol and return the rhs value of the assignment */
Variable* SyntaxVariableDecl::getContentAsVariable( Environment* env ) const {
    
    PRINTF( "Evaluating variable declaration\n" );
    
    // Check if variable exists
    if ( env->existsVariable( *variableName ) )
        throw RbException( "Illegal attempt to redefine variable " + *variableName );
    
    // Check if type exists
    if ( !Workspace::userWorkspace().existsType( *elementTypeName ) )
        throw RbException( "Type " + *elementTypeName + " does not exist" );

    // Evaluate length specification
    std::vector<int> length;
    for ( std::list<SyntaxElement*>::iterator i=lengthExpr->begin(); i!=lengthExpr->end(); i++ ) {
        
        if ( (*i) == NULL ) {

            length.push_back( -1 );
        }
        else {
            
            DAGNode*        temp    = (*i)->getContentAsVariable( env )->getDagNodePtr();
            const RbObject* value   = temp->getValue();
            
            if ( value->isType( Integer_name ) )
                length.push_back( static_cast<const Integer*>( value )->getValue() );
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
        env->addVariable( *variableName, typeSpec );
    }
    
    return NULL;
}


/** Print info about the syntax element */
void SyntaxVariableDecl::print(std::ostream& o) const {
    
    o << "SyntaxVariableDecl: " << briefInfo() << std::endl;
}

