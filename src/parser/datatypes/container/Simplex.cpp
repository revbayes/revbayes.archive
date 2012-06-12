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
#include "RbUtil.h"
#include "Real.h"
#include "Simplex.h"
#include <iomanip>


/** Construct simplex of length (size) n */
Simplex::Simplex(const size_t n) : MemberObject() {
    
    double value = 1.0 / n;
    for (size_t i = 0; i < n; i++) {
        elements.push_back( value );
    }

}


/** Construct simplex from STL vector */
Simplex::Simplex(const std::vector<double>& x) : MemberObject() {

    elements = x;
    
    // rescale so that the elements sum to 1
    rescale();
}




/** Construct simplex from STL vector */
Simplex::Simplex(const RlVector<RealPos>& x) : MemberObject() {
    
    std::vector<double> tmp;
    
    for (size_t i = 0; i < x.size(); ++i) {
        tmp.push_back( x[i] );
    }
    
    elements = tmp;
    
    // rescale so that the elements sum to 1
    rescale();
}


/** Const subscript operator allowing caller to see value but not to modify it */
double Simplex::operator[](size_t i) {
    
    return elements[i];
}

/** Const subscript operator allowing caller to see value but not to modify it */
double Simplex::operator[](size_t i) const {

    return elements[i];
}


/** Clone function */
Simplex* Simplex::clone() const {

    return new Simplex(*this);
}


/** Get class name of object */
const std::string& Simplex::getClassName(void) { 
    
    static std::string rbClassName = "Simplex";
    
	return rbClassName; 
}


/* Get the stl vector */
const std::vector<double>& Simplex::getValue( void ) const {
    return elements;
}

/* Get class type spec describing type of object */
const TypeSpec& Simplex::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


RbValue<void*> Simplex::getLeanValue( void ) const {
    RbValue<void*> tmp;
    tmp.value = static_cast<void*>( &const_cast<std::vector<double>&>( elements ) );
    return tmp;
}

/* Get type spec */
const TypeSpec& Simplex::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


size_t Simplex::memorySize() const {
    return sizeof( std::vector<double> );
}


/* Print value for user */
void Simplex::printValue(std::ostream& o) const {
    
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << "[ ";
    o << std::fixed;
    o << std::setprecision(4);
    for (std::vector<double>::const_iterator i = elements.begin(); i!= elements.end(); i++) 
    {
        if (i != elements.begin())
            o << ", ";
        o << (*i);
    }
    o <<  " ]";
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
}


/** Push back a value. This will automatically rescale the elements.*/



/** Rescale the simplex */
void Simplex::rescale(void) {

    // Rescale the simplex
    double sum = 0.0;
    for (size_t i=0; i<elements.size(); i++)
       sum += elements[i];

    for (size_t i=0; i<elements.size(); i++) {
        elements[i] = elements[i] / sum;
    }
    
}


void Simplex::setLeanValue(const RbValue<void *> &val) {
    elements = *static_cast<std::vector<double> *>( val.value );
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
        elements.push_back(x[i]/sum);
}


/** Get the size of the simplex (number of elements) */
size_t Simplex::size( void ) const {
    return elements.size();
}

