/**
 * @file
 * This file contains the implementation of some of the functions
 * in the abstract base class for language objects, RbLanguageObject.
 *
 * @brief Partial implementation of RbLanguageObject
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-02, version 1.0
 * @extends RbLanguageObject
 *
 * $Id$
 */

#include "MemberFunction.h"
#include "MethodTable.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbLanguageObject.h"
#include "RlUtils.h"
#include "Vector.h"
#include "RlString.h"
#include "TypeSpec.h"
#include "Workspace.h"

#include <sstream>

using namespace RevLanguage;




/** Constructor: we set member variables here from member rules */
RbLanguageObject::RbLanguageObject(const MemberRules& memberRules) {
    
}


/** 
 * Destructor. 
 * Nothing to do here because we don't own anything.
 */
RbLanguageObject::~RbLanguageObject( void )
{
    
}


/**
 * Addition operation.
 * Since we don't know the types and thus don't know the special behavior we simply throw and error.
 *
 * \param[in]   rhs     The right hand side value of the division operation.
 *
 * \return              NULL
 */
RbLanguageObject* RbLanguageObject::add(const RbLanguageObject &rhs) const
{
    throw RbException("Cannot add a value of type '" + this->getType() + "' to a value of type '" + rhs.getType() + "'.");
    
    return NULL;
}

/** The default implementation does nothing because we don't have an internal object */
void RbLanguageObject::constructInternalObject( void ) {
    // nothing to do
}



/** Convert to type and dim. The caller manages the returned object. */
RbLanguageObject* RbLanguageObject::convertTo(const TypeSpec& typeSpec) const {
        
    throw RbException("Failed conversion from " + getTypeSpec() + " to " + typeSpec);
    
    return NULL;
}


/**
 * Make a new object that is an indirect deterministic reference to the object.
 * The default implementation throws an error.
 */
RbLanguageObject* RbLanguageObject::dagReference(void) {
    
    throw RbException( "Dynamic reference to this object type not supported");
}


/**
 * Decrement operation.
 * Since we don't know the types and thus don't know the special behavior we simply throw and error.
 */
void RbLanguageObject::decrement( void ) 
{
    throw RbException("Cannot decrement a value of type '" + this->getType() + "'.");

}


/**
 * Division operation.
 * Since we don't know the types and thus don't know the special behavior we simply throw and error.
 *
 * \param[in]   rhs     The right hand side value of the division operation.
 *
 * \return              NULL
 */
RbLanguageObject* RbLanguageObject::divide(const RbLanguageObject &rhs) const
{
    throw RbException("Cannot divide a value of type '" + this->getType() + "' to a value of type '" + rhs.getType() + "'.");
    
    return NULL;
}


/** 
 * Execute simple method. 
 */
RbLanguageObject* RbLanguageObject::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "get") 
    {
        // get the member with give name
        const std::string &varName = static_cast<const RlString &>( args[0].getVariable()->getValue() ).getValue();
        
        // check if a member with that name exists
        if ( hasMember( varName ) ) 
        {
            return getMember(varName);
        }
        
        // there was no variable with the given name
        return NULL;
        
    }
    else if ( name == "members" ) 
    {
        
        return NULL;
    }
    else if ( name == "methods" ) 
    {
        // just print the method names (including inherited methods)
        const MethodTable &m = getMethods();
        m.printValue(std::cout, true);
        
        return NULL;
    }
    else 
    {
        throw RbException( "No mapping from member method '" + name + "' to internal function call provided" );
    }
}


/** Get class vector describing type of object */
const std::string& RbLanguageObject::getClassName(void) { 
    
    static std::string rbClassName = "language object";
	return rbClassName; 
}


/** Get class vector describing type of object */
const TypeSpec& RbLanguageObject::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec(getClassName() );
	
    return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& RbLanguageObject::getMemberRules(void) const {
    
    static const MemberRules rules = MemberRules();
    
    return rules;
}


/** Get a member variable */
RbLanguageObject* RbLanguageObject::getMember(const std::string& name) const {
    
    throw RbException("No Member named '" + name + "' available.");
}


/** 
 * Get method specifications of all member objects. 
 * We support two methods:
 * 1) memberNames()
 * 2) get("name")
 */
const MethodTable& RbLanguageObject::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool        methodsSet = false;
    
    if ( methodsSet == false ) {
        
        ArgumentRules* getMembersArgRules = new ArgumentRules();
        ArgumentRules* getMethodsArgRules = new ArgumentRules();
        ArgumentRules* getArgRules = new ArgumentRules();
        
        // add the 'members()' method
        methods.addFunction("members", new MemberFunction(RlUtils::Void, getMembersArgRules) );
        
        // add the 'members()' method
        methods.addFunction("methods", new MemberFunction(RlUtils::Void, getMethodsArgRules) );
        
        // add the 'memberNames()' method
        getArgRules->push_back( new ArgumentRule( "name", true, RlString::getClassTypeSpec() ) );
        methods.addFunction("get", new MemberFunction(RbLanguageObject::getClassTypeSpec(), getArgRules) );
        
        methodsSet = true;
    }   
    
    return methods;
}


/**
 * Get the value as a DAG node. This default implementation throws an error.
 */
const std::string& RbLanguageObject::getType( void ) const {
    
    return getTypeSpec().getType();
}


/**
 * Get the value as a DAG node. This default implementation throws an error.
 */
RevBayesCore::DagNode* RbLanguageObject::getValueNode( void ) const {
    
    throw RbException("RevLanguage only objects cannot be used inside DAG's!");
    
    return NULL;
}


/** Does this object have a member called "name" */
bool RbLanguageObject::hasMember(std::string const &name) const {
    return false;
}


/**
 * Increment operation.
 * Since we don't know the types and thus don't know the special behavior we simply throw and error.
 */
void RbLanguageObject::increment( void ) 
{
    throw RbException("Cannot increment a value of type '" + this->getType() + "'.");
    
}


/**
 * The default implementation is that the variable is constant. Only variables which actually store
 * internally DAG nodes have to ask the DAG nodes if they are constant.
 */
bool RbLanguageObject::isConstant( void ) const {
    return true;
}


/** Is convertible to type? */
bool RbLanguageObject::isConvertibleTo(const TypeSpec& type) const {
    
    return false;
}


/* Are we of specified language type? */
bool RbLanguageObject::isTypeSpec(const TypeSpec& typeSpec) const {
    
    return getTypeSpec().isDerivedOf( typeSpec );
}


/**
  * Converting the value of the internal variable to a constant. The default implementation does nothing because we don't have a DAG node as our internal variable.
  * Note, RevLanguage types which can be used as types in the DAG should overwrite this method.
  */
void RbLanguageObject::makeConstantValue( void ) {
    // do nothing
}


/**
 * Multiplication operation.
 * Since we don't know the types and thus don't know the special behavior we simply throw and error.
 *
 * \param[in]   rhs     The right hand side value of the division operation.
 *
 * \return              NULL
 */
RbLanguageObject* RbLanguageObject::multiply(const RbLanguageObject &rhs) const
{
    throw RbException("Cannot multiply a value of type '" + this->getType() + "' to a value of type '" + rhs.getType() + "'.");
    
    return NULL;
}



/**
 * Replace the variable. This default implementation does nothing.
 */
void RbLanguageObject::replaceVariable(RbLanguageObject *newVar) {
    
}


/** Set a member variable */
void RbLanguageObject::setConstMember(const std::string& name, const RbPtr<const Variable> &var) {
    
    // here, we might want to do some general stuff like catching all members so that we can provide general functions as
    // 1) getNames()
    // 2) getMember(name)
    
    setConstMemberVariable(name, var );
    
}


/** Set a member variable */
void RbLanguageObject::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    throw RbException("No constant member with name \"" + name + "\" found to set.");
}


/* Set a member variable.
 * In this default implementation, we delegate to setConstMemberVariable.
 * Derived classes of MemberObject who need non-const variable should overwrite this function.
 * If you don't care if the variable is const, then you should only overwrite the setConstMemberVariable.
 */
void RbLanguageObject::setMember(const std::string& name, const RbPtr<Variable> &var) {
    
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
void RbLanguageObject::setMemberVariable(const std::string& name, const RbPtr<Variable> &var) {
    
    setConstMemberVariable(name, RbPtr<const Variable>( var ) );
    
}



/** 
 * Setting the name of the internal variable. The default implementation does nothing because we don't have a DAG node as our internal variable.
 * Note, RevLanguage types which can be used as types in the DAG should override this method.
 */
void RbLanguageObject::setName(std::string const &n) {
    // do nothing
}


/**
 * Subtraction operation.
 * Since we don't know the types and thus don't know the special behavior we simply throw and error.
 *
 * \param[in]   rhs     The right hand side value of the division operation.
 *
 * \return              NULL
 */
RbLanguageObject* RbLanguageObject::subtract(const RbLanguageObject &rhs) const
{
    throw RbException("Cannot subtract a value of type '" + rhs.getType() + "' from a value of type '" + this->getType() + "'.");
    
    return NULL;
}


/**
 * Get the info about this object as a string.
 * By default we simply call the overloaded operator<<.
 *
 */
std::string RbLanguageObject::toString( void ) const
{
    std::stringstream o;
    printValue(o);
    
    return o.str();
}

/** Make sure we can print the value of the object easily */
std::ostream& operator<<(std::ostream& o, const RbLanguageObject& x) {
    
    x.printValue(o);
    return o;
}

