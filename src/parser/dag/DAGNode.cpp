/**
 * @file
 * This file contains the implementation of DAGNode, which is the base
 * class for nodes in the model DAG.
 *
 * @brief Implementation of DAGNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @interface DAGNode
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "DagNodeFunction.h"
#include "MethodTable.h"
#include "Monitor.h"
#include "Plate.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "RbObject.h"
#include "RbOptions.h"
#include "RbString.h"
#include "Variable.h"
#include "VariableNode.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>



/** Constructor of filled node */
DAGNode::DAGNode( const RbPtr<const Plate> &p ) : children(), parents(), plate( p ) {
    
}


/**
 * Copy constructor should not copy children because it creates an
 * independent node. The new node is simply not used by any other objects
 * at this point. The parent nodes are left empty here because they are better
 * dealt with by the derived VariableNode classes, which have to maintain
 * dual copies of them (function arguments, distribution parameters,
 * or container elements).
 */
DAGNode::DAGNode( const DAGNode& x ) : children(), parents(), plate( x.plate ) {
    
    // copy the name so that we still be able to identify the variable in a cloned DAG
    name = x.name;
    
}


/** Destructor deletes value if not NULL */
DAGNode::~DAGNode( void ) {

    for (std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++) {
        VariableNode* theNode = *i;
        theNode->removeParentNode( this );
    }
}


/** add a child node */
void DAGNode::addChildNode(VariableNode *c) {
    PRINTF("Adding child with name \"%s\" to parent with name \"%s\".\n",c->getName().c_str(),getName().c_str());
        
    children.insert(c);
    
    // if this node is eliminated, than we pass on the factor root
    // the factor root is the node which start the likelihood calculation for the part of the graph which is eliminated
    if ( isNotInstantiated() ) {
        c->setFactorRoot( static_cast<const VariableNode*>( this )->getFactorRoot() );
        
        // we know that this must be a variable node
        static_cast<VariableNode*>( this )->likelihoodsNeedUpdates();
    }
}

/** Execute method. 
 * This is the deault implemention. You should overwrite this method only if you want to treat the multidimensional data type RlValue
 * yourselve.
 *
 * We assumes that the arguments come as scalar types, not multidimensional RevLanguage objects.
 * Therefore, we just delegate the call to executeSimpleMethod.
 */
RbPtr<RbLanguageObject> DAGNode::executeMethod(std::string const &name, const std::vector<RlValue<const RbLanguageObject> > &args) {
    
    std::vector<const RbLanguageObject *> simpleArgs;
    for (std::vector<RlValue<const RbLanguageObject> >::const_iterator i =args.begin(); i != args.end(); ++i) {
        // check first if these arguments are actually scalars
        if (i->lengths.size() > 0 ) {
            throw RbException("We currently do not support setting of multidimensional member values!");
        }
        simpleArgs.push_back( i->getSingleValue() );
    }
    
    // calling the internal method to execute the member-method
    return executeSimpleMethod(name, simpleArgs );
}


/** 
 * DAG nodes might provides special member access functions. 
 * These functions are implemented here.
 */
RbPtr<RbLanguageObject> DAGNode::executeSimpleMethod(std::string const &name, const std::vector<const RbLanguageObject *> &args) {
    
    throw RbException("No method with name '" + name + "' available for DAG nodes.");
}



const std::set<VariableNode *>& DAGNode::getChildren( void ) const {
    return children;
}


/** Get class name of object */
const std::string& DAGNode::getClassName(void) { 
    
    static std::string rbClassName = "DAG node";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& DAGNode::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


/**
 * Get all affected nodes this DAGNode.
 * This means we call getAffected() of all children. getAffected() is pure virtual.
 */
void DAGNode::getAffectedNodes(std::set<RbPtr<StochasticNode> > &affected) {
    
    // get all my affected children
    for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->getAffected(affected);
}

/** 
 * Get method specifications of all DAG node types.
 * We do not support any methods (yet).
 */
const MethodTable& DAGNode::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool        methodsSet = false;
    
    if ( methodsSet == false ) {

        methodsSet = true;
    }   
    
    return methods;
}

/** Get name of DAG node from its surrounding objects */
const std::string& DAGNode::getName( void ) const {

    return name;
}


const RbPtr<const Plate>& DAGNode::getPlate( void ) const {
    return plate;
}



const std::set<RbPtr<DAGNode> >& DAGNode::getParents( void ) const {
    return parents;
}

/** Is this a constant node */
bool DAGNode::isConst( void ) const {

    return false;
}


/** Check if node is a parent of node x in the DAG: needed to check for cycles in the DAG */
bool DAGNode::isParentInDAG( const DAGNode* x, std::list<DAGNode*>& done ) const {

    for( std::set<RbPtr<DAGNode> >::const_iterator i = parents.begin(); i != parents.end(); i++ ) {

        if ( std::find( done.begin(), done.end(), (*i) ) == done.end() ) {
            if ( (*i)->isParentInDAG( x, done ) )
                return true;
        }
    }

    return false;
}


/**
 * Keep the value of the node.
 * This function delegates the call to keepMe() and calls keepAffected() too.
 */
void DAGNode::keep(void) {
    // keep myself first
    keepMe();
    
    // next, keep all my children
    keepAffected();
}

/**
 * Tell affected variable nodes to keep the current value.
 */
void DAGNode::keepAffected() {
    
    // keep all my children
    for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->keepMe();
}

/** Print children */
void DAGNode::printChildren( std::ostream& o ) const {

    o << "[ ";

    for ( std::set<VariableNode*>::const_iterator i = children.begin(); i != children.end(); i++) {
        if ( i != children.begin() )
            o << ", ";
        if ( (*i)->getName() == "" ) {
            o << "<";
            (*i)->printValue(o);
            o << ">";
        }
        else
            o << (*i)->getName();
    }
    o << " ]";
}


/** Print parents */
void DAGNode::printParents( std::ostream& o ) const {

    o << "[ ";

    for ( std::set<RbPtr<DAGNode> >::const_iterator i = parents.begin(); i != parents.end(); i++) {
        if ( i != parents.begin() )
            o << ", ";
        if ( (*i)->getName() == "" ) {
            o << "<";
            (*i)->printValue(o);
            o << ">";
        }
        else
            o << (*i)->getName();
    }
    o << " ]";
}

/** Remove a child from this DAG node. Free the pointer to the child. */
void DAGNode::removeChildNode(VariableNode *c) {
    
    // test if we even have this node as a child
    if (children.find(c) != children.end()) {
        // we do not own our children! See addChildNode for explanation
       
        // remove the child from our list
        children.erase(c);
    }
    
}


/**
 * Restore this DAGNode.
 * This means we call restoreMe() and restoreAffected(). There is a default implementation of restoreAffected()
 * which call restoreMe() of all children of this node. restoreMe() is pure virtual.
 */
void DAGNode::restore(void) {
    // first restore myself
    restoreMe();
    
    // next, restore all my children
    restoreAffected();
}


/**
 * Restore all affected nodes this DAGNode.
 * This means we call restoreMe() of all children. restoreMe() is pure virtual.
 */
void DAGNode::restoreAffected(void) {
    
    // next, restore all my children
    for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->restoreMe();
}


void DAGNode::setPlate(const Plate *p) {
    plate = p;
}


/**
 * Touch the DAG node.
 *
 * This function should be called if the value of the variable has changed or if you want this node to be reevaluated. 
 * The function will automatically call the touchMe() which is implemented differently in the different DAG node types.
 *
 * Since the DAG node was touch and possibly changed, we tell affected variable nodes to keep the current value.
 */
void DAGNode::touch() {
    // first touch myself
    touchMe();
    
    // next, touch all my children
    touchAffected();
}


/**
 * Tell affected variable nodes to touch themselves (i.e. that they've been touched).
 */
void DAGNode::touchAffected() {
    
    // touch all my children
    for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->touchMe();
}

