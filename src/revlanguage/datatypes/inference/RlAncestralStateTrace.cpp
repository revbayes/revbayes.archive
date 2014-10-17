/*
 *  RlAncestralStateTrace.cpp
 *  revbayes-proj
 *
 *  Created by mucho mass on 10/16/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "RlAncestralStateTrace.h"
#include "AncestralStateTrace.h"

#include "ArgumentRules.h"
#include "MemberProcedure.h"
#include "MethodTable.h"
#include "Natural.h"
#include "RlUtils.h"

using namespace RevLanguage;

AncestralStateTrace::AncestralStateTrace() : WorkspaceObject<RevBayesCore::AncestralStateTrace>() {
    
}



AncestralStateTrace::AncestralStateTrace(const RevBayesCore::AncestralStateTrace& m) : WorkspaceObject<RevBayesCore::AncestralStateTrace>( new RevBayesCore::AncestralStateTrace( m ) ) {
    
}

AncestralStateTrace::AncestralStateTrace(const AncestralStateTrace& m) : WorkspaceObject<RevBayesCore::AncestralStateTrace>( m ) {
    
}

/** Clone object */

AncestralStateTrace* AncestralStateTrace::clone(void) const {
    
	return new AncestralStateTrace(*this);
}



void AncestralStateTrace::constructInternalObject( void ) {
    throw RbException("We do not support a constructor function for AncestralStateTrace.");
}


/* Map calls to member methods */

RevPtr<Variable> AncestralStateTrace::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    
    return RevObject::executeMethod( name, args );
}


/** Get Rev type of object */

const std::string& AncestralStateTrace::getClassType(void) { 
    
    static std::string revType = "AncestralStateTrace";
    
	return revType; 
}

/** Get class type spec describing type of object */

const TypeSpec& AncestralStateTrace::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceObject<RevBayesCore::AncestralStateTrace>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */

const MemberRules& AncestralStateTrace::getMemberRules(void) const {
    
    static MemberRules modelMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {        
        rulesSet = true;
    }
    
    return modelMemberRules;
}


/* Get method specifications */

const MethodTable& AncestralStateTrace::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false )
    {
        
        // necessary call for proper inheritance
        methods.setParentTable( &RevObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}

/** Get type spec */

const TypeSpec& AncestralStateTrace::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */

void AncestralStateTrace::printValue(std::ostream &o) const {
    
    o << "AncestralStateTrace";
}


/** Set a member variable */

void AncestralStateTrace::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "xxx") {
        
    } 
    else {
        RevObject::setConstMemberVariable(name, var);
    }
}