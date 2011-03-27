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
    VectorInteger       index;
    VariableSlot*       slot = NULL;
    std::string         varName;
    variable->getVariableInfo( frame, slot, &index );
    varName = *variable->getIdentifier();
    
    // Get references to the variable and to the last addressable element
    DAGNode*        variableRef  = NULL;
    DAGNode*        lastVariable = NULL;
    VectorInteger   lastIndex;
    if ( slot != NULL ) {
        variableRef = const_cast<DAGNode*>( slot->getVariable()->getVarElement(index) );
        if ( variableRef != NULL ) {
            lastVariable = variableRef;
            // lastIndex should remain empty
        }
        else {
            // Get last index and last variable
            lastIndex = index;
            lastVariable = variableRef;
            for (;;) {
                if ( (int) lastIndex.size() < lastVariable->getDim() )
                    throw ( "Assignment to subcontainer not allowed (yet)" );

                // Get index to element in current variable
                VectorInteger tempIndex;
                size_t i;
                for ( i = 0; (int) i < lastVariable->getDim(); i++ ) {
                    tempIndex.push_back( lastIndex[i] );
                }

                // Break if next variable is NULL
                const DAGNode* temp = lastVariable->getVarElement( tempIndex );
                if ( temp == NULL )
                    break;

                // Update lastIndex and lastVariable
                tempIndex.clear();
                for ( ; (int) i < lastIndex.getDim(); i++ )
                    tempIndex.push_back( lastIndex[i] );
                lastIndex    = tempIndex;
                lastVariable = const_cast<DAGNode*>( temp );
            }
        }
    }

    // Declare variable storing the return value of the assignment expression
    DAGNode* retValue;

    // Deal with arrow assignments
    if ( opType == ArrowAssign ) {

        PRINTF("Arrow assignment\n");

        // Calculate the value of the rhs expression; this can be a reference if the base element
        // is a function or a variable but more often it is a value.
        // If the rhs expression evaluates to a reference, it is important we not make a clone of it.
        // If it is a value, we do not want to turn it into a reference, which would happen if we allowed
        // the return value pointer to point to exprValue. Therefore we get a fresh clone in this case.
        DAGNode* exprValue = expression->getValue( frame );
        if ( exprValue->isTemp() )
            retValue = exprValue->clone();
        else
            retValue = exprValue;

        if ( slot == NULL ) {

            // The variable does not exist - add it to current frame
            PRINTF ( "Creating variable %s %s with a constant node %sthrough arrow assignment\n",
                TypeSpec( exprValue->getValueType(), index.size() ), varName, index.size() > 0 ? "element " : "" );
            frame->addVariable( varName, index, exprValue );
		}
        else if ( slot->isReference() && index.size() == 0 ) {

            // Assignment to a reference variable
            if ( exprValue == NULL || exprValue->getValue() == NULL )
                PRINTF ( "Setting reference variable %s to point to %s %s through arrow assignment\n",
                    slot->getName(), slot->getTypeSpec(), "NULL" );
            else if ( exprValue->isTemp() )
                PRINTF ("Setting reference variable %s to point to %s <temp> through arrow assignment\n",
                    slot->getName(), exprValue->getTypeSpec() );
            else
                PRINTF ("Setting reference variable %s to point to %s %s through arrow assignment\n",
                    slot->getName(), exprValue->getTypeSpec(), exprValue->getName());
            slot->setReference( exprValue );
        }
        else if ( variableRef != NULL && variableRef->isDAGType( StochasticNode_name ) &&
                  exprValue != NULL && exprValue->getValue() != NULL ) {

            // Setting value of existing stochastic node
            StochasticNode* theStochasticNode = static_cast<StochasticNode*>( variableRef );

            const RbObject* theValue = exprValue->getValue();
            if ( theValue->isType( theStochasticNode->getValueType() ) ) {

                PRINTF ( "Setting the value of stochastic node %s %s through arrow assignment\n",
                    theStochasticNode->getTypeSpec(), theStochasticNode->getName() );
                theStochasticNode->setValue( theValue->clone() );
            }
            else if ( theValue->isConvertibleTo( theStochasticNode->getTypeSpec(), true ) ) {

                PRINTF ( "Setting the value of stochastic node %s %s through arrow assignment with type conversion\n",
                    theStochasticNode->getTypeSpec(), theStochasticNode->getName() );
                theStochasticNode->setValue(exprValue->getValue()->convertTo( theStochasticNode->getValueType() ) );
            }
            else {
                std::ostringstream msg;
                msg << "Invalid assignment of " << theValue->getTypeSpec() << " value  to ";
                msg << theStochasticNode->getTypeSpec() << " " << theStochasticNode->getName();
                throw RbException( msg );
            }
        }
        else {

            // Regular assignment

            // Get the value; we really want a temp variable at this point, if not NULL
            const RbObject* theValue = NULL;
            if ( exprValue != NULL ) {
                if ( !exprValue->isTemp() )
                    exprValue = exprValue->clone();
                theValue = exprValue->getValue();
            }

            // It exists - assign to it using the variable reference or the last addressable variable
            if ( variableRef != NULL ) {

                if ( theValue == NULL || theValue->isType( variableRef->getTypeSpec() ) ) {

                    PRINTF ( "Assigning a value to variable %s %s through arrow assignment\n",
                        variableRef->getTypeSpec(), variableRef->getName() );
                    variableRef->mutateTo( exprValue ); 
                }
                else if ( variableRef->isMutableTo( exprValue ) ) {

                    PRINTF ( "Assigning a value to variable %s %s through arrow assignment with type conversion\n",
                        variableRef->getTypeSpec(), variableRef->getName() );
                    variableRef->mutateTo( exprValue );
                    if ( variableRef->numRefs() == 0 )
                        delete variableRef;
               }
               else
                    throw RbException( "Invalid assignment" );
            }
            else {
                
                if ( theValue == NULL || ( lastIndex.size() == lastVariable->getDim() && theValue->isType( lastVariable->getValueType() ) ) ) {

                    PRINTF ( "Assigning a value to variable %s %s %sthrough arrow assignment\n",
                        lastVariable->getTypeSpec(), lastVariable->getName(), lastIndex.size() > 0 ? "element " : "" );
                    variableRef->setElement( lastIndex, exprValue ); 
                }
                else if ( lastIndex.size() == lastVariable->getDim() && theValue->isConvertibleTo( lastVariable->getValueType(), true ) ) {

                    PRINTF ( "Assigning a value to variable %s %s %sthrough arrow assignment with type conversion\n",
                        lastVariable->getTypeSpec(), lastVariable->getName(), lastIndex.size() > 0 ? "element " : "" );
                    variableRef->setElement( lastIndex, theValue->convertTo( lastVariable->getTypeSpec() ) );
                    delete exprValue;
                }
                else if ( lastVariable->isMutableTo( TypeSpec( theValue->getType(), theValue->getDim() + lastIndex.size() ) ) ) {

                    PRINTF ( "Assigning a value to variable %s %s element through arrow assignment with variable mutation\n",
                        lastVariable->getTypeSpec(), lastVariable->getName() );
                    DAGNode* newVar = lastVariable->mutateTo( TypeSpec( theValue->getType(), theValue->getDim() + lastIndex.size() ) );
                    newVar->setElement( lastIndex, exprValue );
                    if ( lastVariable->numRefs() == 0 )
                        delete lastVariable;
                }
                else {

                    // We give up and hope that lastVariable knows what to do
                    lastVariable->setElement( lastIndex, exprValue );
                }
            }
        }
    }

    // Deal with equation assignments
    else if (opType == EquationAssign) {

        PRINTF("Equation assignment\n");

        // We do not allow equation assignment into a reference slot
        // We may want to reconsider this, since "raw" lookups are
        // essentially the same as making a value slot into a reference slot
        if ( slot != NULL && slot->isReference() && index.size() == 0 ) {
        
            throw RbException( "Assignment of temp deterministc node to reference variable not allowed" );
        }

        // Get DAG node representation of expression
        // We allow direct references without lookup nodes
        // We also allow constant expressions
        DAGNode* node = expression->getDAGNodeExpr(frame);
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
            PRINTF ( "Creating variable %s %s with a dag expression %sthrough equation assignment\n",
                TypeSpec( node->getValueType(), index.size() ), varName, index.size() > 0 ? "element " : "" );
            frame->addVariable(varName, index, node);
        }
        else {

            // It exists - assign to it using the variable reference or the last addressable variable
            if ( variableRef != NULL ) {

                if ( Workspace::userWorkspace().isXOfTypeY( node->getTypeSpec(), variableRef->getTypeSpec() ) ) {
                    
                    PRINTF ( "Assigning a dag expression to variable %s %s through equation assignment\n",
                        variableRef->getTypeSpec(), variableRef->getName() );
                    variableRef->mutateTo( node ); 
                }
                else if ( Workspace::userWorkspace().isXConvertibleToY( node->getTypeSpec(), variableRef->getTypeSpec() ) ) {
                    
                    PRINTF ( "Assigning a dag expression to variable %s %s through equation assignment with type conversion\n",
                        variableRef->getTypeSpec(), variableRef->getName() );
                    variableRef->mutateTo( new ConverterNode( node, variableRef->getTypeSpec() ) );
                }
                else if ( variableRef->isMutableTo( node ) ) {

                    PRINTF ( "Assigning a dag expression to variable %s %s through equation assignment with variable mutation\n",
                        variableRef->getTypeSpec(), variableRef->getName() );
                    variableRef->mutateTo( node );
                    if ( variableRef->numRefs() == 0 )
                        delete variableRef;
               }
               else
                    throw RbException( "Invalid assignment" );
            }
            else {
                
                if ( lastIndex.size() == lastVariable->getDim() && Workspace::userWorkspace().isXOfTypeY( node->getTypeSpec(), lastVariable->getTypeSpec() ) ) {

                    PRINTF ( "Assigning a dag expression to variable %s %s %sthrough equation assignment\n",
                        lastVariable->getTypeSpec(), lastVariable->getName(), lastIndex.size() > 0 ? "element " : "" );
                    variableRef->setElement( lastIndex, node );
                }
                else if ( lastIndex.size() == lastVariable->getDim() && Workspace::userWorkspace().isXConvertibleToY( node->getTypeSpec(), lastVariable->getTypeSpec() ) ) {

                    PRINTF ( "Assigning a value to variable %s %s %sthrough equation assignment with type conversion\n",
                        lastVariable->getTypeSpec(), lastVariable->getName(), lastIndex.size() > 0 ? "element " : "" );
                    variableRef->setElement( lastIndex, new ConverterNode( node, lastVariable->getTypeSpec() ) );
                }
                else if ( lastVariable->isMutableTo( TypeSpec( node->getValueType(), node->getDim() + lastIndex.size() ) ) ) {

                    PRINTF ( "Assigning a value to variable %s %s element through arrow assignment with variable mutation\n",
                        lastVariable->getTypeSpec(), lastVariable->getName() );
                    DAGNode* newVar = lastVariable->mutateTo( TypeSpec( node->getValueType(), node->getDim() + lastIndex.size() ) );
                    newVar->setElement( lastIndex, node );
                    if ( lastVariable->numRefs() == 0 )
                        delete lastVariable;
                }
                else {

                    // We give up and hope that lastVariable knows what to do
                    lastVariable->setElement( lastIndex, node );
                }
            }
        }
    }

    // Deal with tilde assignments
    else if (opType == TildeAssign) {

        PRINTF( "Tilde assignment\n" );

        // We do not allow tilde assignment into a reference slot
        if ( slot->isReference() && index.size() == 0 ) {
        
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
        if ( dist == NULL || !Workspace::userWorkspace().isXOfTypeY( dist->getTypeSpec(), Distribution_name ) ) {
            delete exprValue;
            throw RbException( "Function does not return a distribution" );
        }

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
        StochasticNode* node = new StochasticNode( dist );

        // Does the variable exist?
        if ( slot == NULL ) {
 
            // It does not exist - add it
            PRINTF ( "Creating variable %s %s with a stochastic node %sthrough tilde assignment\n",
                TypeSpec( node->getValueType(), index.size() ), varName, index.size() > 0 ? "element " : "" );
            frame->addVariable( varName, index, node );
        }
        else {

            // It exists - assign to it using the variable reference or the last addressable variable
            // Note that new type conversion is allowed here, only variable mutation
            if ( variableRef != NULL ) {

                PRINTF ( "Assigning a stochastic node of value type %s to variable %s %s through tilde assignment\n",
                    node->getValueType(), variableRef->getTypeSpec(), variableRef->getName() );
                if ( variableRef->isMutableTo(node) ) {
                    variableRef->mutateTo( node );
                    if ( variableRef->numRefs() == 0 )
                        delete variableRef;
                }
                else
                    throw RbException( "Invalid assignment" );
            }
            else if ( lastVariable->isMutableTo( TypeSpec( node->getValueType(), lastIndex.size()) ) ) {

                PRINTF ( "Assigning a stochastic node of value type %s to variable %s %s through tilde assignment\n",
                    node->getValueType(), lastVariable->getTypeSpec(), lastVariable->getName() );
                DAGNode* newVar = lastVariable->mutateTo( TypeSpec( node->getValueType(), lastIndex.size() ) );
                newVar->setElement( lastIndex, node );
                if ( lastVariable->numRefs() == 0 )
                    delete lastVariable;
            }
            else
                throw RbException( "Invalid assignment" );
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
