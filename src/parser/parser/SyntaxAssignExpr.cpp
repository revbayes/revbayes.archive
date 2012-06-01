/**
 * @file
 * This file contains the implementation of SyntaxAssignExpr, which is
 * used to hold binary expressions in the syntax tree.
 *
 * @brief Implementation of SyntaxAssignExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "ConstArgument.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Func_reference.h"
#include "ParserDistribution.h"
#include "Plate.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "SyntaxFunctionCall.h"
#include "StochasticNode.h"
#include "VariableSlot.h"
#include "SyntaxAssignExpr.h"
#include "Workspace.h"
#include "ConstructorFunction.h"

#include <iostream>
#include <list>
#include <sstream>


/** Static vector of strings giving names of operator types */
std::string SyntaxAssignExpr::opCode[] = { "ARROW_ASSIGN", "TILDE_ASSIGN", "TILDIID_ASSIGN", "EQUATION_ASSIGN" };


/** Construct from operator type, variable and expression */
SyntaxAssignExpr::SyntaxAssignExpr(SyntaxAssignExpr::operatorT op, SyntaxVariable* var, SyntaxElement* expr) 
: SyntaxElement(), variable(var), functionCall(NULL), expression(expr), opType(op) {
}


/** Construct from operator type, function call and expression */
SyntaxAssignExpr::SyntaxAssignExpr(SyntaxAssignExpr::operatorT op, SyntaxFunctionCall* fxnCall, SyntaxElement* expr) : SyntaxElement(), variable(NULL), functionCall(fxnCall), expression(expr), opType(op) {
}

/** Construct from operator type, variable and expression */
SyntaxAssignExpr::SyntaxAssignExpr(SyntaxAssignExpr::operatorT op, SyntaxVariable* var, SyntaxFunctionCall* expr)  : SyntaxElement(), variable(var), functionCall(NULL), expression( expr ), opType(op) {
}


/** Construct from operator type, function call and expression */
SyntaxAssignExpr::SyntaxAssignExpr(SyntaxAssignExpr::operatorT op, SyntaxFunctionCall* fxnCall, SyntaxFunctionCall* expr) : SyntaxElement(), variable(NULL), functionCall(fxnCall), expression( expr ), opType(op) {
}


/** Deep copy constructor */
SyntaxAssignExpr::SyntaxAssignExpr(const SyntaxAssignExpr& x) : SyntaxElement(x) {
    
    if ( x.variable != NULL )
        variable   = x.variable->clone();

    if ( x.functionCall != NULL )
        functionCall = x.functionCall->clone();

    expression = x.expression->clone();
    opType     = x.opType;
}


/** Destructor deletes operands */
SyntaxAssignExpr::~SyntaxAssignExpr() {
    
    delete variable;
    delete functionCall;
    delete expression;
    
}


/** Assignment operator */
SyntaxAssignExpr& SyntaxAssignExpr::operator=(const SyntaxAssignExpr& x) {
    
    if ( this != &x ) {
        
        functionCall = NULL;
        variable = NULL;
        
        if ( x.variable != NULL )
            variable   = x.variable;
        
        if ( x.functionCall != NULL )
            functionCall = x.functionCall;
        
        expression = x.expression;
        opType     = x.opType;
    }
    
    return (*this);
}


/** Clone syntax element */
SyntaxAssignExpr* SyntaxAssignExpr::clone () const {
    
    return new SyntaxAssignExpr(*this);
}


/** Get class name of object */
const std::string& SyntaxAssignExpr::getClassName(void) { 
    
    static std::string rbClassName = "Assignment";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& SyntaxAssignExpr::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( SyntaxElement::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& SyntaxAssignExpr::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get semantic value: insert symbol and return the rhs value of the assignment */
RbPtr<Variable> SyntaxAssignExpr::evaluateContent( Environment& env ) {
    
    PRINTF( "Evaluating assign expression\n" );
    
    // Get variable info from lhs
    const RbPtr<Variable>& theSlot = variable->createVariable( env );
    
    // Declare variable storing the return value of the assignment expression
    RbPtr<Variable> theVariable = NULL;
    
    // Deal with arrow assignments
    if ( opType == ArrowAssign ) {
        
        PRINTF("Arrow assignment\n");
        
        // Calculate the value of the rhs expression
        theVariable = expression->evaluateContent( env );
        if ( theVariable == NULL )
            throw RbException( "Invalid NULL variable returned by rhs expression in assignment" );
        
        // fill the slot with the new variable
        const RbPtr<DAGNode> &theConstNode = theVariable->getDagNode();
        const RlValue<RbLanguageObject>& value = theConstNode->getValue();
        
        
        DAGNode* theNode;
        // check if the type is valid. This is necessary for reassignments
        if ( !value.getTypeSpec().isDerivedOf( theSlot->getValueTypeSpec() ) ) {
            // We are not of a derived type (or the same type)
            // since this will create a constant node we are allowed to type cast
            if (value.isConvertibleTo( theSlot->getValueTypeSpec() ) ) {
                RlValue<RbObject> tmp = value.convertTo( theSlot->getValueTypeSpec() );
                
                std::vector<RbPtr<RbLanguageObject> > vals;
                for (std::vector<RbPtr<RbObject> >::iterator i = tmp.value.begin(); i != tmp.value.end(); ++i) {
                    vals.push_back( RbPtr<RbLanguageObject>( static_cast<RbLanguageObject *>( (RbObject *) *i ) ) );
                }
                
                RlValue<RbLanguageObject> convValue = RlValue<RbLanguageObject>(vals, tmp.lengths);
                theNode = new ConstantNode( convValue );
            }
            else {
                std::ostringstream msg;
                msg << "Cannot reassign variable '" << theSlot->getName() << "' with type " << value.getTypeSpec() << " with value ";
                value.printValue(msg);
                msg << " because the variable requires type " << theSlot->getValueTypeSpec() << "." << std::endl;
                throw RbException( msg );
            }
        }
        else {
            if (value.isTypeSpec( DAGNode::getClassTypeSpec() )) {
                theNode = static_cast<DAGNode*>( value.getSingleValue()->clone() );
            }
            else {
                RlValue<RbObject> tmp = value.clone();
                
                std::vector<RbPtr<RbLanguageObject> > vals;
                for (std::vector<RbPtr<RbObject> >::iterator i = tmp.value.begin(); i != tmp.value.end(); ++i) {
                    vals.push_back( RbPtr<RbLanguageObject>( static_cast<RbLanguageObject *>( (RbObject *) *i ) ) );
                }
                
                RlValue<RbLanguageObject> clonedValue = RlValue<RbLanguageObject>(vals, tmp.lengths);
                theNode = new ConstantNode( clonedValue );
            }
        }
        
        // set the DAG node of the variable
        theSlot->setDagNode( theNode );
        
        // set the name of the DAG node. This will ensure nicer outputs about the DAG.
        theNode->setName( theSlot->getName() );
    }
    
    // Deal with equation assignments
    else if ( opType == EquationAssign ) {
        
        PRINTF( "Equation assignment\n" );
        
        // Get DAG node representation of expression
        // We allow direct references without lookup nodes
        // We also allow constant expressions
        theVariable = expression->evaluateContent( env );
        PRINTF ( "Created %s with function \"%s\" and value %s \n", theVariable->getDagNode()->getType().c_str(), theVariable->getDagNode()->getValue().getTypeSpec().toString().c_str());
        
        // if the right-hand-side was a lookup to a variable (e.g. b := a)
        // we therefore create a new reference function which will lookup the value of the original node each time. Hence, the new node (left-hand-side) is just a reference of the original node (right-hand-side).
        SyntaxVariable* rhs = dynamic_cast<SyntaxVariable*>( expression );
        if ( !theVariable->getDagNode()->isTypeSpec( DeterministicNode::getClassTypeSpec() ) || rhs != NULL && !rhs->hasFunctionCall() ) {
            
            RbFunction* func = new Func_reference();
            std::vector<RbPtr<Argument> > args;
            args.push_back( new Argument( theVariable ) );
            func->processArguments(args);
            theVariable = RbPtr<Variable>( new Variable( new DeterministicNode( func, NULL ) ) );
        }
        
        const DeterministicNode* detNode = static_cast<const DeterministicNode*>( (DAGNode *) theVariable->getDagNode() );
        const RbFunction& theFunction = detNode->getFunction();
        // check if the type is valid. This is necessary for reassignments
        if ( !theFunction.getReturnType().isDerivedOf( theSlot->getValueTypeSpec() ) ) {
            std::ostringstream msg;
            msg << "Cannot reassign variable '" << theSlot->getName() << "' with a function with return type " << theFunction.getReturnType() << " because the variable requires type " << theSlot->getValueTypeSpec() << "." << std::endl;
            throw RbException( msg );
        }
        
        // fill the slot with the new variable
        theSlot->setDagNode( theVariable->getDagNode() );
        
        // set the name of the DAG node. This will ensure nicer outputs about the DAG.
        theVariable->getDagNode()->setName( theSlot->getName() );
    }
    
    // Deal with tilde assignments
    else if ( opType == TildeAssign ) {
        
        PRINTF( "Tilde assignment\n" );
        
        // get the rhs expression wrapped and executed into a variable
        theVariable = expression->evaluateContent(env);
        
        // Get distribution, which should be the return value of the rhs function
        const DAGNode* exprValue = theVariable->getDagNode();
        if ( exprValue == NULL ) {
            throw RbException( "Distribution function returns NULL" );
        }
        
        const DeterministicNode* detNode = dynamic_cast<const DeterministicNode*>( exprValue );
        if ( detNode == NULL || !detNode->getFunction().isTypeSpec( ConstructorFunction::getClassTypeSpec() ) ) {
            
            throw RbException( "Function does not return a distribution" );
        }
        
        // Make an independent copy of the distribution and delete the exprVal
//        Distribution* distribution = (Distribution*) detNode->getFunctionPtr()->execute();
        const ParserDistribution *dist = dynamic_cast<const ParserDistribution *>( (const RbLanguageObject *) detNode->getValue().getSingleValue() );
               
        // Create new stochastic node
        DAGNode* node = new StochasticNode( dist->clone(), NULL );
        
        // fill the slot with the new variable
        theSlot->setDagNode( node );
        
        // set the name of the DAG node. This will ensure nicer outputs about the DAG.
        node->setName( theSlot->getName() );
        
    }
    
    theSlot->getDagNode()->touch();
    theSlot->getDagNode()->keep();

    
#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif    

    return theVariable;
}



/** Print info about the syntax element */
void SyntaxAssignExpr::printValue(std::ostream& o) const {
    
    o << "SyntaxAssignExpr:" << std::endl;
    o << "variable      = ";
    variable->printValue(o);
    o << std::endl;
    o << "expression    = ";
    expression->printValue(o);
    o << std::endl;
    o << "operation     = " << opCode[opType];
}


/**
 * Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
 * We just delegate that to the element on our right-hand-side and also to the variable itself (lhs).
 */
void SyntaxAssignExpr::replaceVariableWithConstant(const std::string& name, const RbLanguageObject& c) {
    expression->replaceVariableWithConstant(name, c);
    variable->replaceVariableWithConstant(name, c);
}


