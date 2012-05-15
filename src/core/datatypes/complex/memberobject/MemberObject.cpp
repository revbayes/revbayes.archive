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
#include "MemberObject.h"
#include "MethodTable.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbNullObject.h"
#include "RbString.h"
#include "RbUtil.h"
#include "SimpleMemberFunction.h"
#include "UserInterface.h"
#include "VariableNode.h"


/** Constructor */
MemberObject::MemberObject() : RbLanguageObject() {
    
}



/** Constructor: we set member variables here from member rules */
MemberObject::MemberObject(const MemberRules& memberRules) : RbLanguageObject() {

}


/** Execute method. 
 * This is the deault implemention. You should overwrite this method only if you want to treat the multidimensional data type RlValue
 * yourselve.
 *
 * We assumes that the arguments come as scalar types, not multidimensional RevLanguage objects.
 * Therefore, we just delegate the call to executeSimpleMethod.
 */
RbPtr<RbLanguageObject> MemberObject::executeMethod(std::string const &name, const std::vector<RlValue<const RbObject> > &args) {
    
    std::vector<const RbObject *> simpleArgs;
    for (std::vector<RlValue<const RbObject> >::const_iterator i =args.begin(); i != args.end(); ++i) {
        // check first if these arguments are actually scalars
        if (i->lengths.size() > 0 ) {
            throw RbException("We currently do not support setting of multidimensional member values!");
        }
        simpleArgs.push_back( i->getSingleValue() );
    }
    
    // calling the internal method to execute the member-method
    return executeSimpleMethod(name, simpleArgs );
}



/* Execute simple method. This method assumes that the arguments come as scalar types, not multidimensional RevLanguage objects.
 * Therefore, it should be OK if we just get an RbObject argument instead of an RlValue.
 */
RbPtr<RbLanguageObject> MemberObject::executeSimpleMethod(std::string const &name, const std::vector<const RbObject*>& args) {
    
    if (name == "get") {
        // get the member with give name
        const RbString *varName = static_cast<const RbString *>( args[0] );
        
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
        methods.addFunction("memberNames", new SimpleMemberFunction(RbVoid_name, getMemberNamesArgRules) );
        
        // add the 'memberNames()' method
        getArgRules->push_back( new ConstArgumentRule( "name" , RbString::getClassTypeSpec() ) );
        methods.addFunction("get", new SimpleMemberFunction(RbLanguageObject::getClassTypeSpec(), getArgRules) );
        
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


/** Set a member variable */
void MemberObject::setConstMember(const std::string& name, const RbPtr<const Variable> &var) {
    
    // here, we might want to do some general stuff like catching all members so that we can provide general functions as
    // 1) getNames()
    // 2) getMember(name)
    
    setConstMemberVariable(name, var );
    
}


/** Set a member variable */
void MemberObject::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if (var->getValue().lengths.size() > 0 ) {
        throw RbException("We currently do not support setting of multidimensional member values!");
    }
    
    // calling the internal mthod to set the DAG node
    // the derived classes should know how to set their members
    setSimpleMemberValue(name, var->getValue().getSingleValue() );
    
}


/* Set a member variable.
 * In this default implementation, we delegate to setConstMemberVariable.
 * Derived classes of MemberObject who need non-const variable should overwrite this function.
 * If you don't care if the variable is const, then you should only overwrite the setConstMemberVariable.
 */
void MemberObject::setMember(const std::string& name, const RbPtr<Variable> &var) {
    
    // here, we might want to do some general stuff like catching all members so that we can provide general functions as
    // 1) getNames()
    // 2) getMember(name)
    
    setMemberVariable(name, var );
    
}


/* Set a member variable.
 * In this default implementation, we delegate to setConstMemberVariable.
 * Derived classes of MemberObject who need non-const variable should overwrite this function.
 * If you don't care if the variable is const, then you should only overwrite the setConstMemberVariable.
 */
void MemberObject::setMemberVariable(const std::string& name, const RbPtr<Variable> &var) {
    
    setConstMemberVariable(name, RbPtr<const Variable>( var ) );
    
}





/** Set a member DAG node */
void MemberObject::setSimpleMemberValue(const std::string& name, const RbPtr<const RbLanguageObject> &var) {
    
    throw RbException("No Member named '" + name + "' expected in an object of class " + getTypeSpec().getBaseType() + " and therefore cannot set it.");
}



