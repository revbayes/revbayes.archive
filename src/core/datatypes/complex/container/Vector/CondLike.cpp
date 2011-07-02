/**
 * @file
 * This file contains the implementation of CondLike, used to 
 * hold conditional likelihoods
 *
 * @brief Implementation of CondLike
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

#include "CondLike.h"
#include "RbException.h"
#include "RbNames.h"
#include <sstream>
#include <string>



/** Construct an empty cond like vector */
CondLike::CondLike(size_t nc, size_t ns) : MemberObject(getMemberRules()) {

    numChars  = nc;
    numStates = ns;
    clArray   = std::valarray<double>(0.0, numChars*numStates);
}


/** Copy constructor */
CondLike::CondLike(const CondLike& x) : MemberObject(getMemberRules()) {

    numChars  = x.numChars;
    numStates = x.numStates;
    clArray   = x.clArray;
}


/** Subscript operator */
double& CondLike::operator[](size_t i) {

    if (i >= clArray.size())
        throw RbException("Index out of bounds");
    return clArray[i];
}


/** Subscript const operator */
const double& CondLike::operator[](size_t i) const {

    if (i >= clArray.size())
        throw RbException("Index out of bounds");
    return clArray[i];
}


/** Overloaded assignment operator */
CondLike& CondLike::operator=(const CondLike& x) {

	if (this != &x)
		{
        numChars  = x.numChars;
        numStates = x.numStates;
        clArray   = x.clArray;
		}
	return *this;
}


CondLike operator*(CondLike& a, CondLike& b) {

    if ( a.getNumChars() == b.getNumChars() && a.getNumStates() == b.getNumStates() )
        {
        size_t nc = a.getNumChars();
        size_t ns = a.getNumStates();
        CondLike res(nc, ns);
        double* clL = &a[0];
        double* clR = &b[0];
        double* clP = &res[0];
        for (size_t c=0; c<nc; c++)
            {
            for (size_t i=0; i<ns; i++)
                clP[i] = clL[i] * clR[i];
            clL += ns;
            clR += ns;
            clP += ns;
            }
        return res;
        }
    return a;
}


CondLike &operator*=(CondLike& a, CondLike& b) {

    if ( a.getNumChars() == b.getNumChars() && a.getNumStates() == b.getNumStates() )
        {
        size_t nc = a.getNumChars();
        size_t ns = a.getNumStates();
        CondLike res(nc, ns);
        double* clP = &a[0];
        double* clD = &b[0];
        for (size_t c=0; c<nc; c++)
            {
            for (size_t i=0; i<ns; i++)
                clP[i] *= clD[i];
            clP += ns;
            clD += ns;
            }
        }
	return a;
}


/** Clone object */
CondLike* CondLike::clone(void) const {

    return new CondLike(*this);
}


/** Get class */
const VectorString& CondLike::getClass(void) const {

    static VectorString rbClass = VectorString(CondLike_name) + MemberObject::getClass();
    return rbClass;
}


/** Print value for user */
void CondLike::printValue(std::ostream& o) const {

    for (size_t i=0; i<clArray.size(); i++)
        o << clArray[i] << " ";
    o << std::endl;
}


/** Complete info about object */
std::string CondLike::richInfo(void) const {

    std::ostringstream o;
    o <<  "Conditional Likelihoods: ";
    printValue( o );
    return o.str();
}


