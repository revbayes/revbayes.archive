/**
 * @file
 * This file contains the implementation of FunctionNode, which is derived
 * from DeterministicNode. FunctionNode is used for a DAG node associated with
 * an explicit function call that determines its value.
 *
 * @brief Implementation of FunctionNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-30 00:19:25 +0100 (Ons, 30 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: FunctionNode.cpp 216 2009-12-29 23:19:25Z ronquist $
 */

#include "ArgumentFrame.h"
#include "ArgumentRule.h"
#include "Boolean.h"
#include "DAGNode.h"
#include "FunctionNode.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "VectorString.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>


/** Constructor of empty function node */
FunctionNode::FunctionNode( const TypeSpec& valType )
    : DeterministicNode( valType.getType() ), valueDim( valType.getDim() ) {	
}


/** Constructor from function: get parents from the function object */
FunctionNode::FunctionNode( RbFunction* func )
    : DeterministicNode( func->getReturnType().getType() ), valueDim( func->getReturnType().getDim() )  {

    /* Check for cycles */
    const ArgumentFrame& arguments = func->getArgs();
    std::list<DAGNode*> done;
    for ( size_t i = 0; i < arguments.size(); i++ ) {
        if ( arguments[i].getVariable()->isParentInDAG( this, done ) )
            throw RbException( "Invalid assignment: cycles in the DAG" );
    }

    /* Set parents and add this node as a child node of these */
    for ( size_t i = 0; i < arguments.size(); i++ ) {

        DAGNode* theArgument = const_cast<DAGNode*>( arguments[i].getVariable() );
        parents.insert( theArgument );
        theArgument->addChildNode( this );
    }

    /* Set the function */
    function = func;

    /* Set value and stored value */
    DAGNode* retVal = function->execute();
    value           = retVal->getValue()->clone();
    storedValue     = NULL;
    delete retVal;
}


/** Copy constructor */
FunctionNode::FunctionNode( const FunctionNode& x ) : DeterministicNode( x ), valueDim( x.valueDim ) {

    function        = x.function->clone();
    touched         = x.touched;
    changed         = x.changed;
    value           = x.value->clone();
    if ( x.storedValue != NULL )
        storedValue = x.storedValue->clone();
    else
        storedValue = NULL;

    /* Set parents and add this node as a child node of these */
    const ArgumentFrame& args = function->getArgs();
    for ( size_t i = 0; i < args.size(); i++ ) {

        DAGNode* theArgument = const_cast<DAGNode*>( args[i].getVariable() );
        parents.insert( theArgument );
        theArgument->addChildNode( this );
    }
}


/** Destructor */
FunctionNode::~FunctionNode( void ) {

    if ( numRefs() != 0 )
        throw RbException ( "Cannot delete FunctionNode with references" );

    /* Remove parents first so that VariableNode destructor does not get in the way */
    for ( std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++ )
        (*i)->removeChildNode(this);
    parents.clear();

    /* Deleting the function will remove all the parent nodes when appropriate */
    if ( function )
        delete function;
}


/** Assignment operator */
FunctionNode& FunctionNode::operator=( const FunctionNode& x ) {

    if ( this != &x ) {

        if (valueType != x.valueType)
            throw RbException( "Type mismatch in FunctionNode assignment" );
        
        /* Remove parents first */
        for ( std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++ )
            (*i)->removeChildNode( this );
        parents.clear();

        delete value;
        if ( storedValue )
            delete storedValue;
        delete function;    // This will delete any DAG nodes that need to be deleted

        function        = x.function->clone();
        touched         = x.touched;
        changed         = x.changed;
        value           = x.value->clone();
        if ( x.storedValue != NULL )
            storedValue = x.storedValue->clone();
        else
            storedValue = NULL;

        /* Set parents and add this node as a child node of these */
        const ArgumentFrame& args = function->getArgs();
        for ( size_t i = 0; i < args.size(); i++ ) {

            DAGNode* theArgument = const_cast<DAGNode*>( args[i].getVariable() );
            parents.insert( theArgument );
            theArgument->addChildNode( this );
        }
    }

    return ( *this );
}


/** Clone this object */
FunctionNode* FunctionNode::clone( void ) const {

	return new FunctionNode( *this );
}


/** Clone the entire graph: clone children, swap parents */
FunctionNode* FunctionNode::cloneDAG( std::map<const DAGNode*, DAGNode*>& newNodes ) const {

    if ( newNodes.find( this ) != newNodes.end() )
        return static_cast<FunctionNode*>( newNodes[ this ] );

    /* Get pristine copy */
    FunctionNode* copy = new FunctionNode( valueType );
    newNodes[ this ] = copy;

    /* Set the copy member variables */
    copy->function = function->clone();
    copy->touched  = touched;
    copy->changed  = changed;
    copy->value    = value->clone();
    if (storedValue == NULL)
        copy->storedValue = NULL;
    else
        copy->storedValue = storedValue->clone();

    /* Set the copy arguments to their matches in the new DAG */
    const ArgumentFrame& args     = function->getArgs();
    ArgumentFrame&       copyArgs = const_cast<ArgumentFrame&>( copy->function->getArgs() );

    for ( size_t i = 0; i < args.size(); i++ ) {

        DAGNode* theArgClone = args[i].getVariable()->cloneDAG( newNodes );
        copyArgs[i].setVariable( theArgClone );

        copy->parents.insert( theArgClone );
        theArgClone->addChildNode( copy );
    }

    DAGNode* retVal   = copy->function->execute();
    copy->value       = retVal->getValue()->clone();
    copy->storedValue = NULL;
    copy->touched     = false;
    copy->changed     = false;
    delete retVal;

    /* Make sure the children clone themselves */
    for( std::set<VariableNode*>::const_iterator i = children.begin(); i != children.end(); i++ ) {
        (*i)->cloneDAG( newNodes );
    }

    return copy;
}


/** Does element exist ? */
bool FunctionNode::existsElement( VectorInteger& index ) {

    return getReference()->existsElement( index );
}


/** Get class vector describing type of DAG node */
const VectorString& FunctionNode::getDAGClass() const {

    static VectorString rbClass = VectorString( FunctionNode_name ) + DeterministicNode::getDAGClass();
    return rbClass;
}


/** Get element for parser */
DAGNode* FunctionNode::getElement( VectorInteger& index ) {

    return getReference()->getElement( index );
}


/** Get element reference for modification by parser */
DAGNode* FunctionNode::getElementRef( VectorNatural& index) {

    return getReference()->getElementRef( index );
}


/** Execute function and get a reference */
DAGNode* FunctionNode::getReference(void) {

    if ( !isValueReference )
        throw RbException( "Function does not return a reference" );

    touched = true;
    changed = false;
    return function->execute();
}


/** Execute function and get a variable value */
DAGNode* FunctionNode::getVariable(void) {

    if ( isValueReference )
        throw RbException( "Function does not return a variable value" );

    return function->execute();
}


/** Print struct for user */
void FunctionNode::printStruct( std::ostream& o ) const {

    o << "_DAGClass    = " << getDAGClass() << std::endl;
    o << "_valueType   = " << valueType << std::endl;
    o << "_dim         = " << getDim() << std::endl;
    o << "_function    = " << function << std::endl;
    o << "_touched     = " << ( touched ? Boolean( true ) : Boolean( false ) ) << std::endl;
    o << "_changed     = " << ( changed ? Boolean( true ) : Boolean( false ) ) << std::endl;
    o << "_value       = " << value->briefInfo() << std::endl;
    if ( touched && changed )
        o << "_storedValue = " << storedValue->briefInfo() << std::endl;

    o << "_parents     = ";
    printParents(o);
    o << std::endl;

    o << "_children    = ";
    printChildren(o);
    o << std::endl;

    o << std::endl;
}


/** Complete info about object */
std::string FunctionNode::richInfo( void ) const {

    std::ostringstream o;

    o << "FunctionNode:" << std::endl;

    o << "name        = " << getName() << std::endl;
    o << "touched     = " << (touched ? "true" : "false") << std::endl;
    o << "changed     = " << (changed ? "true" : "false") << std::endl;

    o << "function    = ";
    function->printValue(o);
    o << std::endl;

    o << "value       = ";
    value->printValue(o);
    o << std::endl;

    o << "storedValue = ";
    storedValue->printValue(o);
    o << std::endl;

    return o.str();
}


/** Allow parser to set element */
void FunctionNode::setElement( VectorNatural& index, DAGNode* var ) {

    getReference()->setElement( index, var );
}


/** Update value and stored value after node and its surroundings have been touched by a move */
void FunctionNode::update( void ) {

    if ( touched && !changed ) {

        assert( storedValue == NULL );
        
        DAGNode* retVal = function->execute();

        storedValue     = value;
        value           = retVal->getValue()->clone();
        changed         = true;

        delete retVal;
    }
}

