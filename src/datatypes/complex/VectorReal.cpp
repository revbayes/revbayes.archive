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
 * @extends RbComplex
 *
 * $Id$
 */

#include "ContainerIterator.h"
#include "RbException.h"
#include "RbNames.h"
#include "Real.h"
#include "VectorInteger.h"
#include "VectorReal.h"
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
    length[0] = 1;
}


/** Construct vector with n doubles x */
VectorReal::VectorReal(const size_t n, const double x) : Vector(Real_name) {

    for (size_t i = 0; i < n; i++)
        elements.push_back(new Real(x));
    length[0] = elements.size();
}


/** Constructor from double vector */
VectorReal::VectorReal(const std::vector<double>& x) : Vector(Real_name) {

    for (std::vector<double>::const_iterator i=x.begin(); i!=x.end(); i++)
        elements.push_back(new Real(*i));
    length[0] = elements.size();
}


/** Subscript operator */
double& VectorReal::operator[](size_t i) {

    if (i < 0 || i > int(elements.size()))
        throw RbException("Index out of bounds");

    return static_cast<Real*>(elements[i])->getValueRef();
}


/** Subscript const operator */
const double& VectorReal::operator[](size_t i) const {

    if (i < 0 || i > int(elements.size()))
        throw RbException("Index out of bounds");
    return static_cast<Real*>(elements[i])->getValueRef();
}


/** Equals comparison */
bool VectorReal::operator==(const VectorReal& x) const {

    if (size() != x.size())
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


/** Get class vector describing type of object */
const VectorString& VectorReal::getClass(void) const {

    static VectorString rbClass = VectorString(VectorReal_name) + Vector::getClass();
    return rbClass;
}


/** Export value as STL vector */
std::vector<double> VectorReal::getValue(void) const {

    std::vector<double> temp;
    for (size_t i=0; i<size(); i++)
        temp.push_back(operator[](i));

    return temp;
}


/** Print value for user */
void VectorReal::printValue(std::ostream& o) const {

    int previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();

    o << "[ ";
    o << std::fixed;
    o << std::setprecision(1);
    for (std::vector<RbObject*>::const_iterator i = elements.begin(); i!= elements.end(); i++) 
        {
        if (i != elements.begin())
            o << ", ";
        o << *(*i);
        }
    o <<  " ]";
    if (getIsRowVector() == false)
        o << "'";

    o.setf(previousFlags);
    o << std::setprecision(previousPrecision);
}


/** Append element to end of vector, updating length in process */
void VectorReal::push_back(double x) {

    elements.push_back(new Real(x));
    length[0]++;
}


/** Add element in front of vector, updating length in process */
void VectorReal::push_front(double x) {

    elements.insert(elements.begin(), new Real(x));
    length[0]++;
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
    for (std::vector<double>::const_iterator i=x.begin(); i!=x.end(); i++) {   
        elements.push_back(new Real(*i));
        length[0]++;
    }
}   


/** Set value of vector using VectorReal */
void VectorReal::setValue(const VectorReal& x) {

    clear();
    for (size_t i=0; i<x.size(); i++) {   
        elements.push_back(new Real(x[i]));
        length[0]++;
    }
}   

