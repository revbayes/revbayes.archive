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

#include "ConverterNode.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "ContainerNode.h"
#include "DeterministicNode.h"
#include "Distribution.h"
#include "MemberNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbOptions.h"
#include "SyntaxFunctionCall.h"
#include "StochasticNode.h"
#include "ValueContainer.h"
#include "VariableContainer.h"
#include "VectorIndex.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include "SyntaxAssignExpr.h"
#include "Workspace.h"

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
SyntaxAssignExpr::SyntaxAssignExpr(SyntaxAssignExpr::operatorT op, SyntaxFunctionCall* fxnCall, SyntaxElement* expr) 
: SyntaxElement(), variable(NULL), functionCall(fxnCall), expression(expr), opType(op) {
}


/** Deep copy constructor */
SyntaxAssignExpr::SyntaxAssignExpr(const SyntaxAssignExpr& x)
: SyntaxElement(x) {
    
    if ( x.variable != NULL )
        variable   = x.variable->clone();

    if ( x.functionCall != NULL )
        functionCall = x.functionCall->clone();

    expression = x.expression->clone();
    opType     = x.opType;
}


/** Destructor deletes operands */
SyntaxAssignExpr::~SyntaxAssignExpr() {
    
    delete functionCall;
    delete variable;
    delete expression;
}


/** Assignment operator */
SyntaxAssignExpr& SyntaxAssignExpr::operator=(const SyntaxAssignExpr& x) {
    
    if ( this != &x ) {
        
        delete functionCall;
        delete variable;
        delete expression;
        
        functionCall = NULL;
        variable = NULL;
        
        if ( x.variable != NULL )
            variable   = x.variable->clone();
        
        if ( x.functionCall != NULL )
            functionCall = x.functionCall->clone();
        
        expression = x.expression->clone();
        opType     = x.opType;
    }
    
    return (*this);
}


/** Return brief info about object */
std::string SyntaxAssignExpr::briefInfo () const {
    
    std::ostringstream   o;
    o << "SyntaxAssignExpr: operation = " << opCode[opType];
    
    return o.str();
}


/** Clone syntax element */
SyntaxAssignExpr* SyntaxAssignExpr::clone () const {
    
    return new SyntaxAssignExpr(*this);
}


/** Get class vector describing type of object */
const VectorString& SyntaxAssignExpr::getClass(void) const { 
    
    static VectorString rbClass = VectorString(SyntaxAssignExpr_name) + SyntaxElement::getClass();
	return rbClass; 
}


/** Convert element to DAG node: insert symbol and return reference to it */
DAGNode* SyntaxAssignExpr::getDAGNodeExpr(VariableFrame* frame) const {
    

    if ( variable != NULL ) {

        // Variable on lhs. Insert symbol; discard the return value
        DAGNode* retVal = getValue(frame);
        if (retVal->numRefs() == 0)
            delete retVal;
        
        return variable->getDAGNodeExpr(frame);
    }
    else {
        
        // Function call on lhs. Get reference, execute assignment, return reference.
        DAGNode* reference = functionCall->getValue( frame );
        
        if ( reference == NULL )
            throw RbException( "Null expression on left-hand side of assignment" );
        
        if ( reference->getSlot() == NULL ) {
            delete reference;
            throw RbException( "Temp variable expression on left-hand side of assignment" );
        }
        
        DAGNode* retVal = getValue(frame);
        if (retVal->numRefs() == 0)
            delete retVal;
        
        return reference;
    }
}


/** Get semantic value: insert symbol and return the rhs value of the assignment */
DAGNode* SyntaxAssignExpr::getValue( VariableFrame* frame ) const {
    
    PRINTF( "Evaluating assign expression\n" );
    
    // Get variable info from lhs
    VectorIndex         elemIndex;
    VariableSlot*       slot = NULL;
    DAGNode*            theVariable;
    RbString            varName;

    if ( variable != NULL ) {
        
        theVariable = variable->getLValue( frame, slot, elemIndex );
        
        if ( theVariable == NULL && slot != NULL )
            throw RbException( "Unexpected left-hand side value of assignment" );

        if ( variable->getIdentifier() != NULL )
            varName = *variable->getIdentifier();
    }
    else {
        
        theVariable = functionCall->getValue( frame );
        
        if ( theVariable == NULL )
            throw RbException( "Null expression on left-hand side of assignment" );
        
        if ( theVariable->getSlot() == NULL ) {
            delete theVariable;
            throw RbException( "Temp variable expression on left-hand side of assignment" );
        }
    }
    
    // Declare variable storing the return value of the assignment expression
    DAGNode* retValue;
    
    // Deal with arrow assignments
    if ( opType == ArrowAssign ) {
        
        PRINTF("Arrow assignment\n");
        
        // Calculate the value of the rhs expression
        retValue = expression->getValue( frame );
        if ( retValue == NULL )
            throw RbException( "Invalid NULL variable returned by rhs expression in assignment" );
        
        // Find the value we want to use for the assignment
        const RbObject* retValueObject = retValue->getValue();
        RbObject* value = NULL;
        if ( retValueObject != NULL )
            value = retValueObject->clone();

        if ( slot == NULL ) {
            
            // The variable does not exist - add it to current frame
            // We make the new slot as generous as possible (type RbObject)
            if ( elemIndex.size() > 0 && value->getDim() > 0 )
                throw RbException( "Invalid attempt to create container of containers" );
            
            PRINTF ( "Creating constant variable %s %s with a %s value %sthrough arrow assignment\n", TypeSpec( RbObject_name, elemIndex.size() ).toString().c_str(), varName.c_str(), value == NULL ? "NULL" : value->getTypeSpec().toString().c_str(), elemIndex.size() > 0 ? "element " : "" );
            TypeSpec typeSpec( RbObject_name, elemIndex.size() );
            if ( elemIndex.size() == 0 )
                frame->addVariable( varName, typeSpec, value->wrapIntoVariable() );
            else {
                frame->addVariable( varName, typeSpec, makeContainer( elemIndex, value ) );
            }
        }
        else if ( elemIndex.size() == 0 ) {
            
            // Assigning to an existing regular variable
            PRINTF ( "Assigning a value to %s %s through arrow assignment\n", theVariable->getTypeSpec().toString().c_str(), theVariable->getName().c_str() );
            slot->setValue( value );
        }
        else /* if ( elemIndex.size() > 0 ) */ {
            
            // Assigning to a container element or member object element
            PRINTF ( "Assigning a value to %s %s%s through arrow assignment\n", theVariable->getTypeSpec().toString().c_str(), theVariable->getName().c_str(), elemIndex.toIndexString().c_str() );
            slot->setElement( elemIndex, value );
        }
    }
    
    // Deal with equation assignments
    else if ( opType == EquationAssign ) {
        
        PRINTF( "Equation assignment\n" );
        
        // Get DAG node representation of expression
        // We allow direct references without lookup nodes
        // We also allow constant expressions
        DAGNode* node = expression->getDAGNodeExpr( frame );
        if ( node == NULL )
            node = new ConstantNode( RbObject_name );      // NULL object
        else if ( !node->isPermanent( frame ) ) {
            if ( !node->isDAGType( LookupNode_name ) && !node->isDAGType( FunctionNode_name ) )
                throw RbException( "Invalid equation assignment using temp variable" );
        }
        
        // Set return value of the assignment expression
        // We return a clone of the root DAG node itself
        retValue = node->clone();
        
        // Does the variable exist?
        if ( slot == NULL ) {
            
            // It does not exist - add it
            // We make the new slot as generous as possible (type RbObject)
            PRINTF ( "Creating variable %s %s with a dag expression %sthrough equation assignment\n", TypeSpec( node->getValueType(), elemIndex.size() ).toString().c_str(), varName.c_str(), elemIndex.size() > 0 ? "element " : "" );
            TypeSpec typeSpec( RbObject_name, elemIndex.size() );
            if ( elemIndex.size() == 0 )
                frame->addVariable( varName, typeSpec, node );
            else {
                frame->addVariable( varName, typeSpec, makeContainer( elemIndex, node ) );
            }
        }
        else if ( elemIndex.size() == 0 ) {
            
            // It exists - replace it
            PRINTF ( "Assigning a dag expression to %s %s through equation assignment\n", theVariable->getTypeSpec().toString().c_str(), theVariable->getName().c_str() );
            slot->setVariable( node );
        }
        else /* if ( elemIndex.size() > 0 ) */ {
            
            // Equation assignment to a container element or member object element
            PRINTF ( "Assigning a dag expression to %s %s%s through equation assignment\n", theVariable->getTypeSpec().toString().c_str(), theVariable->getName().c_str(), elemIndex.toIndexString().c_str() );
            slot->setElement( elemIndex, node );
        }
    }
    
    // Deal with tilde assignments
    else if ( opType == TildeAssign ) {
        
        PRINTF( "Tilde assignment\n" );
        
        // Get distribution, which should be the return value of the rhs function
        // The rhs could evaluate to a reference to a distribution (allowing several
        // stochastic nodes to share the same distribution)
        DAGNode* exprValue = expression->getValue( frame );
        if ( exprValue == NULL ) {
            throw RbException( "Distribution function returns NULL" );
        }
        MemberNode* dist = dynamic_cast<MemberNode*>( exprValue );
        if ( dist == NULL || dist->getValue() == NULL || !dist->getValue()->isType( Distribution_name ) ) {
            delete exprValue;
            throw RbException( "Function does not return a distribution" );
        }
        
        // Make an independent copy of the distribution and delete the exprVal
        Distribution* distribution = static_cast<Distribution*>( dist->getMemberObject()->clone() );
        delete exprValue;
        if ( distribution == NULL )
            throw RbException( "Function returns a NULL distribution" );
        
        // Create new stochastic node
        StochasticNode* node = new StochasticNode( distribution );
        
        // Set return value of the assignment expression
        // We return a clone of the stochastic node itself
        retValue = node->clone();
        
        // Does the variable exist?
        if ( slot == NULL ) {
            
            // It does not exist - add it
            // We make the new slot as generous as possible (type RbObject)
            PRINTF ( "Creating variable %s %s with a stochastic node %sthrough tilde assignment\n", TypeSpec( node->getValueType(), elemIndex.size() ).toString().c_str(), varName.c_str(), elemIndex.size() > 0 ? "element " : "" );
            TypeSpec typeSpec( RbObject_name, elemIndex.size() );
            if ( elemIndex.size() == 0 )
                frame->addVariable( varName, typeSpec, node );
            else {
                frame->addVariable( varName, typeSpec, makeContainer( elemIndex, node ) );
            }
        }
        else if ( elemIndex.size() == 0 ) {
            
            // It exists - replace it without type conversion
            PRINTF ( "Assigning a stochastic node of value type %s to %s %s through tilde assignment\n", node->getValueType().c_str(), theVariable->getTypeSpec().toString().c_str(), theVariable->getName().c_str() );
            slot->setVariable( node, false );
        }
        else /* if ( elemIndex.size() > 0 ) */ {
            
            // Tilde assignment to a container element or member object element without type conversion
            PRINTF ( "Assigning a stochastic node of value type %s to %s %s%s through tilde assignment\n", node->getValueType().c_str(), theVariable->getTypeSpec().toString().c_str(), theVariable->getName().c_str(), elemIndex.toIndexString().c_str() );
            slot->setElement( elemIndex, node, false );
        }
    }
    
    
    // Deal with tilde iid assignments
    else if ( opType == TildeIidAssign ) {
        
        PRINTF( "Tilde iid assignment\n" );
        
        // Get distribution, which should be the return value of the rhs function
        // The rhs could evaluate to a reference to a distribution (allowing several
        // stochastic nodes to share the same distribution)
        DAGNode* exprValue = expression->getValue( frame );
        if ( exprValue == NULL ) {
            throw RbException( "Distribution function returns NULL" );
        }
        MemberNode* dist = dynamic_cast<MemberNode*>( exprValue );
        if ( dist == NULL || dist->getValue() == NULL || !dist->getValue()->isType( Distribution_name ) ) {
            delete exprValue;
            throw RbException( "Function does not return a distribution" );
        }
        
        // Make an independent copy of the distribution and delete the exprVal
        Distribution* distribution = static_cast<Distribution*>( dist->getMemberObject()->clone() );
        delete exprValue;
        if ( distribution == NULL )
            throw RbException( "Function returns a NULL distribution" );
        
        delete distribution;
        throw RbException( "Support of ~iid not complete yet" );
    }

    return retValue;
}



/** Make new value container */
ContainerNode* SyntaxAssignExpr::makeContainer( const VectorIndex& index, RbObject* elem ) const {
    
    // Catch attempt to create containers of containers
    if ( elem->getDim() > 0 )
        throw RbException( "Illegal attempt to create container of containers" );
    
    // Check index and get length specification
    std::vector<size_t> length;
    VectorNatural       elementIndex;
    for ( size_t i = 0; i < index.size(); i++ ) {
        
        if ( !index[i]->isType( Integer_name ) )
            throw RbException( "Cannot make container based on string index" );
        
        int k = static_cast<const Integer*>( index[i] )->getValue();
        if ( k < 0 )
            throw RbException( "Cannot make container based on empty index" );
        
        elementIndex.push_back( k );
        length.push_back( k + 1 );
    }
    
    ValueContainer* temp = new ValueContainer( TypeSpec( elem->getType(), index.size() ), length );
    temp->setElement( elementIndex, elem );

    return new ContainerNode( temp );
}


/** Make new variable container */
ContainerNode* SyntaxAssignExpr::makeContainer( const VectorIndex& index, DAGNode* elem ) const {
    
    // Catch attempt to create containers of containers
    if ( elem->getDim() > 0 )
        throw RbException( "Illegal attempt to create container of containers" );
    
    // Check index and get length specification
    std::vector<size_t> length;
    VectorNatural       elementIndex;
    for ( size_t i = 0; i < index.size(); i++ ) {
        
        if ( !index[i]->isType( Integer_name ) )
            throw RbException( "Cannot make container based on string index" );
        
        int k = static_cast<const Integer*>( index[i] )->getValue();
        if ( k < 0 )
            throw RbException( "Cannot make container based on empty index" );
        
        elementIndex.push_back( k );
        length.push_back( k + 1 );
    }
    
    VariableContainer* temp = new VariableContainer( TypeSpec( elem->getValueType(), index.size() ), length );
    temp->setElement( elementIndex, elem );
    
    return new ContainerNode( temp );
}


/** Print info about the syntax element */
void SyntaxAssignExpr::print(std::ostream& o) const {
    
    o << "SyntaxAssignExpr:" << std::endl;
    o << "variable      = " << variable->briefInfo() << std::endl;
    o << "expression    = " << expression->briefInfo() << std::endl;
    o << "operation     = " << opCode[opType];
}

