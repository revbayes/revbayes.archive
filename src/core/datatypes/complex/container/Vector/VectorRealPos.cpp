/**
 * @file
 * This file contains the implementation of VectorRealPos,
 * a vector type used to hold positive real numbers.
 *
 * @brief Implementation of VectorRealPos
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#include "ContainerIterator.h"
#include "RbException.h"
#include "RbNames.h"
#include "RealPos.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorReal.h"
#include "VectorRealPos.h"
#include "VectorString.h"

#include <iomanip>

/** Construct empty vector */
VectorRealPos::VectorRealPos( void ) : Vector( RealPos_name ) {
}


/** Construct vector with one positive real x from a double */
VectorRealPos::VectorRealPos( double x ) : Vector( RealPos_name ) {

    if ( x <= 0.0 )
        throw RbException( "Trying to set " + RealPos_name + "[] with nonpositive value" );
    elements.push_back( new RealPos( x ) );
    length[0] = elements.size();
}


/** Construct vector with n copies of positive real x from a double */
VectorRealPos::VectorRealPos( size_t n, double x ) : Vector( RealPos_name ) {

    if ( x <= 0.0 )
        throw RbException( "Nonpositive value for " + VectorRealPos_name );

    for ( size_t i = 0; i < n; i++ )
        elements.push_back( new RealPos( x ) );
    length[0] = elements.size();
}


/** Constructor from double vector */
VectorRealPos::VectorRealPos( const std::vector<double>& x ) : Vector( RealPos_name ) {

    for ( size_t i = 0; i < x.size(); i++ ) {
        if ( x[i] <= 0.0 )
            throw RbException( "Trying to set " + RealPos_name + "[] with nonpositive value(s)" );
    }

    for ( size_t i = 0; i < x.size(); i++ )
        elements.push_back( new RealPos( x[i] ) );
    length[0] = elements.size();

}


/** Constructor from VectorReal */
VectorRealPos::VectorRealPos( const VectorReal& x ) : Vector( RealPos_name ) {

    for ( size_t i = 0; i < x.size(); i++ ) {
        if ( x[i] <= 0.0 )
            throw RbException( "Trying to set " + RealPos_name + "[] with nonpositive value(s)" );
    }

    for ( size_t i = 0; i < x.size(); i++ )
        elements.push_back( new RealPos( x[i] ) );
    length[0] = elements.size();

}


/**
 * Subscript operator. By only implementing a value
 * subscript operator, we can prevent users of the class from
 * modifying a value without passing through the validation
 * checks in this class.
 */
double VectorRealPos::operator[]( size_t i ) const {

    if (i < 0 || i > int(elements.size()))
        throw RbException("Index out of bounds");
    return static_cast<RealPos*>(elements[i])->getValue();
}


/** Equals comparison */
bool VectorRealPos::operator==( const VectorRealPos& x ) const {

    if ( size() != x.size() )
        return false;

    for ( size_t i = 0; i < elements.size(); i++) {
        if ( operator[](i) != x[i] )
            return false;
    }

    return true;
}


/** Not equals comparison */
bool VectorRealPos::operator!=( const VectorRealPos& x ) const {

    return !operator==( x );
}


/** Clone function */
VectorRealPos* VectorRealPos::clone( void ) const {

    return new VectorRealPos( *this );
}


/** Get class vector describing type of object */
const VectorString& VectorRealPos::getClass( void ) const {

    static VectorString rbClass = VectorString( VectorRealPos_name ) + Vector::getClass();
    return rbClass;
}


/** Get value as an STL vector of double */
std::vector<double> VectorRealPos::getValue( void ) const {

    std::vector<double> temp;
    for ( std::vector<RbObject*>::const_iterator i = elements.begin(); i != elements.end(); i++ )
        temp.push_back( static_cast<RealPos*>( (*i) )->getValue() );

    return temp;
}


/** Print value for user */
void VectorRealPos::printValue(std::ostream& o) const {

    int previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();

    o << "[ ";
    o << std::fixed;
    o << std::setprecision(1);
    for (std::vector<RbObject*>::const_iterator i = elements.begin(); i!= elements.end(); i++) {
        if (i != elements.begin())
            o << ", ";
        o << *(*i);
    }
    o <<  " ]";

    o.setf(previousFlags);
    o << std::setprecision(previousPrecision);
}


/** Push a double onto the back of the vector after checking */
void VectorRealPos::push_back( double x ) {

    if ( x <= 0.0 )
        throw RbException( "Trying to set " + RealPos_name + "[] element with nonpositive value" );

    elements.push_back( new RealPos( x ) );
    length[0]++;
}


/** Push a double onto the front of the vector after checking */
void VectorRealPos::push_front( double x ) {

    if ( x <= 0.0 )
        throw RbException( "Trying to set " + RealPos_name + "[] element with nonpositive value" );

    elements.insert( elements.begin(), new RealPos( x ) );
    length[0]++;
}


/** Complete info about object */
std::string VectorRealPos::richInfo( void ) const {

    std::ostringstream o;
    o <<  "VectorRealPos: value = ";
    printValue( o );

    return o.str();
}


/** Set value of vector using STL vector of double */
void VectorRealPos::setValue( const std::vector<double>& x ) {

    if ( x.size() != elements.size() ) {
    
        clear();
        for ( size_t i = 0; i < x.size(); i++ ) {
            if ( x[i] <= 0.0 )
                throw RbException( "Trying to set " + RealPos_name + "[] with nonpositive value(s)" );
            elements.push_back( new RealPos( x[i] ) );
        }
        length[0] = elements.size();
    }
    else {

        for ( size_t i = 0; i < x.size(); i++ ) {
            if ( x[i] <= 0.0 )
                throw RbException( "Trying to set " + RealPos_name + "[] with nonpositive value(s)" );
            static_cast<RealPos*>( elements[i] )->setValue( x[i] );
        }
    }
}


/** Set value of vector using VectorInteger */
void VectorRealPos::setValue( const VectorInteger& x ) {

    if ( x.size() != elements.size() ) {
    
        clear();
        for ( size_t i = 0; i < x.size(); i++ ) {
            if ( x[i] <= 0.0 )
                throw RbException( "Trying to set " + RealPos_name + "[] with nonpositive value(s)" );
            elements.push_back( new RealPos( x[i] ) );
        }
        length[0] = elements.size();
    }
    else {

        for ( size_t i = 0; i < x.size(); i++ ) {
            if ( x[i] <= 0.0 )
                throw RbException( "Trying to set " + RealPos_name + "[] with nonpositive value(s)" );
            static_cast<RealPos*>( elements[i] )->setValue( x[i] );
        }
    }
}


/** Set value of vector using VectorNatural */
void VectorRealPos::setValue( const VectorNatural& x ) {

    if ( x.size() != elements.size() ) {
    
        clear();
        for ( size_t i = 0; i < x.size(); i++ )
            elements.push_back( new RealPos( x[i] ) );
        length[0] = elements.size();
    }
    else {

        for ( size_t i = 0; i < x.size(); i++ )
            static_cast<RealPos*>( elements[i] )->setValue( x[i] );
    }
}


/** Set value of vector using VectorReal */
void VectorRealPos::setValue( const VectorReal& x ) {

    if ( x.size() != elements.size() ) {
    
        clear();
        for ( size_t i = 0; i < x.size(); i++ ) {
            if ( x[i] <= 0.0 )
                throw RbException( "Trying to set " + RealPos_name + "[] with nonpositive value(s)" );
            elements.push_back( new RealPos( x[i] ) );
        }
        length[0] = elements.size();
    }
    else {

        for ( size_t i = 0; i < x.size(); i++ ) {
            if ( x[i] <= 0.0 )
                throw RbException( "Trying to set " + RealPos_name + "[] with nonpositive value(s)" );
            static_cast<RealPos*>( elements[i] )->setValue( x[i] );
        }
    }
}


/** Set value of vector using VectorRealPos */
void VectorRealPos::setValue( const VectorRealPos& x ) {

    if ( x.size() != elements.size() ) {
    
        clear();
        for ( size_t i = 0; i < x.size(); i++ )
            elements.push_back( new RealPos( x[i] ) );
        length[0] = elements.size();
    }
    else {

        for ( size_t i = 0; i < x.size(); i++ )
            static_cast<RealPos*>( elements[i] )->setValue( x[i] ); 
    }
}   

