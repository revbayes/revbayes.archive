/**
 * @file
 * This file contains the implementation of VectorReal, a complex type
 * used to hold double vectors.
 *
 * @brief Implementation of VectorReal
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends Vector
 *
 * $Id$
 */

#include "ContainerIterator.h"
#include "RbException.h"
#include "RbNames.h"
#include "Real.h"
#include "VectorInteger.h"
#include "VectorReal.h"
#include "VectorRealPos.h"
#include "VectorString.h"

#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>



/** Default constructor */
VectorReal::VectorReal(void) : Vector(Real_name) {
}


/** Construct vector with one double x */
VectorReal::VectorReal(const double x) : Vector(Real_name) {

    elements.push_back(new Real(x));
    length = 1;
}


/** Construct vector with n doubles x */
VectorReal::VectorReal(const size_t n, const double x) : Vector(Real_name) {

    for (size_t i = 0; i < n; i++)
        elements.push_back(new Real(x));
    length = elements.size();
}


/** Constructor from double vector */
VectorReal::VectorReal(const std::vector<double>& x) : Vector(Real_name) {

    for (std::vector<double>::const_iterator i=x.begin(); i!=x.end(); i++)
        elements.push_back(new Real(*i));
    length = elements.size();
}


/** Constructor from VectorRealPos */
VectorReal::VectorReal( const VectorRealPos& x ) : Vector( Real_name ) {

    for ( size_t i = 0; i < x.getLength(); i++ )
        elements.push_back( new Real( x[i] ) );
    length = elements.size();
}


/** Subscript operator */
double& VectorReal::operator[](size_t i) {

    if (i >= int(elements.size()))
        throw RbException("Index out of bounds");

    return static_cast<Real*>(elements[i])->getValueReference();
}


/** Subscript const operator */
const double& VectorReal::operator[](size_t i) const {

    if (i >= int(elements.size()))
        throw RbException("Index out of bounds");
    return static_cast<Real*>(elements[i])->getValueReference();
}


/** Equals comparison */
bool VectorReal::operator==(const VectorReal& x) const {

    if (getLength() != x.getLength())
        return false;

    for (size_t i=0; i<elements.size(); i++) {
        if (operator[](i) != x[i])
            return false;
    }

    return true;
}


/** Not equals comparison */
bool VectorReal::operator!=(const VectorReal& x) const {

    return !operator==(x);
}


/** Clone function */
VectorReal* VectorReal::clone(void) const {

    return new VectorReal(*this);
}


/** Can we convert this vector into another object? */
RbLanguageObject* VectorReal::convertTo(std::string const &type) const {
    
    // test for type conversion
    if (type == VectorRealPos_name) {
        
        return new VectorRealPos( getValue() );
    }
    
    return Vector::convertTo(type);
}


/** Get class vector describing type of object */
const VectorString& VectorReal::getClass(void) const {

    static VectorString rbClass = VectorString(VectorReal_name) + Vector::getClass();
    return rbClass;
}


/** Export value as STL vector */
std::vector<double> VectorReal::getValue(void) const {

    std::vector<double> temp;
    for (size_t i=0; i<getLength(); i++)
        temp.push_back(operator[](i));

    return temp;
}


/** Can we convert this vector into another object? */
bool VectorReal::isConvertibleTo(std::string const &type, bool once) const {
    
    // test for type conversion
    if (type == VectorRealPos_name && once == true) {
        
        for (std::vector<RbLanguageObject*>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            Real *x = dynamic_cast<Real*>(*it);
            
            // test whether we can convert this element, otherwise return false
            if (!x->isConvertibleTo(type, once)) {
                return false;
            }
        }
        
        return true;
    }
    
    return Vector::isConvertibleTo(type, once);
}


/** Print value for user */
void VectorReal::printValue(std::ostream& o) const {

    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();

    o << "[ ";
    o << std::fixed;
    o << std::setprecision(1);
    for (std::vector<RbLanguageObject*>::const_iterator i = elements.begin(); i!= elements.end(); i++) 
        {
        if (i != elements.begin())
            o << ", ";
        o << *(*i);
        }
    o <<  " ]";

    o.setf(previousFlags);
    o.precision(previousPrecision);
}


/** Append element to end of vector, updating length in process */
void VectorReal::push_back(double x) {

    elements.push_back(new Real(x));
    length++;
}


/** Add element in front of vector, updating length in process */
void VectorReal::push_front(double x) {

    elements.insert(elements.begin(), new Real(x));
    length++;
}


/** Complete info about object */
std::string VectorReal::richInfo(void) const {

    std::ostringstream o;
    o <<  "VectorReal: value = ";
    printValue(o);

    return o.str();
}


/** Set value of vector using STL vector */
void VectorReal::setValue(const std::vector<double>& x) {

    clear();
    length = 0;
    for (std::vector<double>::const_iterator i=x.begin(); i!=x.end(); i++) {   
        elements.push_back(new Real(*i));
        length++;
    }
}   


/** Set value of vector using VectorReal */
void VectorReal::setValue(const VectorReal& x) {

    clear();
    length = 0;
    for (size_t i=0; i<x.getLength(); i++) {   
        elements.push_back(new Real(x[i]));
        length++;
    }
}   

