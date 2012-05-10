/**
 * @file
 * This file contains the implementation of ParserTransitionProbablityMatrix, a complex type
 * used to hold a transition probability matrix.
 *
 * This object simply represent the wrapper for the parser for a TransitionProbabilityMatrix.
 *
 * @brief Implementation of ParserTransitionProbabilityMatrix
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-09 10:40:24 +0200 (Wed, 09 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: ParserTransitionProbabilityMatrix.cpp 1493 2012-05-09 08:40:24Z hoehna $
 */


#include "ParserTransitionProbabilityMatrix.h"

#include "RbException.h"
#include "RbUtil.h"
#include "TransitionProbabilityMatrix.h"

#include <iostream>
#include <iomanip>


/* Constructor */
ParserTransitionProbabilityMatrix::ParserTransitionProbabilityMatrix(void) : MemberObject( getMemberRules() ) {
    
}



/* Clone function */
ParserTransitionProbabilityMatrix* ParserTransitionProbabilityMatrix::clone() const {
    
    return new ParserTransitionProbabilityMatrix(*this);
}


/* Get class name of object */
const std::string& ParserTransitionProbabilityMatrix::getClassName(void) { 
    
    static std::string rbClassName = "Parser object of a transition probability matrix";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ParserTransitionProbabilityMatrix::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& ParserTransitionProbabilityMatrix::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Print value for user */
void ParserTransitionProbabilityMatrix::printValue(std::ostream& o) const {
    
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << "[ ";
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < m.size(); i++) {
        if (i == 0)
            o << "[ ";
        else 
            o << "  ";
        
        for (std::vector<double>::const_iterator j = m[i].begin(); j != m[i].end(); ++j) 
        {
            if (j != m[i].begin())
                o << ", ";
            o << (*j);
        }
        o <<  " ]";
        
        if (i == m.size()-1)
            o << " ]";
        else 
            o << " ,\n";
        
        //RBOUT(lineStr);
    }
    o <<  " ]";
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
}



