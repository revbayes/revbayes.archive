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
#include "Integer.h"
#include "MemberFunction.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbUtil.h"
#include "UserInterface.h"
#include "VectorString.h"
#include "ValueRule.h"
#include "VariableNode.h"


/** Constructor */
MemberObject::MemberObject() : RbLanguageObject() {
    
    members = RbPtr<Environment>( new Environment() );
    
}



/** Constructor: we set member variables here from member rules */
MemberObject::MemberObject(const RbPtr<MemberRules> memberRules) : RbLanguageObject() {

    members = RbPtr<Environment>( new Environment() );
    
    /* Fill member table (frame) based on member rules */
    for ( MemberRules::const_iterator i = memberRules->begin(); i != memberRules->end(); i++ ) {
        // creare variable slots with name and type
        members->addVariable( (*i)->getArgumentLabel(), (*i)->getArgumentTypeSpec() );
    }
}




/** Copy constructor */
MemberObject::MemberObject(const MemberObject &m) : RbLanguageObject() {
    
    // copy the members
    members = RbPtr<Environment>( m.members->clone() );
}


/** Execute member method: delegate to method table. */
RbPtr<RbLanguageObject> MemberObject::executeMethod(const std::string& name, const std::vector<RbPtr<Argument> >& args) {

    return getMethods()->executeFunction(name, args);
}


/** Map member method call to internal function call. This is used as an alternative mechanism to providing a complete
 *  RbFunction object to execute a member method call. We throw an error here to capture cases where this mechanism
 *  is used without the appropriate mapping to internal function calls being present. */
RbPtr<RbLanguageObject> MemberObject::executeOperation(const std::string& name, Environment& args) {
    
    if (name == "memberNames") {
        for (size_t i=0; i<members->size(); i++) {
            RBOUT(members->getName(i));
        }
        
        return RbPtr<RbLanguageObject>::getNullPtr();
    } 
    else if (name == "get") {
        // get the member with give name
        const RbPtr<RbString> varName( static_cast<RbString*>(args[0]->getValue().get()) );
        
        // check if a member with that name exists
        if (members->existsVariable(*varName)) {
            return (*members)[*varName]->getDagNodePtr()->getValuePtr();
        }
        
        // there was no variable with the given name
        return RbPtr<RbLanguageObject>::getNullPtr();
        
    }
    else {
        throw RbException( "No mapping from member method " + name + " to internal function call provided" );
    }
}


/** Get class vector describing type of object */
const VectorString& MemberObject::getClass(void) const {

    static VectorString rbClass = VectorString(MemberObject_name) + RbLanguageObject::getClass();
    return rbClass;
}


/** Return member rules (no members) */
const RbPtr<MemberRules> MemberObject::getMemberRules(void) const {

    throw RbException( "Object does not have members" );
}


/** Get type specification for a member variable */
const TypeSpec MemberObject::getMemberTypeSpec(const std::string& name) const {
    return (*members)[name]->getTypeSpec();
}


/** Get method specifications (no methods) */
const RbPtr<MethodTable> MemberObject::getMethods(void) const {

    static RbPtr<MethodTable> methods( new MethodTable() );
    static RbPtr<ArgumentRules> getMemberNamesArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> getArgRules( new ArgumentRules() );
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) {
        
        // add the 'memberNames()' method
        methods->addFunction("memberNames", RbPtr<RbFunction>( new MemberFunction(RbVoid_name, getMemberNamesArgRules) ) );
        
        // add the 'memberNames()' method
        getArgRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "name" , RbString_name ) ) );
        methods->addFunction("get", RbPtr<RbFunction>( new MemberFunction(RbLanguageObject_name, getArgRules) ) );
        
        methodsSet = true;
    }   
    
    return methods;
}


const RbPtr<Environment> MemberObject::getMembers(void) const {
    return members;
}


RbPtr<Environment> MemberObject::getMembersPtr(void) {
    return members;
}



/** Get const value of a member variable */
const RbPtr<RbLanguageObject> MemberObject::getMemberValue(const std::string& name) const {

    return members->getValue(name);
}


/** Get a member variable */
const RbPtr<DAGNode> MemberObject::getMemberDagNode(const std::string& name) const {

    return members->getDagNode(name);
}


/** Get a member variable (non-const, for derived classes) */
RbPtr<DAGNode> MemberObject::getMemberDagNodePtr(const std::string& name) {

    return members->getDagNodePtr(name);
}

/** Does this object have a member called "name" */
bool MemberObject::hasMember(std::string const &name) const {
    return members->existsVariable( name );
}


/** Is the object constant? */
bool MemberObject::isConstant( void ) const {

    return true;
}


/** Print value for user */
void MemberObject::printValue(std::ostream& o) const {
    
    o << "Printing members of MemberObject ...." << std::endl;

    for ( size_t i = 0; i < members->size(); i++ ) {

        o << "." << members->getName(i) << std::endl;
        if ( (*members)[members->getName(i)]->getValue() == NULL)
            o << "NULL";
        else
            (*members)[members->getName(i)]->getValue()->printValue(o);
        o << std::endl << std::endl;
    }
}


/** Complete info about object */
std::string MemberObject::richInfo(void) const {

    std::ostringstream o;
    o << getType() << ":" << std::endl;
    printValue(o);

    return o.str();
}


/** Set a member DAG node */
void MemberObject::setMemberDagNode(const std::string& name, RbPtr<DAGNode> var) {
    
    (*members)[name]->getVariable()->setDagNode(var);
}


/** Set a member variable */
void MemberObject::setMemberVariable(const std::string& name, RbPtr<Variable> var) {

    (*members)[name]->setVariable(var);
}



