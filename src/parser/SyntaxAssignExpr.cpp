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

#include <iostream>
#include <list>
#include <sstream>

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
#include "VectorNatural.h"
#include "VectorString.h"
#include "SyntaxAssignExpr.h"
#include "Workspace.h"


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
DAGNode* SyntaxAssignExpr::getDAGNodeExpr(Frame* frame) const {

    // Insert symbol; discard the return value
    DAGNode* retVal = getValue(frame);
    if (retVal->numRefs() == 0)
        delete retVal;

    // Return DAG node from variable
    return variable->getDAGNodeExpr(frame);
}


/** Get semantic value: insert symbol and return the rhs value of the assignment */
DAGNode* SyntaxAssignExpr::getValue( Frame* frame ) const {

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

        // Calculate the value of the rhs expression; this can be a reference if the base element
        // is a function or a variable, but more often it is a temp variable value. If the rhs
        // expression evaluates to a reference, it is important we not make a clone of it, or we will
        // lose track of the original referenced variable. If it is a temp value, we do not want to
        // turn it into a reference, which would happen if we allowed the return value pointer to
        // point to exprValue, which will be transformed to a non-temp variable during the execution
        // of the assignment expression. Therefore we get a fresh clone in this case and use that
        // as the return value of the assign expression.
        DAGNode* exprValue = expression->getValue( frame );
        if ( exprValue == NULL )
            throw RbException( "Invalid NULL variable returned by rhs expression in assignment" );

        if ( exprValue->isTemp() )
            retValue = exprValue->clone();
        else
            retValue = exprValue;

        // Get handy reference to the object value
        const RbObject* theValue = exprValue->getValue();

        if ( slot == NULL ) {

            // The variable does not exist - add it to current frame
            // We make the new slot as generous as possible (type RbObject)
            if ( elemIndex.size() > 0 && exprValue->getDim() > 0 )
                throw RbException( "Invalid container to container assignment" );

            PRINTF ( "Creating variable %s %s with a %s node %sthrough arrow assignment\n", TypeSpec( RbObject_name, elemIndex.size() ).toString().c_str(), varName.c_str(), exprValue->getTypeSpec().toString().c_str(), elemIndex.size() > 0 ? "element " : "" );
            TypeSpec typeSpec( RbObject_name, elemIndex.size() );
            frame->addVariable( varName, typeSpec, elemIndex, exprValue ); 
		}
        else if ( slot->isReference() && elemIndex.size() == 0 ) {

            // Assignment to a reference variable
            if ( theValue == NULL ) {

                PRINTF ( "Setting reference variable %s to point to %s %s through arrow assignment\n", slot->getName(), slot->getTypeSpec(), "NULL" );
                slot->setReference( NULL );
            }
            else if ( exprValue->isTemp() ) {

                if ( theValue->isTypeSpec( slot->getTypeSpec() ) ) {

                    PRINTF ("Setting reference variable %s to point to %s <temp> through arrow assignment\n", slot->getName(), exprValue->getTypeSpec() );
                    slot->setReference( exprValue );
                }
                else if ( theValue->isConvertibleTo( slot->getTypeSpec(), true ) ) {
                
                    PRINTF ("Setting reference variable %s to point to %s <temp> through arrow assignment with type conversion\n", slot->getName(), exprValue->getTypeSpec() );
                    slot->setValue( theValue->convertTo( slot->getTypeSpec() ) );
                    if ( exprValue->numRefs() == 0 )
                        delete exprValue;
                }
            }
            // Unlike the temp case, here we want to test the variable, not the value
            else if ( exprValue->isTypeSpec( slot->getTypeSpec() ) ) {

                PRINTF ("Setting reference variable %s to point to %s %s through arrow assignment\n", slot->getName(), exprValue->getTypeSpec(), exprValue->getName());
                slot->setReference( exprValue );
            }
            else
                throw RbException( "Invalid assignment to reference variable " + slot->getTypeSpec() + " " + slot->getName() );
        }

#if 0
        // I think that allowing the setting of values of stochastic nodes through arrow assignment is a bad feature of the
        // language because it makes it impossible for users to convert stochastic variables to constant variables without
        // going over an equation assignment or calling a specialized function.
        // It is better to implement clamp, unclamp, and setval functions for stochastic nodes.
        else if ( theVariable->isDAGType( StochasticNode_name ) && theValue != NULL ) {

            // Setting value of existing stochastic node
            StochasticNode* theStochasticNode = static_cast<StochasticNode*>( theVariable );

            if ( theValue->isTypeSpec( theStochasticNode->getTypeSpec() ) ) {

                PRINTF ( "Setting the value of stochastic node %s %s through arrow assignment\n", theStochasticNode->getTypeSpec(), theStochasticNode->getName() );
                theStochasticNode->setValue( theValue->clone() );
            }
            else if ( theValue->isConvertibleTo( theStochasticNode->getTypeSpec(), true ) ) {

                PRINTF ( "Setting the value of stochastic node %s %s through arrow assignment with type conversion\n", theStochasticNode->getTypeSpec(), theStochasticNode->getName() );
                theStochasticNode->setValue( theValue->convertTo( theStochasticNode->getTypeSpec() ) );
                if ( exprValue->numRefs() == 0 )
                    delete exprValue;
            }
            else {

                std::ostringstream msg;
                msg << "Invalid assignment of " << theValue->getTypeSpec() << " value  to stochastic node ";
                msg << theStochasticNode->getTypeSpec() << " " << theStochasticNode->getName();
                throw RbException( msg );
            }
        }
#endif
        else {

            // Regular assignment to an existing variable

            // Get the value; we really want a temp variable at this point, if not NULL
            if ( theValue != NULL && !exprValue->isTemp() ) {
                exprValue = exprValue->clone();
                theValue = exprValue->getValue();
            }

            if ( elemIndex.size() == 0 ) {

                if ( exprValue->isTypeSpec( theVariable->getTypeSpec() ) ) {

                    PRINTF ( "Assigning a value to variable %s %s through arrow assignment\n", theVariable->getTypeSpec(), theVariable->getName() );
                    theVariable->mutateTo( exprValue );
                    if ( theVariable->numRefs() == 0 )
                        delete theVariable;
                }
                else if ( theValue == NULL || theValue->isConvertibleTo( theVariable->getTypeSpec(), true ) ) {
                    
                    PRINTF ( "Assigning a value to variable %s %s through arrow assignment with type conversion\n", theVariable->getTypeSpec(), theVariable->getName() );
                    
                    DAGNode*  newNode;
                    if ( theValue == NULL ) {
                        if ( theVariable->getDim() > 0 )
                            newNode = new ContainerNode( theVariable->getValueType(), theVariable->getDim() );
                        else if ( theVariable->isDAGType( MemberObject_name ) )
                            newNode = new MemberNode( theVariable->getValueType() );
                        else
                            newNode = new ConstantNode( theVariable->getValueType() );
                    }
                    else {
                        RbObject* newVal = theValue->convertTo( theVariable->getTypeSpec() );
                        if ( newVal->isType( Container_name ) )
                            newNode = new ContainerNode( static_cast<Container*>( newVal ) );
                        else if ( newVal->isType( MemberObject_name ) )
                            newNode = new MemberNode( static_cast<MemberObject*>( newVal ) );
                        else
                            newNode = new ConstantNode( newVal );
                    }
                    theVariable->mutateTo( exprValue );
                    if ( theVariable->numRefs() == 0 )
                        delete theVariable;
                    if ( exprValue->numRefs() == 0 )
                        delete exprValue;
                }
                else if ( theVariable->isMutableTo( exprValue ) ) {

                    PRINTF ( "Assigning a value to variable %s %s through arrow assignment with variable mutation\n", theVariable->getTypeSpec(), theVariable->getName() );
                    theVariable->mutateTo( exprValue );
                    if ( theVariable->numRefs() == 0 )
                        delete theVariable;
               }
               else
                   throw RbException( "Invalid assignment: conversion of source and mutation of target both failed" );
            }
            else {
                
                // Assigning of a value to a member object that wants to the job itself
                VectorNatural index = elemIndex;
                theVariable->setElement( index, exprValue );
            }
        }
    }

    // Deal with equation assignments
    else if ( opType == EquationAssign ) {

        PRINTF( "Equation assignment\n" );

        // We do not allow equation assignment into a reference slot
        // We may want to reconsider this, since "raw" lookups are
        // essentially the same as making a value slot into a reference slot
        if ( slot != NULL && slot->isReference() && elemIndex.size() == 0 )
            throw RbException( "Assignment of temp deterministc node to reference variable not allowed" );

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
        // We return a fresh copy of the root DAG node, or if it is
        // a LookupNode or a FunctionNode, we return a fresh copy of
        // the function variable value or the lookup variable
        // See implementation of getReference in the DAGNode class
        retValue = node->getReference()->clone();

        // Does the variable exist?
        if ( slot == NULL ) {
 
            // It does not exist - add it
            // We make the new slot as generous as possible (type RbObject)
            PRINTF ( "Creating variable %s %s with a dag expression %sthrough equation assignment\n", TypeSpec( node->getValueType(), elemIndex.size() ), varName, elemIndex.size() > 0 ? "element " : "" );
            TypeSpec typeSpec( RbObject_name, elemIndex.size() );
            frame->addVariable( varName, typeSpec, elemIndex, node );
        }
        else {

            // It exists - assign to it using the variable reference
            if ( elemIndex.size() != NULL ) {

                if ( Workspace::userWorkspace().isXOfTypeY( node->getTypeSpec(), theVariable->getTypeSpec() ) ) {
                    
                    PRINTF ( "Assigning a dag expression to variable %s %s through equation assignment\n", theVariable->getTypeSpec(), theVariable->getName() );
                    theVariable->mutateTo( node );
                }
                else if ( Workspace::userWorkspace().isXConvertibleToY( node->getTypeSpec(), theVariable->getTypeSpec() ) ) {
                    
                    PRINTF ( "Assigning a dag expression to variable %s %s through equation assignment with type conversion\n", theVariable->getTypeSpec(), theVariable->getName() );
                    theVariable->mutateTo( new ConverterNode( node, theVariable->getTypeSpec() ) );
                    if ( theVariable->numRefs() == 0 )
                        delete theVariable;
                }
                else if ( theVariable->isMutableTo( node ) ) {

                    PRINTF ( "Assigning a dag expression to variable %s %s through equation assignment with variable mutation\n", theVariable->getTypeSpec(), theVariable->getName() );
                    theVariable->mutateTo( node );
                    if ( theVariable->numRefs() == 0 )
                        delete theVariable;
                }
                else
                    throw RbException( "Invalid assignment: conversion of source and mutation of target both failed" );
                    
            }
            else {
                
                // Assignment of a deterministic node to a member object that wants to do the job itself
                VectorNatural index = elemIndex;
                theVariable->setElement( index, node );
            }
        }
    }

    // Deal with tilde assignments
    else if ( opType == TildeAssign ) {

        PRINTF( "Tilde assignment\n" );

        // We do not allow tilde assignment into a reference slot
        if ( slot != NULL && slot->isReference() && elemIndex.size() == 0 ) {
        
            throw RbException( "Assignment of temp stochastic node to reference variable not allowed" );
        }

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
        delete exprValue;

        // Set return value of the assignment expression
        // We return a fresh temp if we got a fresh distribution copy from the rhs
        // If we got a reference to a distribution, we return the reference
        // We need to do this before creating a new stochastic node from the
        // distribution, since this will make the exprValue a non-temp
        if ( exprValue->isTemp() )
            retValue = exprValue->clone();
        else
            retValue = exprValue;

        // Create new stochastic node
        StochasticNode* node = new StochasticNode( distribution );

        // Does the variable exist?
        if ( slot == NULL ) {
 
            // It does not exist - add it
            // We make the new slot as generous as possible (type RbObject)
            PRINTF ( "Creating variable %s %s with a stochastic node %sthrough tilde assignment\n", TypeSpec( node->getValueType(), elemIndex.size() ), varName, elemIndex.size() > 0 ? "element " : "" );
            TypeSpec typeSpec( RbObject_name, elemIndex.size() );
            frame->addVariable( varName, typeSpec, elemIndex, node ); 
        }
        else {

            // It exists - assign to it using the variable reference
            // Note that no type conversion is allowed here, only variable mutation
            if ( elemIndex.size() == 0 ) {

                if ( node->isTypeSpec( theVariable->getTypeSpec() ) ) {

                    PRINTF ( "Assigning a stochastic node of value type %s to variable %s %s through tilde assignment\n", node->getValueType(), theVariable->getTypeSpec(), theVariable->getName() );
                    theVariable->mutateTo( node );
                    if ( theVariable->numRefs() == 0 )
                        delete theVariable;
                }
                else if ( theVariable->isMutableTo( node ) ) {

                    PRINTF ( "Assigning a stochastic node of value type %s to variable %s %s through tilde assignment\n", node->getValueType(), theVariable->getTypeSpec(), theVariable->getName() );
                    theVariable->mutateTo( node );
                    if ( theVariable->numRefs() == 0 )
                        delete theVariable;
                else
                    throw RbException( "Invalid assignment: conversion of source not allowed and mutation of target failed" );
                }                    
            }
            else {
                
                // Assignment of a deterministic node to a member object that wants to do the job itself
                VectorNatural index = elemIndex;
                theVariable->setElement( index, node );
            }
        }
    }

    return retValue;
}


/** Print info about the syntax element */
void SyntaxAssignExpr::print(std::ostream& o) const {

    o << "SyntaxAssignExpr:" << std::endl;
    o << "variable      = " << variable->briefInfo() << std::endl;
    o << "expression    = " << expression->briefInfo() << std::endl;
    o << "operation     = " << opCode[opType];
}
