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
#include "StochasticNode.h"
#include "ValueContainer.h"
#include "VariableContainer.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include "SyntaxAssignExpr.h"
#include "Workspace.h"

#include <iostream>
#include <list>
#include <sstream>


/** Static vector of strings giving names of operator types */
std::string SyntaxAssignExpr::opCode[] = { "ARROW_ASSIGN", "TILDE_ASSIGN", "EQUATION_ASSIGN" };


/** Construct from operator type, variable and expression */
SyntaxAssignExpr::SyntaxAssignExpr(SyntaxAssignExpr::operatorT op, SyntaxVariable* var, SyntaxElement* expr) 
    : SyntaxElement(), variable(var), expression(expr), opType(op) {
}


/** Deep copy constructor */
SyntaxAssignExpr::SyntaxAssignExpr(const SyntaxAssignExpr& x)
    : SyntaxElement(x) {

    variable   = new SyntaxVariable(*x.variable);
    expression = x.expression->clone();
    opType     = x.opType;
}


/** Destructor deletes operands */
SyntaxAssignExpr::~SyntaxAssignExpr() {
    
    delete variable;
    delete expression;
}


/** Return brief info about object */
std::string SyntaxAssignExpr::briefInfo () const {

    std::ostringstream   o;
    o << "SyntaxAssignExpr: operation = " << opCode[opType];

    return o.str();
}


/** Clone syntax element */
SyntaxElement* SyntaxAssignExpr::clone () const {

    return (SyntaxElement*)(new SyntaxAssignExpr(*this));
}


/** Get class vector describing type of object */
const VectorString& SyntaxAssignExpr::getClass(void) const { 

    static VectorString rbClass = VectorString(SyntaxAssignExpr_name) + SyntaxElement::getClass();
	return rbClass; 
}


/** Convert element to DAG node: insert symbol and return reference to it */
DAGNode* SyntaxAssignExpr::getDAGNodeExpr(VariableFrame* frame) const {

    // Insert symbol; discard the return value
    DAGNode* retVal = getValue(frame);
    if (retVal->numRefs() == 0)
        delete retVal;

    // Return DAG node from variable
    return variable->getDAGNodeExpr(frame);
}


/** Get semantic value: insert symbol and return the rhs value of the assignment */
DAGNode* SyntaxAssignExpr::getValue( VariableFrame* frame ) const {

    PRINTF( "Evaluating assign expression\n" );

    // Get variable info from lhs
    VectorInteger       elemIndex;
    VariableSlot*       slot        = NULL;
    DAGNode*            theVariable = variable->getLValue( frame, slot, elemIndex );
    RbString            varName;
    if ( variable->getIdentifier() != NULL )
        varName = *variable->getIdentifier();

    if ( theVariable == NULL && slot != NULL )
        throw RbException( "Unexpected l-value of assignment" );
    
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
        RbObject* value = retValue->getValue()->clone();

        if ( slot == NULL ) {

            // The variable does not exist - add it to current frame
            // We make the new slot as generous as possible (type RbObject)
            if ( elemIndex.size() > 0 && value->getDim() > 0 )
                throw RbException( "Invalid attempt to create container of containers" );

            PRINTF ( "Creating constant variable %s %s with a %s value %sthrough arrow assignment\n", TypeSpec( RbObject_name, elemIndex.size() ).toString().c_str(), varName.c_str(), value->getTypeSpec().toString().c_str(), elemIndex.size() > 0 ? "element " : "" );
            TypeSpec typeSpec( RbObject_name, elemIndex.size() );
            if ( elemIndex.size() == 0 )
                frame->addVariable( varName, typeSpec, wrapValue( value ) );
            else {
                VectorNatural index = elemIndex;
                frame->addVariable( varName, typeSpec, makeContainer( elemIndex, value ) );
            }
        }
        else if ( elemIndex.size() == 0 ) {

            // Assigning to an existing regular variable
            PRINTF ( "Assigning a value to %s %s through arrow assignment\n", theVariable->getTypeSpec(), theVariable->getName() );
            slot->setValue( value );
        }
        else /* if ( elemIndex.size() > 0 ) */ {

            // Assigning to a container element or member object element
            PRINTF ( "Assigning a value to %s %s%s through arrow assignment\n", theVariable->getTypeSpec(), theVariable->getName(), elemIndex.toIndexString() );
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
        else if ( node->isTemp() ) {
            if ( !node->isDAGType( LookupNode_name ) && !node->isDAGType( FunctionNode_name ) )
                throw RbException( "Invalid equation assignment using temp variable" );
        }

        // Set return value of the assignment expression
        // We return the root DAG node itself
        retValue = node;

        // Does the variable exist?
        if ( slot == NULL ) {
 
            // It does not exist - add it
            // We make the new slot as generous as possible (type RbObject)
            PRINTF ( "Creating variable %s %s with a dag expression %sthrough equation assignment\n", TypeSpec( node->getValueType(), elemIndex.size() ), varName, elemIndex.size() > 0 ? "element " : "" );
            TypeSpec typeSpec( RbObject_name, elemIndex.size() );
            if ( elemIndex.size() == 0 )
                frame->addVariable( varName, typeSpec, node );
            else {
                VectorNatural index = elemIndex;
                frame->addVariable( varName, typeSpec, makeContainer( elemIndex, node ) );
            }
        }
        else if ( elemIndex.size() == 0 ) {

            // It exists - replace it
            PRINTF ( "Assigning a dag expression to %s %s through equation assignment\n", theVariable->getTypeSpec(), theVariable->getName() );
            slot->setVariable( node );
        }
        else /* if ( elemIndex.size() > 0 ) */ {

            // Equation assignment to a container element or member object element
            PRINTF ( "Assigning a dag expression to %s %s%s through equation assignment\n", theVariable->getTypeSpec(), theVariable->getName(), elemIndex.toIndexString() );
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
        Distribution* distribution = static_cast<Distribution*>( dist->getMemberObject()->clone() );
        if ( distribution == NULL ) {
            delete exprValue;
            throw RbException( "Function returns a NULL distribution" );
        }

        // Create new stochastic node
        StochasticNode* node = new StochasticNode( distribution );

        // Set return value of the assignment expression
        // We return the stochastic node itself
        retValue = node;

        // Does the variable exist?
        if ( slot == NULL ) {
 
            // It does not exist - add it
            // We make the new slot as generous as possible (type RbObject)
            PRINTF ( "Creating variable %s %s with a stochastic node %sthrough tilde assignment\n", TypeSpec( node->getValueType(), elemIndex.size() ).toString().c_str(), varName, elemIndex.size() > 0 ? "element " : "" );
            TypeSpec typeSpec( RbObject_name, elemIndex.size() );
            if ( elemIndex.size() == 0 )
                frame->addVariable( varName, typeSpec, node );
            else {
                VectorNatural index = elemIndex;
                frame->addVariable( varName, typeSpec, makeContainer( elemIndex, node ) );
            }
        }
        else if ( elemIndex.size() == 0 ) {

            // It exists - replace it without type conversion
            PRINTF ( "Assigning a stochastic node of value type %s to %s %s through tilde assignment\n", node->getValueType(), theVariable->getTypeSpec(), theVariable->getName() );
            slot->setVariable( node, false );
        }
        else /* if ( elemIndex.size() > 0 ) */ {

            // Equation assignment to a container element or member object element without type conversion
            PRINTF ( "Assigning a stochastic node of value type %s to %s %s%s through tilde assignment\n", node->getValueType(), theVariable->getTypeSpec(), theVariable->getName(), elemIndex.toIndexString() );
            slot->setElement( elemIndex, node, false );
        }
    }

    return retValue;
}




/** Make new value container */
ContainerNode* SyntaxAssignExpr::makeContainer( const VectorNatural& index, RbObject* elem ) const {

    if ( elem->getDim() > 0 )
        throw RbException( "Illegal attempt to create container of containers" );
    
    std::vector<size_t> length;
    for ( size_t i = 0; i < index.size(); i++ )
        length.push_back( int( index[i] ) + 1 );

    ValueContainer* temp = new ValueContainer( TypeSpec( elem->getType(), index.size() ), length );
    temp->setElement( index, wrapValue( elem ) );

    return new ContainerNode( temp );
}


/** Make new variable container */
ContainerNode* SyntaxAssignExpr::makeContainer( const VectorNatural& index, DAGNode* elem ) const {

    if ( elem->getDim() > 0 )
        throw RbException( "Illegal attempt to create container of containers" );

    std::vector<size_t> length;
    for ( size_t i = 0; i < index.size(); i++ )
        length.push_back( int( index[i] ) + 1 );

    VariableContainer* temp = new VariableContainer( TypeSpec( elem->getValueType(), index.size() ), length );
    temp->setElement( index, elem );

    return new ContainerNode( temp );
}

/** Print info about the syntax element */
void SyntaxAssignExpr::print(std::ostream& o) const {

    o << "SyntaxAssignExpr:" << std::endl;
    o << "variable      = " << variable->briefInfo() << std::endl;
    o << "expression    = " << expression->briefInfo() << std::endl;
    o << "operation     = " << opCode[opType];
}


/** Wrap value appropriately to get a variable */
DAGNode* SyntaxAssignExpr::wrapValue( RbObject* value ) const {

    if ( value->isType( MemberObject_name ) )
        return new MemberNode( static_cast<MemberObject*>( value ) );
    else if ( value->isType( Container_name ) )
        return new ContainerNode( static_cast<Container*>( value ) );
    else
        return new ConstantNode( value );
}
