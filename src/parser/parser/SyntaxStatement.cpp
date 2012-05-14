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
#include "RbNullObject.h"
#include "RbUtil.h"
#include "Signals.h"
#include "SyntaxAssignExpr.h"
#include "SyntaxForCondition.h"
#include "SyntaxStatement.h"
#include "UserInterface.h"
#include "Workspace.h"

#include <cassert>
#include <iostream>
#include <list>
#include <sstream>


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
SyntaxStatement::SyntaxStatement(statementT                   type,
                                 SyntaxElement*               cond,
                                 std::list<SyntaxElement*>*   stmts1,
                                 std::list<SyntaxElement*>*   stmts2)
    : SyntaxElement(), statementType(type), expression(cond), statements1(stmts1), statements2(stmts2) {
}


/** Deep copy constructor */
SyntaxStatement::SyntaxStatement(const SyntaxStatement& x) : SyntaxElement(x) {

    statementType   = x.statementType;
    expression      = x.expression->clone();

    statements1 = new std::list<SyntaxElement*>();
    if ( x.statements1 != NULL ) {
        for (std::list<SyntaxElement*>::const_iterator i=x.statements1->begin(); i!=x.statements1->end(); i++)
            statements1->push_back( (*i)->clone() );
    }
    
    statements2 = new std::list<SyntaxElement*>();
    if ( x.statements2 != NULL ) {
        for (std::list<SyntaxElement*>::const_iterator i=x.statements2->begin(); i!=x.statements2->end(); i++)
            statements2->push_back( (*i)->clone() );
    }
}


/** Destructor deletes expression and statements */
SyntaxStatement::~SyntaxStatement() {
    
    delete expression;
    
    if (statements1 != NULL) {
        for (std::list<SyntaxElement*>::iterator i=statements1->begin(); i!=statements1->end(); i++) {
            delete *i;
        }
        delete statements1;
    }
    
    if (statements2 != NULL) {
        for (std::list<SyntaxElement*>::iterator i=statements2->begin(); i!=statements2->end(); i++) {
            delete *i;
        }
        delete statements2;
    }
}


/** Assignment operator */
SyntaxStatement& SyntaxStatement::operator= (const SyntaxStatement& x) {

    if (&x != this) {

        SyntaxElement::operator=(x);

        statementType   = x.statementType;
        expression      = x.expression->clone();
        
        for (std::list<SyntaxElement*>::iterator i = statements1->begin(); i != statements1->end(); i++) {
            SyntaxElement* theSyntaxElement = *i;
            delete theSyntaxElement;
        }
        statements1->clear();
        for (std::list<SyntaxElement*>::const_iterator i=x.statements1->begin(); i!=x.statements1->end(); i++)
            statements1->push_back( (*i)->clone() );
        
        for (std::list<SyntaxElement*>::iterator i = statements2->begin(); i != statements2->end(); i++) {
            SyntaxElement* theSyntaxElement = *i;
            delete theSyntaxElement;
        }
        statements2->clear();
        for (std::list<SyntaxElement*>::const_iterator i=x.statements2->begin(); i!=x.statements2->end(); i++)
            statements2->push_back( (*i)->clone() );
    }

    return (*this);
}


/** Clone syntax element */
SyntaxStatement* SyntaxStatement::clone () const {

    return new SyntaxStatement(*this);
}


/** Get class name of object */
const std::string& SyntaxStatement::getClassName(void) { 
    
    static std::string rbClassName = "Statement";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& SyntaxStatement::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( SyntaxElement::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& SyntaxStatement::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get semantic value: it is here that we execute the statement */
RbPtr<Variable> SyntaxStatement::evaluateContent(Environment& env) {

    RbPtr<Variable> result = NULL;
    
    if (statementType == For) {

        // Convert expression to for condition
        SyntaxForCondition* forCond = dynamic_cast<SyntaxForCondition*>( expression );
        assert (forCond != NULL);

        // Initialize for loop
        Signals::getSignals().clearFlags();
        
        // create a new environment for the loop
        // we need a new environment so that the elements will not be visible from the outside
//        Environment *loopEnv = new Environment(env);
        Environment& loopEnv = env;
        
        // here we initialize the loop
        forCond->initializeLoop(loopEnv);

        // Now loop over statements inside the for loop
        while ( forCond->isFinished() ) {
            
            RbLanguageObject& indexValue = forCond->getNextLoopState();
            for (std::list<SyntaxElement*>::iterator i=statements1->begin(); i!=statements1->end(); i++) {

                SyntaxElement* theSyntaxElement = *i;
                // replace the index variable first with its new value
                theSyntaxElement->replaceVariableWithConstant(forCond->getIndexVarName().getValue(), indexValue);
                // Execute statement
                result = theSyntaxElement->evaluateContent(loopEnv);
                
                // Print result if it is not an assign expression (==NULL)
                if ( !Signals::getSignals().isSet( Signals::RETURN ) && result != NULL && !(*i)->isTypeSpec(SyntaxAssignExpr::getClassTypeSpec()) && result->getValue() != NULL ) {
                    std::ostringstream msg;
                    result->getDagNode()->printValue(msg);
                    RBOUT( msg.str() );
                }

                // Free memory
				if ( !Signals::getSignals().isSet( Signals::RETURN ) && result != NULL) {
                    result = NULL;  // discard result
                }

                // Catch signal
                if ( !Signals::getSignals().isGood() )
                    break;
            }

            // Catch signals
            if ( Signals::getSignals().isSet(Signals::BREAK) ) {
                forCond->finalizeLoop();   // We need to tell the for condition to finalize the loop
                Signals::getSignals().clearFlags();
                break;
            }
            else if ( Signals::getSignals().isSet(Signals::CONTINUE) ) {
                Signals::getSignals().clearFlags();  // Just continue with next loop state
            }
        }
        
        // finalize the loop
        forCond->finalizeLoop();
        
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
	            result = (*i)->evaluateContent( env );
                
                // Print result if it is not an assign expression (==NULL)
                if ( !Signals::getSignals().isSet( Signals::RETURN ) && result != NULL && !(*i)->isTypeSpec(SyntaxAssignExpr::getClassTypeSpec()) && result->getValue() != NULL ) {
                    std::ostringstream msg;
                    result->getDagNode()->printValue(msg);
                    RBOUT( msg.str() );
                }
	 
	            // Free memory
                if ( !Signals::getSignals().isSet( Signals::RETURN ) && result != NULL ){
                    result = NULL;  // discard result
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
        return expression->evaluateContent(env);
    }
    else if ( statementType == If ) {

        // Process statements inside the if clause if expression is true
        if ( isTrue( expression, env ) ) {
        	 	 
            for ( std::list<SyntaxElement*>::iterator i=statements1->begin(); i!=statements1->end(); i++ ) {

                // Execute statement
                result = (*i)->evaluateContent(env);
                
                // Print result if it is not an assign expression (==NULL)
                if ( !Signals::getSignals().isSet( Signals::RETURN ) && result != NULL && !(*i)->isTypeSpec(SyntaxAssignExpr::getClassTypeSpec()) && result->getValue() != NULL ) {
                    std::ostringstream msg;
                    result->getDagNode()->printValue(msg);
                    RBOUT( msg.str() );
                }

                // Free memory
                if ( !Signals::getSignals().isSet( Signals::RETURN ) && result != NULL){
                    result = NULL;  // discard result
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
                result = (*i)->evaluateContent( env );
                
                // Print result if it is not an assign expression (==NULL)
                if ( !Signals::getSignals().isSet( Signals::RETURN ) && result != NULL && result->getValue() != NULL && !(*i)->isTypeSpec(SyntaxAssignExpr::getClassTypeSpec()) ) {
                    std::ostringstream msg;
                    result->getDagNode()->printValue(msg);
                    RBOUT( msg.str() );
                }
                
                // Free memory
                if ( !Signals::getSignals().isSet( Signals::RETURN ) && result != NULL ){
                    result = NULL;  // discard result
                }
            }
        }
        else {	
            for ( std::list<SyntaxElement*>::iterator i=statements2->begin(); i!=statements2->end(); i++ ) {
    
                // Execute statement
                result = (*i)->evaluateContent( env );
                
                // Print result if it is not an assign expression (==NULL)
                if ( !Signals::getSignals().isSet( Signals::RETURN ) && result != NULL && !(*i)->isTypeSpec(SyntaxAssignExpr::getClassTypeSpec()) && result->getValue() != NULL ) {
                    std::ostringstream msg;
                    result->getDagNode()->printValue(msg);
                    RBOUT( msg.str() );
                }
                    
                // Free memory
                if ( !Signals::getSignals().isSet( Signals::RETURN ) && result != NULL ) {
                    result = NULL;  // discard result
                }
            }
        }
    }

    return result;
}


/**
 * This is a help function that evaluates the expression and then checks
 * whether the result is true or false, or can be interpreted as a RbBoolean
 * true or false value.
 */
bool SyntaxStatement::isTrue( SyntaxElement* expression, Environment& env ) const {
    
    RbPtr<Variable> temp = expression->evaluateContent( env );
    
    if ( temp == NULL )
        return false;
    
    if ( temp->getValue()->isTypeSpec( RbBoolean::getClassTypeSpec() ) ) {
        
        bool retValue = static_cast<const RbBoolean&>( *temp->getValue() ).getValue();
        
        return retValue;
    }
    else {
        
        RbBoolean* tempBool = static_cast<RbBoolean*>( temp->getValue()->convertTo( RbBoolean::getClassTypeSpec() ) );
        bool     retValue = tempBool->getValue();
        delete tempBool;
        
        return   retValue;
    }
}


/** Print info about the syntax element */
void SyntaxStatement::printValue(std::ostream& o) const {

    o << "SyntaxStatement:" << std::endl;
    o << "statementType = " << stmtName[statementType] << std::endl;
    if (expression == NULL)
        o << "expression    = NULL" << std::endl;
    else {
        o << "expression    = [" << expression << "] ";
        expression->printValue(o);
        o << std::endl;
    }
    if (statements1 == NULL)
        o << "statements1   = NULL" << std::endl;
    else {
        o << "statements1   = <" << statements1->size() << " statements>" << std::endl;
        int count=1;
        for (std::list<SyntaxElement*>::const_iterator i=statements1->begin(); i!=statements1->end(); i++, count++) {
            o << "   stmt " << count << " = [" << (*i) << "] ";
            (*i)->printValue(o);
            o << std::endl;
        }
    }
    if (statements2 == NULL)
        o << "statements2   = NULL" << std::endl;
    else {
        o << "statements2   = <" << statements2->size() << " statements>" << std::endl;
        int count=1;
        for (std::list<SyntaxElement*>::const_iterator i=statements2->begin(); i!=statements2->end(); i++, count++) {
            o << "   stmt " << count << " = [" << (*i) << "] ";
            (*i)->printValue(o);
            o << std::endl;
        }
    }
    o << std::endl;

    if (expression != NULL)
        expression->printValue(o);
    if (statements1 != NULL) {
        for (std::list<SyntaxElement*>::const_iterator i=statements1->begin(); i!=statements1->end(); i++) {
            (*i)->printValue(o);
        }
    }
    if (statements2 != NULL) {
        for (std::list<SyntaxElement*>::const_iterator i=statements2->begin(); i!=statements2->end(); i++) {
            (*i)->printValue(o);
        }
    }
}


/**
 * Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
 * We just delegate that to the elements.
 */
void SyntaxStatement::replaceVariableWithConstant(const std::string& name, const RbLanguageObject& c) {
    
    // the first set of statements
    if ( statements1 != NULL ) {
        for (std::list<SyntaxElement*>::iterator i = statements1->begin(); i != statements1->end(); i++) {
            (*i)->replaceVariableWithConstant(name, c);
        }
    }
    
    // the second set of statements
    if ( statements2 != NULL ) {
        for (std::list<SyntaxElement*>::iterator i = statements2->begin(); i != statements2->end(); i++) {
            (*i)->replaceVariableWithConstant(name, c);
        }
    }
    
    // the expression itself
    if ( expression != NULL ) {
        expression->replaceVariableWithConstant(name, c);
    }
    
}

