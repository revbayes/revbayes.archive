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


/** Construct empty list */
List::List( void ) : MemberObject() {
}


/** Copy constructor */
List::List( const List& x ) : MemberObject( getMemberRules() ) {

    for ( size_t i=0; i<elements.size(); i++ ) {
        
        elements.push_back( x.elements[i]->clone() );
        names.push_back( x.names[i] );
    }
}


/** Destructor */
List::~List( void ) {

    for ( size_t i=0; i<elements.size(); i++ )
        delete elements[i];
}


/** Assignment operator */
List& List::operator=( const List& x ) {

    if ( this != &x ) {
        
        for ( size_t i=0; i<elements.size(); i++ )
            delete elements[i];

        for ( size_t i=0; i<elements.size(); i++ ) {
            
            elements.push_back( x.elements[i]->clone() );
            names.push_back( x.names[i] );
        }
    }
    
    return ( *this );
}


/** Index operator allows caller convenient access to variables */
DAGNode* List::operator[]( size_t i ) const {

    if ( i > elements.size()  )
        throw RbException( "Index out of bound" );

    // We give out pointers to the variables themselves
    return const_cast<DAGNode*>( elements[i]->getVariable() );
}


/** Add variable element to list */
void List::addElement( DAGNode* var, const std::string& name ) {

    // Add a value slot or reference slot depending on whether or not we own the variable
    if ( var->getSlot() == NULL )
        elements.push_back( new VariableSlot( TypeSpec(RbObject_name, var->getDim() ), var ) );
    else
        elements.push_back( new VariableSlot( TypeSpec(RbObject_name, var->getDim(), true ), var ) );

    // Add name
    names.push_back( name );
}


/** Clone function */
List* List::clone() const {

    return new List( *this );
}


/** Get class vector describing type of object */
const VectorString& List::getClass() const {

    static VectorString rbClass = VectorString( List_name ) + MemberObject::getClass();
    return rbClass;
}


/** Get indexed element */
DAGNode* List::getElement( size_t index ) {

    return operator[]( index );
}


/** Get element index */
size_t List::getElementIndex( std::string& s ) const {
    
    size_t i;
    for ( i=0; i<names.size(); i++ ) {
        if ( names[i] == s )
            break;
    }
    
    if ( i == names.size() )
        throw RbException( "No element with index name '" + s + "'" );
    
    return i;
}


/** Print value for user */
void List::printValue(std::ostream& o) const {


    for ( size_t i = 0; i < members.size(); i++ ) {

        if ( names[i] == "" )
            o << "[" << i + 1 << "]" << std::endl;
        else
            o << "[" << names[i] << "]" << std::endl;

        elements[i]->getValue()->printValue( o );

        o << std::endl;
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
    
    //! @todo Fredrik: Fill this in
}