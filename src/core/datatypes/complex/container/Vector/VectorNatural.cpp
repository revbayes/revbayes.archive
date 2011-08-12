/**
 * @file
 * This file contains the implementation of VectorNatural, a
 * complex type used to hold vectors of natural numbers.
 *
 * @brief Implementation of VectorNatural
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "ContainerIterator.h"
#include "Natural.h"
#include "RbException.h"
#include "RbNames.h"
#include "Real.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorReal.h"
#include "VectorRealPos.h"
#include "VectorString.h"


/** Construct empty vector */
VectorNatural::VectorNatural( void ) : Vector( Natural_name ) {
}


/** Construct vector with one natural number x from an int */
VectorNatural::VectorNatural( int x ) : Vector( Natural_name ) {

    if ( x < 0 )
        throw RbException( "Trying to set " + Natural_name + "[] with negative value" );
    elements.push_back( new Natural( x ) );
    length = elements.size();
}


/** Construct vector with one natural number x from an unsigned int */
VectorNatural::VectorNatural( unsigned int x ) : Vector( Natural_name ) {

    elements.push_back( new Natural( x ) );
    length = elements.size();
}


/** Construct vector with n copies of natural number x (int) */
VectorNatural::VectorNatural( size_t n, int x ) : Vector( Natural_name ) {

    if ( x < 0 )
        throw RbException( "Trying to set " + Natural_name + "[] with negative value" );

    for ( size_t i = 0; i < n; i++ )
        elements.push_back( new Natural( x ) );
    length = elements.size();
}


/** Construct vector with n copies of natural number x (unsigned int) */
VectorNatural::VectorNatural( size_t n, unsigned int x ) : Vector( Natural_name ) {

    // No test needed, since unsigned ints are always positive
    // Natural will throw an error if the value is out of range
     for ( size_t i = 0; i < n; i++ )
        elements.push_back( new Natural( x ) );
    length = elements.size();
}


/** Constructor from int vector */
VectorNatural::VectorNatural( const std::vector<int>& x ) : Vector( Natural_name ) {

    for ( size_t i = 0; i < x.size(); i++ ) {
        if ( x[i] < 0 )
            throw RbException( "Trying to set " + Natural_name + "[] with negative value(s)" );
    }

    for ( size_t i = 0; i < x.size(); i++ )
        elements.push_back( new Natural( x[i] ) );
    length = elements.size();

}


/** Constructor from unsigned int vector */
VectorNatural::VectorNatural( const std::vector<unsigned int>& x ) : Vector( Natural_name ) {

    // No test needed, since unsigned ints are always positive
    // Natural will throw an error if the value is out of range
    for ( size_t i = 0; i < x.size(); i++ )
        elements.push_back( new Natural( x[i] ) );
    length = elements.size();
}


/** Constructor from Integer vector (VectorInteger) */
VectorNatural::VectorNatural( const VectorInteger& x ) : Vector( Natural_name ) {

    for ( size_t i = 0; i < x.getLength(); i++ ) {
        if ( x[i] < 0 )
            throw RbException( "Trying to set " + Natural_name + "[] with negative value(s)" );
    }

    for ( size_t i = 0; i < x.getLength(); i++ )
        elements.push_back( new Natural( x[i] ) );
    length = elements.size();

}


/** Constructor from ContainerIterator, which is a std::vector<int> */
VectorNatural::VectorNatural( const ContainerIterator& x ) : Vector( Natural_name ) {

    for ( size_t i = 0; i < x.size(); i++ ) {
        if ( x[i] < 0 )
            throw RbException( "Trying to set " + Natural_name + "[] with negative value(s)" );
    }

    for ( size_t i = 0; i < x.size(); i++ )
        elements.push_back( new Natural( x[i] ) );
    length = elements.size();
}


/**
 * Subscript operator. By only implementing a value
 * subscript operator, we can prevent users of the class from
 * modifying a value without passing through the validation
 * checks in this class.
 */
unsigned int VectorNatural::operator[]( size_t i ) const {

    if ( i >= elements.size() )
        throw RbException("Index out of bounds");

    return static_cast<Natural*>( elements[i] )->getValue();
}


/** Equals comparison */
bool VectorNatural::operator==( const VectorNatural& x ) const {

    if ( getLength() != x.getLength() )
        return false;

    for ( size_t i = 0; i < elements.size(); i++) {
        if ( operator[](i) != x[i] )
            return false;
    }

    return true;
}


/** Not equals comparison */
bool VectorNatural::operator!=( const VectorNatural& x ) const {

    return !operator==( x );
}


/** Clone function */
VectorNatural* VectorNatural::clone( void ) const {

    return new VectorNatural( *this );
}


/** Can we convert this vector into another object? */
RbLanguageObject* VectorNatural::convertTo(std::string const &type) const {
    
    // test for type conversion
    if (type == VectorRealPos_name) {
        
        // create an stl vector and add each element
        std::vector<double> d;
        for (std::vector<RbLanguageObject*>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            int i = dynamic_cast<Natural*>(*it)->getValue();
            d.push_back(i);
        }
        
        return new VectorRealPos(d);
    }
    else if (type == VectorInteger_name) {
        
        return new VectorInteger( getValue() );
    }
    else if (type == VectorReal_name) {
        
        // create an stl vector and add each element
        std::vector<double> d;
        for (std::vector<RbLanguageObject*>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            int i = dynamic_cast<Natural*>(*it)->getValue();
            d.push_back(i);
        }
        
        return new VectorRealPos(d);
    }
    
    return Vector::convertTo(type);
}


/** Get class vector describing type of object */
const VectorString& VectorNatural::getClass( void ) const {

    static VectorString rbClass = VectorString( VectorNatural_name ) + Vector::getClass();
    return rbClass;
}


/** Get value as an STL vector of int */
std::vector<int> VectorNatural::getValue( void ) const {

    std::vector<int> temp;
    for ( std::vector<RbLanguageObject*>::const_iterator i = elements.begin(); i != elements.end(); i++ )
        temp.push_back( static_cast<Natural*>( (*i) )->getValue() );

    return temp;
}


/** Get value as an STL vector of unsigned int */
std::vector<unsigned int> VectorNatural::getUnsignedValue( void ) const {

    std::vector<unsigned int> temp;
    for ( std::vector<RbLanguageObject*>::const_iterator i = elements.begin(); i != elements.end(); i++ )
        temp.push_back( static_cast<Natural*>( (*i) )->getValue() );

    return temp;
}


/** Can we convert this vector into another object? */
bool VectorNatural::isConvertibleTo(std::string const &type, bool once) const {
    
    // test for type conversion
    if (type == VectorRealPos_name || type == VectorInteger_name || type == VectorReal_name) {
        
        return true;
    }
    
    return Vector::isConvertibleTo(type, once);
}

/** Push an int onto the back of the vector after checking */
void VectorNatural::push_back( int x ) {

    if ( x < 0 )
        throw RbException( "Trying to set " + Natural_name + "[] with negative value" );

    elements.push_back( new Natural( x ) );
    length++;
}


/** Push an int onto the front of the vector after checking */
void VectorNatural::push_front( int x ) {

    if ( x < 0 )
        throw RbException( "Trying to set " + Natural_name + "[] with negative value" );

    elements.insert( elements.begin(), new Natural( x ) );
    length++;
}


/** Complete info about object */
std::string VectorNatural::richInfo( void ) const {

    std::ostringstream o;
    o <<  "VectorNatural: value = ";
    printValue( o );

    return o.str();
}


/** Set value of vector using STL vector of int */
void VectorNatural::setValue( const std::vector<int>& x ) {

    if ( x.size() != elements.size() ) {
    
        clear();
        for ( size_t i = 0; i < x.size(); i++ ) {
            if ( x[i] < 0 )
                throw RbException( "Trying to set " + Natural_name + "[] with negative value(s)" );
            elements.push_back( new Natural( x[i] ) );
        }
        length = elements.size();
    }
    else {

        for ( size_t i = 0; i < x.size(); i++ ) {
            if ( x[i] < 0 )
                throw RbException( "Trying to set " + Natural_name + "[] with negative value(s)" );
            static_cast<Natural*>( elements[i] )->setValue( x[i] );
        }
    }
}


/** Set value of vector using STL vector of unsigned int */
void VectorNatural::setValue( const std::vector<unsigned int>& x ) {

    if ( x.size() != elements.size() ) {
    
        clear();
        for ( size_t i = 0; i < x.size(); i++ )
            elements.push_back( new Natural( x[i] ) );
        length = elements.size();
    }
    else {

        for ( size_t i = 0; i < x.size(); i++ )
            static_cast<Natural*>( elements[i] )->setValue( x[i] ); 
    }
}


/** Set value of vector using VectorNatural */
void VectorNatural::setValue( const VectorNatural& x ) {

    if ( x.getLength() != elements.size() ) {
    
        clear();
        for ( size_t i = 0; i < x.getLength(); i++ )
            elements.push_back( new Natural( x[i] ) );
        length = elements.size();
    }
    else {

        for ( size_t i = 0; i < x.getLength(); i++ )
            static_cast<Natural*>( elements[i] )->setValue( x[i] ); 
    }
}   


/** Set value of vector using VectorInteger */
void VectorNatural::setValue( const VectorInteger& x ) {

    if ( x.getLength() != elements.size() ) {
    
        clear();
        for ( size_t i = 0; i < x.getLength(); i++ )
            elements.push_back( new Natural( x[i] ) );
        length = elements.size();
    }
    else {

        for ( size_t i = 0; i < x.getLength(); i++ )
            static_cast<Natural*>( elements[i] )->setValue( x[i] ); 
    }
}

