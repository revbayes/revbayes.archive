/**
 * @file
 * This file contains the partial implementation of MemberObject, an abstract
 * base class for complex objects with member methods and variables.
 *
 * @brief Partial implementation of MemberObject
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "ConstArgumentRule.h"
#include "Integer.h"
#include "MemberFunction.h"
#include "MemberObject.h"
#include "MethodTable.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbNullObject.h"
#include "RbString.h"
#include "RbUtil.h"
#include "UserInterface.h"
#include "VariableNode.h"


/** Constructor */
MemberObject::MemberObject() : RbLanguageObject() {
    
}



/** Constructor: we set member variables here from member rules */
MemberObject::MemberObject(const MemberRules& memberRules) : RbLanguageObject() {

}



/* Execute method. This method just delegate the call to executeOperationSimple and wraps the return value into
 * a constant node. If you don't want this, you have to overwrite this method.
 */
RbPtr<RbLanguageObject> MemberObject::executeOperation(std::string const &name, const std::vector<RbPtr<Argument> >& args) {
    
    // get the return value
    const RbPtr<RbLanguageObject>& value = executeOperationSimple(name, args);
  
    return value;
    
}


/** Map member method call to internal function call. This is used as an alternative mechanism to providing a complete
 *  RbFunction object to execute a member method call. We throw an error here to capture cases where this mechanism
 *  is used without the appropriate mapping to internal function calls being present. */
RbPtr<RbLanguageObject> MemberObject::executeOperationSimple(const std::string& name, const std::vector<RbPtr<Argument> >& args) {
    
    if (name == "get") {
        // get the member with give name
        const RbString *varName = static_cast<const RbString *>( (const RbLanguageObject *) args[0]->getVariable()->getValue() );
        
        // check if a member with that name exists
        if ( hasMember(*varName) ) {
            return getMember(*varName);
        }
        
        // there was no variable with the given name
        return NULL;
        
    }
    else {
        throw RbException( "No mapping from member method " + name + " to internal function call provided" );
    }
}


/** Get class name of object */
const std::string& MemberObject::getClassName(void) { 
    
    static std::string rbClassName = "Member object";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& MemberObject::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Return member rules (no members) */
const MemberRules& MemberObject::getMemberRules(void) const {
    
    throw RbException( "Object does not have members" );
}


/** Get type specification for a member variable */
//const TypeSpec& MemberObject::getMemberTypeSpec(const std::string& name) const {
//    return (*members)[name].getTypeSpec();
//}


/** 
 * Get method specifications of all member objects. 
 * We support two methods:
 * 1) memberNames()
 * 2) get("name")
 */
const MethodTable& MemberObject::getMethods(void) const {

    static MethodTable methods = MethodTable();
    static bool        methodsSet = false;
    
    if ( methodsSet == false ) {
        
        ArgumentRules* getMemberNamesArgRules = new ArgumentRules();
        ArgumentRules* getArgRules = new ArgumentRules();
        
        // add the 'memberNames()' method
        methods.addFunction("memberNames", new MemberFunction(RbVoid_name, getMemberNamesArgRules) );
        
        // add the 'memberNames()' method
        getArgRules->push_back( new ConstArgumentRule( "name" , RbString::getClassTypeSpec() ) );
        methods.addFunction("get", new MemberFunction(RbLanguageObject::getClassTypeSpec(), getArgRules) );
        
        methodsSet = true;
    }   
    
    return methods;
}


/** Get a member variable */
RbPtr<RbLanguageObject> MemberObject::getMember(const std::string& name) const {
    
    throw RbException("No Member named '" + name + "' available.");
}

/** Does this object have a member called "name" */
bool MemberObject::hasMember(std::string const &name) const {
    return false;
}


/** Print value for user */
void MemberObject::printValue(std::ostream& o) const {
    
    o << "Printing MemberObject:" << std::endl;

}


/** Set a member DAG node */
void MemberObject::setMemberVariable(const std::string& name, const RbPtr<RbLanguageObject> &var) {

    throw RbException("No Member named '" + name + "' expected in an object of class " + getTypeSpec().getBaseType() + " and therefore cannot set it.");
}


/** Set a member variable */
void MemberObject::setMember(const std::string& name, const RbPtr<const Variable> &var) {
    // calling the internal mthod to set the DAG node
    // the derived classes should know how to set their members
    setMemberVariable(name, var->getValue()->clone() );
    
}



