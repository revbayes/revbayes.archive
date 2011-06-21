/**
 * @file
 * This file contains the implementation of List, a
 * complex type used to hold lists of variables.
 *
 * @brief Implementation of List
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */


#include "ConstantNode.h"
#include "ContainerNode.h"
#include "MemberNode.h"
#include "RbException.h"
#include "List.h"
#include "RbNames.h"
#include "Real.h"
#include "VectorInteger.h"
#include "VectorRealPos.h"
#include "VectorString.h"


/** Construct empty list */
List::List( void ) : MemberObject(getMemberRules()) {
}


/** Subscript operator allows caller access to variables */
DAGNode* List::operator[](size_t i) const {

    if ( i > variables.size()  )
        throw RbException( "Index out of bound" );

    // All variables (members) are value variables,
    // so we only give out copies
    return variables[i]->clone();
}


/** Add variable to list */
void List::addVariable( DAGNode* var, const std::string& name ) {

    members.addVariable( name, TypeSpec(RbObject_name, var->getDim() ), var );
    names.push_back( name );
    variables.push_back( var );
}


/** Clone function */
List* List::clone() const {

    return new List(*this);
}


/** Map member methods to internal function calls */
DAGNode* List::executeOperation(const std::string& name, ArgumentFrame& args) {

    return MemberObject::executeOperation( name, args );
}


/** Get class vector describing type of object */
const VectorString& List::getClass() const {

    static VectorString rbClass = VectorString(List_name) + MemberObject::getClass();
    return rbClass;
}


/** Return member rules (no original members) */
const MemberRules& List::getMemberRules(void) const {

    static MemberRules memberRules;
    return memberRules;
}


/** Get methods (no methods) */
const MethodTable& List::getMethods(void) const {

    static MethodTable   methods;
    return methods;
}


/**
 * Get subscript element for parser. By giving back a temp variable rather than
 * a reference to a list element, we ensure that the parser cannot set the element
 */
DAGNode* List::getSubelement( VectorInteger& index ) {

    if ( index.size() < 1 )
        throw RbException( "Subscript index empty for " + List_name );

    if ( index[0] < 0 || index[0] > int( variables.size() ) )
        throw RbException( "Index out of bounds for " + List_name );

    if ( index.size() == 1 ) {

        return variables[index[0]]->clone();
    }
    else {

        VectorInteger tempIndex;
        for ( size_t i = 1; i < index.size(); i++ )
            tempIndex.push_back( index[i] );
        index = tempIndex;

        DAGNode* temp = variables[index[0]]->clone();
        if ( !temp->isDAGType( MemberNode_name ) && !temp->isDAGType( ContainerNode_name ) ) {
            delete temp;
            throw RbException( "Object does not have elements" );
        }

        DAGNode* retVal;
        if ( temp->isDAGType( MemberNode_name ) )
            retVal = static_cast<MemberNode*>( temp )->getElement( index );
        else
            retVal = static_cast<ContainerNode*>( temp )->getElement( index );
        
        delete temp;
        return retVal;
    }
}


/** Print value for user */
void List::printValue(std::ostream& o) const {


    for ( size_t i = 0; i < variables.size(); i++ ) {

        if ( names[i] == "" )
            o << "[" << i << "]" << std::endl;
        else
            o << "." << names[i] << std::endl;

        variables[i]->printValue( o );

        o << std::endl;
    }
}


/** Complete info about object */
std::string List::richInfo(void) const {

    std::ostringstream o;
    o <<  "List with " << variables.size() << " elements" << std::endl;
    printValue(o);

    return o.str();
}

