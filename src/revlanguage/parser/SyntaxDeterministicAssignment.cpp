/**
 * @file
 * This file contains the implementation of SyntaxAssignExpr, which is
 * used to hold binary expressions in the syntax tree.
 *
 * @brief Implementation of SyntaxAssignExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-09-07 12:47:31 +0200 (Fri, 07 Sep 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: SyntaxAssignExpr.cpp 1801 2012-09-07 10:47:31Z hoehna $
 */

#include "ArgumentRule.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "RlDistribution.h"
#include "SyntaxFunctionCall.h"
#include "VariableSlot.h"
#include "SyntaxDeterministicAssignment.h"
#include "Workspace.h"

#include <iostream>
#include <list>
#include <sstream>

using namespace RevLanguage;

/** Construct from operator type, variable and expression */
SyntaxDeterministicAssignment::SyntaxDeterministicAssignment(SyntaxVariable* var, SyntaxElement* expr) : SyntaxElement(), variable(var), functionCall(NULL), expression(expr) {

}


/** Construct from operator type, function call and expression */
SyntaxDeterministicAssignment::SyntaxDeterministicAssignment(SyntaxFunctionCall* fxnCall, SyntaxElement* expr) : SyntaxElement(), variable(NULL), functionCall(fxnCall), expression(expr) {
}


/** Deep copy constructor */
SyntaxDeterministicAssignment::SyntaxDeterministicAssignment(const SyntaxDeterministicAssignment& x) : SyntaxElement(x) {
    
    if ( x.variable != NULL )
        variable   = x.variable->clone();
    
    if ( x.functionCall != NULL )
        functionCall = x.functionCall->clone();
    
    expression = x.expression->clone();
}


/** Destructor deletes operands */
SyntaxDeterministicAssignment::~SyntaxDeterministicAssignment() {
    
    delete variable;
    delete functionCall;
    delete expression;
    
}


/** Assignment operator */
SyntaxDeterministicAssignment& SyntaxDeterministicAssignment::operator=(const SyntaxDeterministicAssignment& x) {
    
    if ( this != &x ) {
        
        functionCall = NULL;
        variable = NULL;
        
        if ( x.variable != NULL )
            variable   = x.variable;
        
        if ( x.functionCall != NULL )
            functionCall = x.functionCall;
        
        expression = x.expression;
    }
    
    return (*this);
}


/** Clone syntax element */
SyntaxDeterministicAssignment* SyntaxDeterministicAssignment::clone () const {
    
    return new SyntaxDeterministicAssignment(*this);
}


/** Get semantic value: insert symbol and return the rhs value of the assignment */
RbPtr<Variable> SyntaxDeterministicAssignment::evaluateContent( Environment& env ) {
    
#ifdef DEBUG_PARSER
    printf( "Evaluating assign expression\n" );
#endif
    
    // Get variable info from lhs
    const RbPtr<Variable>& theSlot = variable->createVariable( env );
    
    // Declare variable storing the return value of the assignment expression
    RbPtr<Variable> theVariable = NULL;
    
        
#ifdef DEBUG_PARSER
    printf( "Equation assignment\n" );
#endif
        
    // Get DAG node representation of expression
    // We allow direct references without lookup nodes
    // We also allow constant expressions
    theVariable = expression->evaluateContent( env );
#ifdef DEBUG_PARSER
    printf( "Created %s with function \"%s\" and value %s \n", theVariable->getValue().getType().c_str(), theVariable->getValue().getTypeSpec().toString().c_str());
#endif
        
    // if the right-hand-side was a lookup to a variable (e.g. b := a)
    // we therefore create a new reference function which will lookup the value of the original node each time. Hence, the new node (left-hand-side) is just a reference of the original node (right-hand-side).
    //        SyntaxVariable* rhs = dynamic_cast<SyntaxVariable*>( expression );
    //        if ( !theVariable->getDagNode()->isTypeSpec( DeterministicNode::getClassTypeSpec() ) || rhs != NULL && !rhs->hasFunctionCall() ) {
    //            
    //            Function* func = new Func_reference();
    //            std::vector<Argument> args;
    //            args.push_back( Argument( theVariable ) );
    //            func->processArguments(args);
    //            theVariable = RbPtr<Variable>( new Variable( new DeterministicNode( func, NULL ) ) );
    //        }
        
    const RbLanguageObject &value = theVariable->getValue();
    // check if the type is valid. This is necessary for reassignments
    //        if ( !theFunction.getReturnType().isDerivedOf( theSlot->getValueTypeSpec() ) ) {
    //            std::ostringstream msg;
    //            msg << "Cannot reassign variable '" << theSlot->getName() << "' with a function with return type " << theFunction.getReturnType() << " because the variable requires type " << theSlot->getValueTypeSpec() << "." << std::endl;
    //            throw RbException( msg );
    //        }
    
    // fill the slot with the new variable
    theSlot->setValue( value.clone() );
        
    // set the name of the DAG node. This will ensure nicer outputs about the DAG.
    theVariable->getValue().setName( theSlot->getName() );
    
    //    theSlot->getDagNode()->touch();
    //    theSlot->getDagNode()->keep();
    
    
#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif    
    
    return theVariable;
}


bool SyntaxDeterministicAssignment::isAssignment( void ) const {
    return true;
}



/** Print info about the syntax element */
void SyntaxDeterministicAssignment::printValue(std::ostream& o) const {
    
    o << "SyntaxDeterministicAssignment:" << std::endl;
    o << "variable      = ";
    variable->printValue(o);
    o << std::endl;
    o << "expression    = ";
    expression->printValue(o);
    o << std::endl;
}


/**
 * Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
 * We just delegate that to the element on our right-hand-side and also to the variable itself (lhs).
 */
void SyntaxDeterministicAssignment::replaceVariableWithConstant(const std::string& name, const RbLanguageObject& c) {
    expression->replaceVariableWithConstant(name, c);
    variable->replaceVariableWithConstant(name, c);
}


