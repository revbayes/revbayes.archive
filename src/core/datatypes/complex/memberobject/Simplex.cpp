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
 * $Id:$
 */


#include "ConstantNode.h"
#include "MemberNode.h"
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
Simplex::Simplex(const size_t n) : MemberObject() {

    if (n < 2)
        throw RbException( "Simplex must have at least two elements" );

    for (size_t i=0; i<n; i++)
        value.push_back(1.0/n);
}


/** Construct simplex from STL vector */
Simplex::Simplex(const std::vector<double>& x) : MemberObject(getMemberRules()) {

    if (x.size() < 2)
        throw RbException( "Simplex must have at least two elements" );

    for (size_t i=0; i<x.size(); i++)
        if (x[i] <= 0.0)
            throw (RbException("Cannot set simplex from nonpositive value"));

    value = x;
    rescale();
}


/** Construct simplex from VectorRealPos, which is guaranteed to have real positive numbers */
Simplex::Simplex(const VectorRealPos& x) : MemberObject(getMemberRules()) {

    for (size_t i=0; i<x.size(); i++)
        if (x[i] <= 0.0)
            throw (RbException("Cannot set simplex from nonpositive value"));

    value = x.getValue();
    rescale();
}


/** Const subscript operator allowing caller to see value but not to modify it */
double Simplex::operator[](size_t i) const {

    if ( i >= value.size()  )
        throw RbException( "Index out of bound" );

    return value[i];
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


/**
 * Get subscript element for parser. By giving back a temp variable rather than
 * a reference, we ensure that the parser cannot set the element
 */
DAGNode* Simplex::getElement( VectorInteger& index ) const {

    if ( index.size() != 1 )
        throw RbException( "Wrong dimensions of index for " + Simplex_name );
    if ( index[0] > int( value.size() ) )
        throw RbException( "Index out of bounds for " + Simplex_name );

    if ( index[0] < 0 )
        return new MemberNode( this->clone() );

    else
        return new ConstantNode( new RealPos( value[index[0]] ) );
}


/** Get member rules */
const MemberRules& Simplex::getMemberRules(void) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) 
        {
        rulesSet = true;
        }

    return memberRules;
}


/** Print value for user */
void Simplex::printValue(std::ostream& o) const {

    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();

    o << "[ ";
    o << std::fixed;
    o << std::setprecision(3);
    for (std::vector<double>::const_iterator i = value.begin(); i!= value.end(); i++) 
        {
        if (i != value.begin())
            o << ", ";
        o << (*i);
        }
    o <<  " ]";

    o.setf(previousFlags);
    o.precision(previousPrecision);
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


/** Complete info about object */
std::string Simplex::richInfo(void) const {

    std::ostringstream o;
    printValue(o);
    return o.str();
}


/** Set value of simplex using VectorReal */
void Simplex::setValue(const VectorReal& x) {

    value.resize(x.size());
    for (size_t i=0; i<x.size(); i++)    
        value[i] = x[i];
    rescale();
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

