//
//  RlModelTrace.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 3/27/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "RlModelTrace.h"

#include "ArgumentRules.h"
#include "MemberProcedure.h"
#include "MethodTable.h"
#include "Natural.h"
#include "RlUtils.h"

using namespace RevLanguage;

ModelTrace::ModelTrace() : WorkspaceToCoreWrapperObject<RevBayesCore::ModelTrace>()
{
    
}


ModelTrace::ModelTrace(const RevBayesCore::ModelTrace &t) : WorkspaceToCoreWrapperObject<RevBayesCore::ModelTrace>( new RevBayesCore::ModelTrace( t ) )
{
    
}


/** Clone object */
ModelTrace* ModelTrace::clone(void) const
{
    
    return new ModelTrace(*this);
}


void ModelTrace::constructInternalObject( void )
{
    throw RbException("We do not support a constructor function for ModelTrace.");
}


/* Map calls to member methods */
RevPtr<RevVariable> ModelTrace::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    return RevObject::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& ModelTrace::getClassType(void) {
    
    static std::string revType = "ModelTrace";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& ModelTrace::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::ModelTrace>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& ModelTrace::getParameterRules(void) const {
    
    static MemberRules modelMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        rulesSet = true;
    }
    
    return modelMemberRules;
}


/** Get type spec */
const TypeSpec& ModelTrace::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void ModelTrace::printValue(std::ostream &o) const {
    
    o << "Filename:                  " << value->getFileName() << std::endl;
    o << "Parameter:                 " << value->getParameterName() << std::endl;
    o << "Samples:                   " << value->getSamples() << std::endl;
    o << "Stepsize:                  " << value->getStepSize() << std::endl;
    //    o << "Heidelberger-Welch-test:   " << (value->hasPassedHeidelbergerWelchTest() ? "TRUE" : "FALSE") << std::endl;
}


/** Set a member variable */
void ModelTrace::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "xxx") {
        
    }
    else {
        RevObject::setConstParameter(name, var);
    }
}


std::ostream& RevLanguage::operator<<(std::ostream& o, const RevLanguage::ModelTrace& x) {
    //    o << x.getParameterName();
    o << "ModelTrace (";
    //    const std::vector<double>& values = x.getValues();
    //    for (std::vector<double>::const_iterator it = values.begin(); it != values.end(); ++it) {
    //        if ( it != values.begin() ) {
    //            o << ", ";
    //        }
    //        o << *it;
    //    }
    o << ")";
    
    return o;
}
