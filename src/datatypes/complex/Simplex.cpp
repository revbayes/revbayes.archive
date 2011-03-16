/**
 * @file
 * This file contains the implementation of Simplex, a complex type
 * used to hold a simplex.
 *
 * @brief Implementation of Simplex
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

#include "RbException.h"
#include "RbNames.h"
#include "Real.h"
#include "Simplex.h"
#include "VectorInteger.h"
#include "VectorString.h"

#include <sstream>


/** Construct simplex of length (size) n */
Simplex::Simplex(const size_t n) : VectorRealPos() {

    for (size_t i=0; i<n; i++)
        value.push_back(1.0/n);
}


/** Construct simplex from double vector */
Simplex::Simplex(const std::vector<double>& x) : VectorRealPos(x) {

    for (size_t i=0; i<x.size(); i++)
        if (x[i] <= 0.0)
            throw (RbException("Cannot set simplex from nonpositive value"));

    rescale();
}


/** Construct simplex from double vector */
Simplex::Simplex(const VectorRealPos& x) : VectorRealPos(x) {

    for (size_t i=0; i<x.size(); i++)
        if (x[i] <= 0.0)
            throw (RbException("Cannot set simplex from nonpositive value"));

    rescale();
}


/** Non-const subscript operator */
double& Simplex::operator[](size_t i) {

    throw (RbException("Cannot grant non-const access to single value of simplex"));
}


/** Clone function */
Simplex* Simplex::clone() const {

    return new Simplex(*this);
}


/** Pointer-based equals comparison */
bool Simplex::equals(const RbObject* obj) const {

    // First check type
    if ( !obj->isType(Simplex_name) )
        return false;

    // Then use parent's function
    return VectorRealPos::equals(obj);
}


/** Get class vector describing type of object */
const VectorString& Simplex::getClass() const {

    static VectorString rbClass = VectorString(Simplex_name) + VectorRealPos::getClass();
    return rbClass;
}


/** Drop an element */
void Simplex::pop_back(void) {

    throw (RbException("Cannot resize simplex"));
}


/** Append an element */
void Simplex::push_back(double x) {

    throw (RbException("Cannot resize simplex"));
}


/** Add an element in front */
void Simplex::push_front(double x) {

    throw (RbException("Cannot resize simplex"));
}


/** Rescale the simplex */
void Simplex::rescale(void) {

   // Rescale the simplex
   double sum = 0.0;
   for (size_t i=0; i<value.size(); i++)
       sum += value[i];

   for (size_t i=0; i<value.size(); i++)
       value[i] /= sum;
}


/** Do not allow the parser to resize the simplex */
void Simplex::resize(const VectorInteger& len) {

    throw (RbException("Cannot resize simplex"));
}


/** Allow parser to set an element (actually do not) */
void Simplex::setElement(const VectorInteger& index, RbObject* val) {

    throw (RbException("Cannot set single value of simplex"));
}


/** Set value of simplex using VectorRealPos */
void Simplex::setValue(const VectorRealPos& x) {

    value.resize(x.size());
    for (size_t i=0; i<x.size(); i++)    
        value[i] = x[i];
    rescale();
}   


/** Set value of simplex using vector<double> */
void Simplex::setValue(const std::vector<double>& x) {

    for (std::vector<double>::const_iterator i=x.begin(); i!=x.end(); i++)
        if ((*i) < 0.0)
            throw (RbException("Cannot set simplex with negative value"));

    value.clear();
    value = x;
    rescale();
}


/** Complete info about object */
std::string Simplex::toString(void) const {

    std::ostringstream o;
    o <<  "Simplex: value = ";
    printValue(o);

    return o.str();
}

