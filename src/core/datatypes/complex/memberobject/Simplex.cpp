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
 *
 * $Id$
 */


#include "ConstantNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "Real.h"
#include "Simplex.h"
#include "VectorInteger.h"
#include "VectorReal.h"
#include "VectorRealPos.h"
#include "VectorString.h"
#include <iomanip>



/** Construct simplex of length (size) n */
Simplex::Simplex(const size_t n) : VectorRealPos(n,1.0/n) {

}


/** Construct simplex from STL vector */
Simplex::Simplex(const std::vector<double>& x) : VectorRealPos(x) {

    rescale();
}


/** Construct simplex from VectorRealPos, which is guaranteed to have real positive numbers */
Simplex::Simplex(const VectorRealPos& x) : VectorRealPos(x) {

    rescale();
}

/** Const subscript operator allowing caller to see value but not to modify it */
double Simplex::operator[](size_t i) {
    
    return VectorRealPos::operator[](i);
}

/** Const subscript operator allowing caller to see value but not to modify it */
const double& Simplex::operator[](size_t i) const {

    return VectorRealPos::operator[](i);
}


/** Clone function */
Simplex* Simplex::clone() const {

    return new Simplex(*this);
}


/** Get class vector describing type of object */
const VectorString& Simplex::getClass() const {

    static VectorString rbClass = VectorString(Simplex_name) + MemberObject::getClass();
    return rbClass;
}


/** Print value for user */
void Simplex::printValue(std::ostream& o) const {

    VectorRealPos::printValue(o);
}


/** Rescale the simplex */
void Simplex::rescale(void) {

    // Rescale the simplex
    double sum = 0.0;
    for (size_t i=0; i<elements.size(); i++)
       sum += static_cast<RealPos*>(elements[i])->getValue();

    for (size_t i=0; i<elements.size(); i++) {
        RealPos* val = static_cast<RealPos*>(elements[i]);
        val->setValue(val->getValue() / sum);
    }
    
}

/** Complete info about object */
std::string Simplex::richInfo(void) const {

    std::ostringstream o;
    printValue(o);
    return o.str();
}


/** Set value of simplex using VectorReal */
void Simplex::setValue(const VectorReal& x) {

    elements.clear();
    for (size_t i=0; i<x.getLength(); i++)    
        elements.push_back(new RealPos(x[i]));
    
    rescale();
    
    // set the number of elements
    length = elements.size();
}   


/** Set value of simplex using VectorRealPos */
void Simplex::setValue(const VectorRealPos& x) {
    
    elements.clear();
    for (size_t i=0; i<x.getLength(); i++)    
        elements.push_back(new RealPos(x[i]));
    rescale();
    
    // set the number of elements
    length = elements.size();
}   


/** Set value of simplex using vector<double> */
void Simplex::setValue(const std::vector<double>& x) {

    for (std::vector<double>::const_iterator i=x.begin(); i!=x.end(); i++)
        if ((*i) < 0.0)
            throw (RbException("Cannot set simplex with negative value"));
    
    double sum = 0.0;
    for (size_t i=0; i<x.size(); i++)
        sum += x[i];
    
    elements.clear();
    for (size_t i=0; i<x.size(); i++)    
        elements.push_back(new RealPos(x[i]/sum));
    
    // set the number of elements
    length = elements.size();
}

