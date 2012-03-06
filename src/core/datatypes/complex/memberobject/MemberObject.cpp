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
#include "RbNullObject.h"
#include "RbUtil.h"
#include "UserInterface.h"
#include "VectorString.h"
#include "ValueRule.h"
#include "VariableNode.h"


/** Constructor */
MemberObject::MemberObject() : RbLanguageObject() {
    
}



/** Constructor: we set member variables here from member rules */
MemberObject::MemberObject(const MemberRules& memberRules) : RbLanguageObject() {

}




/** Copy constructor */
MemberObject::MemberObject(const MemberObject &m) : RbLanguageObject() {
    
    // copy the members.
    // Note, this is a shallow copy because the members are pointers to variables which itself are pointers to DAG nodes.
    members = m.members;
//    for (std::map<std::string, RbVariablePtr>::const_iterator i = m.members.begin(); i != m.members.end(); i++) {
//        setMember(i->first, i->second);
//    }
}


/** Destructor. */
MemberObject::~MemberObject(void) {

}


/** Assignment operator. We need to make a deep copy of the members */
MemberObject& MemberObject::operator=(const MemberObject &m) {
    
    if (this != &m) {
          
        members = m.members;
    }
    
    return (*this);
}


/** Execute member method: delegate to method table. */
const RbLanguageObject& MemberObject::executeMethod(const std::string& name, const std::vector<Argument>& args) {
    // TODO: We shouldn't allow const casts!!!
    MethodTable& mt = const_cast<MethodTable&>( getMethods() );
    return mt.executeFunction(name, args);
}


/* Execute method. This method just delegate the call to executeOperationSimple and wraps the return value into
 * a constant node. If you don't want this, you have to overwrite this method.
 */
const RbLanguageObject& MemberObject::executeOperation(std::string const &name, const std::vector<Argument>& args) {
    
    // get the return value
    const RbLanguageObject& value = executeOperationSimple(name, args);
  
    return value;
    
//    // wrap into constant node
//    DAGNode* theNode = new ConstantNode( value );
//    
//    return theNode;
}


/** Map member method call to internal function call. This is used as an alternative mechanism to providing a complete
 *  RbFunction object to execute a member method call. We throw an error here to capture cases where this mechanism
 *  is used without the appropriate mapping to internal function calls being present. */
const RbLanguageObject& MemberObject::executeOperationSimple(const std::string& name, const std::vector<Argument>& args) {
    
    if (name == "memberNames") {
//        for (size_t i=0; i<members->size(); i++) {
//            RBOUT(members->getName(i));
//        }
        
        return RbNullObject::getInstance();
    } 
    else if (name == "get") {
        // get the member with give name
        const RbString& varName = static_cast<const RbString&>( args[0].getVariable().getValue() );
        
        // check if a member with that name exists
        if (hasMember(varName)) {
            return getMember(varName)->getValue();
        }
        
        // there was no variable with the given name
        return RbNullObject::getInstance();
        
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


/** Get method specifications (no methods) */
const MethodTable& MemberObject::getMethods(void) const {

    static MethodTable methods = MethodTable();
    static bool        methodsSet = false;
    
    if ( methodsSet == false ) {
        
        ArgumentRules* getMemberNamesArgRules = new ArgumentRules();
        ArgumentRules* getArgRules = new ArgumentRules();
        
        // add the 'memberNames()' method
        methods.addFunction("memberNames", new MemberFunction(RbVoid_name, getMemberNamesArgRules) );
        
        // add the 'memberNames()' method
        getArgRules->push_back( new ValueRule( "name" , RbString::getClassTypeSpec() ) );
        methods.addFunction("get", new MemberFunction(RbLanguageObject::getClassTypeSpec(), getArgRules) );
        
        methodsSet = true;
    }   
    
    return methods;
}


const std::map<std::string, RbVariablePtr>& MemberObject::getMembers(void) const {
    return members;
}


std::map<std::string, RbVariablePtr>& MemberObject::getMembers(void) {
    return members;
}


/** Get a member variable */
const Variable* MemberObject::getMember(const std::string& name) const {
    
    std::map<std::string, RbVariablePtr>::const_iterator i = members.find( name );
    if ( i != members.end()) {
        return i->second;
    }
    
    throw RbException("No Member named '" + name + "' available.");
}


/** Get a member variable (non-const, for derived classes) */
Variable* MemberObject::getMember(const std::string& name) {
    
    std::map<std::string, RbVariablePtr>::iterator i = members.find( name );
    if ( i != members.end()) {
        return i->second;
    }
    
    throw RbException("No Member named '" + name + "' available.");
}

/** Does this object have a member called "name" */
bool MemberObject::hasMember(std::string const &name) const {
    return false;
}


/** Is the object constant? */
bool MemberObject::isConstant( void ) const {

    return true;
}


/** Print value for user */
void MemberObject::printValue(std::ostream& o) const {
    
    o << "Printing members of MemberObject ...." << std::endl;

    for ( std::map<std::string, RbVariablePtr>::const_iterator i = members.begin(); i != members.end(); i++ ) {

        o << "." << i->first << std::endl;
        i->second->getValue().printValue(o);
        o << std::endl << std::endl;
    }
}


/** Set a member DAG node */
void MemberObject::setMemberVariable(const std::string& name, Variable* var) {

    throw RbException("No Member named '" + name + "' expected and therefore cannot set it.");
}


/** Set a member variable */
void MemberObject::setMember(const std::string& name, Variable* var) {
    // calling the internal mthod to set the DAG node
    // the derived classes should know how to set their members
    setMemberVariable(name, var);
    
    // test whether this element already was inserted
    std::map<std::string, RbVariablePtr>::iterator existingElement = members.find(name);
    if ( members.count( name ) > 0) {
        members[name] = var;
        if ( members.count( name ) > 1) {
            std::cerr << "Opsi dupsi" << std::endl;
        }
    }
    else {
        // just add this node to the map
        members.insert( std::pair<std::string, RbVariablePtr>(name,var) );
    }
}



