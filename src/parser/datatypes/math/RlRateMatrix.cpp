/**
 * @file
 * This file contains the implementation of RlRateMatrix, which is
 * class that holds a rate matrix in RevBayes.
 *
 * @brief Implementation of RlRateMatrix
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package distributions
 *
 * $Id$
 */

#include "RbBoolean.h"
#include "EigenSystem.h"
#include "Matrix.h"
#include "MemberFunction.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RlRateMatrix.h"
#include "Simplex.h"
#include "TransitionProbabilityMatrix.h"
#include "Workspace.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>


/** Constructor passes member rules and method inits to base class */
RlRateMatrix::RlRateMatrix(void) : MemberObject(getMemberRules()), rm(2) {
    
}


/** Constructor passes member rules and method inits to base class */
RlRateMatrix::~RlRateMatrix(void) {
    
}


RlRateMatrix& RlRateMatrix::operator=(const RlRateMatrix &r) {
    
    if (this != &r) {
        MemberObject::operator=(r);
        
        rm = r.rm;
    }
    
    return *this;
}


/** Clone object */
RlRateMatrix* RlRateMatrix::clone(void) const {
    
    return new RlRateMatrix(*this);
}


/** Map calls to member methods */
RbPtr<RbLanguageObject> RlRateMatrix::executeSimpleMethod(std::string const &name, const std::vector<const RbObject *> &args) {
    
    if (name == "nstates") {
        return new Natural( rm.getNumberOfStates() );
    }
    else if (name == "stationaryfreqs") {
        return new Simplex( rm.getStationaryFrequencies() );
    }
    else if (name == "averate") {
        return new RealPos( rm.averageRate() );
    }        
    else if (name == "reversible") {
        return new RbBoolean( rm.isTimeReversible() );
    }        
    
    return MemberObject::executeSimpleMethod( name, args );
}


/** Get class name of object */
const std::string& RlRateMatrix::getClassName(void) { 
    
    static std::string rbClassName = "Rate matrix";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RlRateMatrix::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


RbValue<void*> RlRateMatrix::getLeanValue( void ) const {
    RbValue<void*> tmp;
    tmp.value = static_cast<void*>( &const_cast<RateMatrix&>( rm ) );
    return tmp;
}

/** Get type spec */
const TypeSpec& RlRateMatrix::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get member rules */
const MemberRules& RlRateMatrix::getMemberRules(void) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) 
    {
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get methods */
const MethodTable& RlRateMatrix::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static ArgumentRules* nstatesArgRules = new ArgumentRules();
    static ArgumentRules* stationaryfreqsArgRules = new ArgumentRules();
    static ArgumentRules* averateArgRules = new ArgumentRules();
    static ArgumentRules* reversibleArgRules = new ArgumentRules();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) 
    {
        
        methods.addFunction("nstates",         new SimpleMemberFunction(Natural::getClassTypeSpec(), nstatesArgRules)         );
        methods.addFunction("stationaryfreqs", new SimpleMemberFunction(Simplex::getClassTypeSpec(), stationaryfreqsArgRules) );
        methods.addFunction("averate",         new SimpleMemberFunction(RealPos::getClassTypeSpec(), averateArgRules)         );
        methods.addFunction("reversible",      new SimpleMemberFunction(RbBoolean::getClassTypeSpec(), reversibleArgRules)    );
        
        // necessary call for proper inheritance
        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}


size_t RlRateMatrix::memorySize( void ) const {
    return sizeof( RateMatrix );
}


/** Print value for user */
void RlRateMatrix::printValue(std::ostream& o) const {
    
    o << "Rate matrix:" << std::endl;
//    theRateMatrix->printValue(o);
    o << std::endl;
    o << "Stationary frequencies: ";
//    theStationaryFreqs->printValue(o);
    o << std::endl;
}
