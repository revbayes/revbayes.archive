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


#include "List.h"
#include "RbException.h"
#include "RbNames.h"
#include "Real.h"
#include "VariableSlot.h"
#include "VectorString.h"

#include <algorithm>


List::List(void) {

}


/** Construct empty list */
List::List( bool ref ) : MemberObject(), referenceList(ref) {

}

List::List(size_t len) {

}


/** Index operator */
const DAGNode* List::operator[]( size_t i ) const {

    // We give out a const pointer to the element
    return elements[i].getVariable();
}


/** Index operator allows caller convenient access to variables */
DAGNode* List::operator[]( size_t i ) {

    // We (attempt to) give out a non-const pointer to the element
    // Variable slot throws an error if we are not allowed to
    return elements[i].getReference();
}


/** Add variable element to list */
void List::addElement( DAGNode* var, const std::string& name ) {

    // Add a value slot or reference slot depending on whether we are a value or reference list
    if ( referenceList == false )
        elements.push_back( name, new VariableSlot( TypeSpec(RbObject_name, var->getDim() ), var ) );
    else
        elements.push_back( name, new VariableSlot( TypeSpec(RbObject_name, var->getDim(), true ), var ) );
}


/** Clone function */
List* List::clone() const {

    return new List( *this );
}


/** Get clone with constant values (evaluate all member variables and replace with constants) */
List* List::cloneWithoutConnections( void ) const {

    List* temp = clone();

    for ( size_t i = 0; i < temp->elements.size(); i++ ) {
    
        RbObject* constValue = temp->elements[i].getValue()->cloneWithoutConnections();
        temp->elements[i].setReferenceFlag( false );
        temp->elements[i].setValue( constValue );
    }

    return temp;
}


/** Get class vector describing type of object */
const VectorString& List::getClass() const {

    static VectorString rbClass = VectorString( List_name ) + MemberObject::getClass();
    return rbClass;
}


/** Get (or make) indexed element for parser */
DAGNode* List::getElement( size_t index ) {

    if ( referenceList ) 
        return elements[index].getReference();
    else
        return elements[index].getVariable()->clone();
}


/** Get element index */
size_t List::getElementIndex( std::string& elemName ) const {
    
    return elements.getIndex( elemName );
}


/** Get member rules (no members) */
const MemberRules& List::getMemberRules(void) const {

    static MemberRules memberRules;
    return memberRules;
}


/** Get methods (no methods) */
const MethodTable& List::getMethods(void) const {

    static MethodTable   methods;
    return methods;
}


/** Print value for user */
void List::printValue( std::ostream& o ) const {


    for ( size_t i = 0; i < elements.size(); i++ ) {

        std::string elemName = elements.getLabel( i );

        if ( elemName == "" )
            o << "[" << i + 1 << "]" << std::endl;
        else
            o << "[" << elemName << "]" << std::endl;

        elements[i].getValue()->printValue( o );

        o << std::endl << std::endl;
    }
}


/** Complete info about object */
std::string List::richInfo(void) const {

    std::ostringstream o;
    o <<  "List with " << elements.size() << " elements" << std::endl;
    printValue(o);

    return o.str();
}


/** Set element */
void List::setElement( size_t index, DAGNode* var, bool convert) {
    
    elements[index].setVariable( var, convert );
}

