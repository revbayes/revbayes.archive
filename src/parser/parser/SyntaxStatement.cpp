/**
 * @file
 * This file contains the implementation of SyntaxStatement, which is
 * used to hold control and loop statements in the syntax tree.
 *
 * @brief Implementation of SyntaxStatement
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "RbBoolean.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Environment.h"
#include "RbException.h"
#include "RbUtil.h"
#include "Signals.h"
#include "VectorString.h"
#include "SyntaxForCondition.h"
#include "SyntaxStatement.h"
#include "UserInterface.h"
#include "Workspace.h"

#include <cassert>
#include <iostream>
#include <list>
#include <sstream>


// Definition of the static type spec member
const TypeSpec SyntaxStatement::typeSpec(SyntaxStatement_name);

/** Static vector of strings giving names of statement types */
std::string SyntaxStatement::stmtName[] = { "IF", "IF_ELSE", "FOR", "WHILE", "NEXT", "BREAK", "RETURN" }; 


/** Construct from statement type */
SyntaxStatement::SyntaxStatement(statementT type)
    : SyntaxElement(), statementType(type), expression(NULL), statements1(NULL), statements2(NULL) {
}


/** Construct from statement type and expression (RETURN expression) */
SyntaxStatement::SyntaxStatement(statementT type, SyntaxElement* expr)
    : SyntaxElement(), statementType(type), expression(expr), statements1(NULL), statements2(NULL) {
}


/** Construct from statement type, condition and statement list */
SyntaxStatement::SyntaxStatement(statementT type, SyntaxElement* cond, std::list<SyntaxElement*>* stmts)
    : SyntaxElement(), statementType(type), expression(cond), statements1(stmts), statements2(NULL) {
}


/** Construct from statement type, condition and itwo statement lists */
SyntaxStatement::SyntaxStatement(statementT                 type,
                                 SyntaxElement*             cond,
                                 std::list<SyntaxElement*>* stmts1,
                                 std::list<SyntaxElement*>* stmts2)
    : SyntaxElement(), statementType(type), expression(cond), statements1(stmts1), statements2(stmts2) {
}


/** Deep copy constructor */
SyntaxStatement::SyntaxStatement(const SyntaxStatement& x)
    : SyntaxElement(x) {

    statementType   = x.statementType;
    expression      = x.expression->clone();

    for (std::list<SyntaxElement*>::iterator i=x.statements1->begin(); i!=x.statements1->end(); i++)
        statements1->push_back((*i)->clone());

    for (std::list<SyntaxElement*>::iterator i=x.statements2->begin(); i!=x.statements2->end(); i++)
        statements2->push_back((*i)->clone());
}


/** Destructor deletes expression and statements */
SyntaxStatement::~SyntaxStatement() {
    
    delete expression;

    if (statements1 != NULL) {
        for (std::list<SyntaxElement*>::iterator i=statements1->begin(); i!=statements1->end(); i++)
            delete (*i);
        delete statements1;
    }

    if (statements2 != NULL) {
        for (std::list<SyntaxElement*>::iterator i=statements2->begin(); i!=statements2->end(); i++)
            delete (*i);
        delete statements2;
    }
}


/** Assignment operator */
SyntaxStatement& SyntaxStatement::operator= (const SyntaxStatement& x) {

    if (&x != this) {
        delete expression;

        if (statements1 != NULL) {
            for (std::list<SyntaxElement*>::iterator i=statements1->begin(); i!=statements1->end(); i++)
                delete (*i);
            delete statements1;
        }

        if (statements2 != NULL) {
            for (std::list<SyntaxElement*>::iterator i=statements2->begin(); i!=statements2->end(); i++)
                delete (*i);
            delete statements2;
        }

        SyntaxElement::operator=(x);

        statementType   = x.statementType;
        expression      = x.expression->clone();

        for (std::list<SyntaxElement*>::iterator i=x.statements1->begin(); i!=x.statements1->end(); i++)
            statements1->push_back((*i)->clone());

        for (std::list<SyntaxElement*>::iterator i=x.statements2->begin(); i!=x.statements2->end(); i++)
            statements2->push_back((*i)->clone());
    }

    return (*this);
}


/** Return brief info about object */
std::string SyntaxStatement::briefInfo () const {

    std::ostringstream   o;
    o << "SyntaxStatement: type = " << stmtName[statementType];

    return o.str();
}


/** Clone syntax element */
SyntaxElement* SyntaxStatement::clone () const {

    return (SyntaxElement*)(new SyntaxStatement(*this));
}


/** Get class vector describing type of object */
const VectorString& SyntaxStatement::getClass(void) const { 
    
    static VectorString rbClass = VectorString(SyntaxStatement_name) + SyntaxElement::getClass();
	return rbClass; 
}



/** Get semantic value: it is here that we execute the statement */
Variable* SyntaxStatement::getContentAsVariable(Environment* env) const {

    Variable* result = NULL;
    
    if (statementType == For) {

        // Convert expression to for condition
        SyntaxForCondition* forCond = dynamic_cast<SyntaxForCondition*>(expression);
        assert (forCond != NULL);

        // Initialize for loop
        Signals::getSignals().clearFlags();
        
        // create a new environment for the loop
        // we need a new environment so that the elements will nt be visible from the outside
//        Environment *loopEnv = new Environment(env);
        Environment *loopEnv = env;
        
        forCond->initializeLoop(loopEnv);

        // Now loop over statements inside the for loop
        while (forCond->getNextLoopState(loopEnv)) {

            for (std::list<SyntaxElement*>::iterator i=statements1->begin(); i!=statements1->end(); i++) {

                // Execute statement
                result = (*i)->getContentAsVariable(loopEnv);
                
                // Print result if it is not an assign expression (==NULL)
                if (result != NULL) {
                    std::ostringstream msg;
                    result->getDagNodePtr()->printValue(msg);
                    RBOUT( msg.str() );
                }

                // Free memory
				if ( !Signals::getSignals().isSet( Signals::RETURN ) && result != NULL && result->isUnreferenced()) {
                    delete result;  // discard result
                    result = NULL;
                }

                // Catch signal
                if ( !Signals::getSignals().isGood() )
                    break;
            }

            // Catch signals
            if ( Signals::getSignals().isSet(Signals::BREAK) ) {
                forCond->finalizeLoop(loopEnv);   // We need to tell the for condition to finalize the loop
                Signals::getSignals().clearFlags();
                break;
            }
            else if ( Signals::getSignals().isSet(Signals::CONTINUE) ) {
                Signals::getSignals().clearFlags();  // Just continue with next loop state
            }
        }
        
        // destroy the loop environment
//        delete loopEnv;
    }
    else if (statementType == Break) {
    
        // Set BREAK signal
        Signals::getSignals().set(Signals::BREAK);
    }
    else if (statementType == Next) {
    
        // Set CONTINUE signal
        Signals::getSignals().set(Signals::CONTINUE);
    }
    else if ( statementType == While ) {

        // Loop over statements inside the while loop, first checking the expression
        while ( isTrue( expression, env ) ) {

            for ( std::list<SyntaxElement*>::iterator i=statements1->begin(); i!=statements1->end(); i++ ) {
	 
                // Execute statement
	            result = (*i)->getContentAsVariable( env );
                
                // Print result if it is not an assign expression (==NULL)
                if (result != NULL) {
                    std::ostringstream msg;
                    result->getDagNodePtr()->printValue(msg);
                    RBOUT( msg.str() );
                }
	 
	            // Free memory
                if ( !Signals::getSignals().isSet( Signals::RETURN ) && result != NULL && result->isUnreferenced() ){
                    delete result;  // discard result
                    result = NULL;
                }
	 
	            // Catch signal
	            if ( !Signals::getSignals().isGood() )
                    break;
            }

            // Catch signals
            if ( Signals::getSignals().isSet(Signals::BREAK) ) {
	                 Signals::getSignals().clearFlags();
	                 break;
            }
            else if ( Signals::getSignals().isSet(Signals::CONTINUE) ) {
	                 Signals::getSignals().clearFlags();  // Just continue with next loop state
            }
        }
    }
    else if (statementType == Return) {
        
        // Set RETURN signal and return expression value
        Signals::getSignals().set(Signals::RETURN);
        return expression->getContentAsVariable(env);
    }
    else if ( statementType == If ) {

        // Process statements inside the if clause if expression is true
        if ( isTrue( expression, env ) ) {
        	 	 
            for ( std::list<SyntaxElement*>::iterator i=statements1->begin(); i!=statements1->end(); i++ ) {

                // Execute statement
                result = (*i)->getContentAsVariable(env);
                
                // Print result if it is not an assign expression (==NULL)
                if (result != NULL) {
                    std::ostringstream msg;
                    result->getDagNodePtr()->printValue(msg);
                    RBOUT( msg.str() );
                }

                // Free memory
                if ( !Signals::getSignals().isSet( Signals::RETURN ) && result != NULL && result->isUnreferenced() ){
                    delete result;  // discard result
                    result = NULL;
                }
            }
        }
    }
    else if ( statementType == IfElse ) {
        
        // Process statements inside the if clause if expression is true,
        // otherwise process statements in else clause
        if ( isTrue( expression, env ) ) {
            
            for ( std::list<SyntaxElement*>::iterator i=statements1->begin(); i!=statements1->end(); i++ ) {
            
                // Execute statement
                result = (*i)->getContentAsVariable( env );
                
                // Print result if it is not an assign expression (==NULL)
                if (result != NULL) {
                    std::ostringstream msg;
                    result->getDagNodePtr()->printValue(msg);
                    RBOUT( msg.str() );
                }
                
                // Free memory
                if ( !Signals::getSignals().isSet( Signals::RETURN ) && result != NULL && result->isUnreferenced() ){
                    delete result;  // discard result
                    result = NULL;
                }
            }
        }
        else {	
            for ( std::list<SyntaxElement*>::iterator i=statements2->begin(); i!=statements2->end(); i++ ) {
    
                // Execute statement
                result = (*i)->getContentAsVariable( env );
                
                // Print result if it is not an assign expression (==NULL)
                if (result != NULL) {
                    std::ostringstream msg;
                    result->getDagNodePtr()->printValue(msg);
                    RBOUT( msg.str() );
                }
                    
                // Free memory
                if ( !Signals::getSignals().isSet( Signals::RETURN ) && result != NULL && result->isUnreferenced() ) {
                    delete result;  // discard result
                    result = NULL;
                }
            }
        }
    }

    return result;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& SyntaxStatement::getTypeSpec(void) const {
    return typeSpec;
}


/**
 * This is a help function that evaluates the expression and then checks
 * whether the result is true or false, or can be interpreted as a RbBoolean
 * true or false value.
 */
bool SyntaxStatement::isTrue( SyntaxElement* expression, Environment* env ) const {
    
    Variable* temp = expression->getContentAsVariable( env );
    
    if ( temp == NULL )
        return false;
    
    if ( temp->getValue()->isTypeSpec( TypeSpec(RbBoolean_name) ) ) {
        
        bool retValue = static_cast<const RbBoolean*>( temp->getValue() )->getValue();
        
        if ( temp->isUnreferenced() )
            delete   temp;
        
        return retValue;
    }
    else {
        
        RbBoolean* tempBool = static_cast<RbBoolean*>( temp->getValue()->convertTo( RbBoolean_name ) );
        bool     retValue = tempBool->getValue();
        
        delete   tempBool;
        if ( temp->isUnreferenced() )
            delete   temp;
        
        return   retValue;
    }
}


/** Print info about the syntax element */
void SyntaxStatement::print(std::ostream& o) const {

    o << "SyntaxStatement:" << std::endl;
    o << "statementType = " << stmtName[statementType] << std::endl;
    if (expression == NULL)
        o << "expression    = NULL" << std::endl;
    else
        o << "expression    = [" << expression << "] " << expression->briefInfo() << std::endl;
    if (statements1 == NULL)
        o << "statements1   = NULL" << std::endl;
    else {
        o << "statements1   = <" << statements1->size() << " statements>" << std::endl;
        int count=1;
        for (std::list<SyntaxElement*>::const_iterator i=statements1->begin(); i!=statements1->end(); i++, count++) {
            o << "   stmt " << count << " = [" << (*i) << "] " << (*i)->briefInfo() << std::endl;
        }
    }
    if (statements2 == NULL)
        o << "statements2   = NULL" << std::endl;
    else {
        o << "statements2   = <" << statements2->size() << " statements>" << std::endl;
        int count=1;
        for (std::list<SyntaxElement*>::const_iterator i=statements2->begin(); i!=statements2->end(); i++, count++) {
            o << "   stmt " << count << " = [" << (*i) << "] " << (*i)->briefInfo() << std::endl;
        }
    }
    o << std::endl;

    if (expression != NULL)
        expression->print(o);
    if (statements1 != NULL) {
        for (std::list<SyntaxElement*>::iterator i=statements1->begin(); i!=statements1->end(); i++) {
            (*i)->print(o);
        }
    }
    if (statements2 != NULL) {
        for (std::list<SyntaxElement*>::iterator i=statements2->begin(); i!=statements2->end(); i++) {
            (*i)->print(o);
        }
    }
}

