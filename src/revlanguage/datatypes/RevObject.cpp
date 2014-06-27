/**
 * @file
 * This file contains the implementation of some of the functions
 * in the abstract base class for language objects, RevObject.
 *
 * @brief Partial implementation of RevObject
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-02, version 1.0
 * @extends RevObject
 *
 * $Id$
 */

#include "MemberFunction.h"
#include "MethodTable.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RevObject.h"
#include "RlUtils.h"
#include "Vector.h"
#include "RlString.h"
#include "TypeSpec.h"
#include "Workspace.h"

#include <sstream>
#include <stdio.h>

using namespace RevLanguage;




/** Constructor: we set member variables here from member rules */
RevObject::RevObject(const MemberRules& memberRules) {
    
}


/** 
 * Destructor. 
 * Nothing to do here because we don't own anything.
 */
RevObject::~RevObject( void )
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
RevObject* RevObject::add(const RevObject &rhs) const
{
    throw RbException("Cannot add a value of type '" + this->getType() + "' to a value of type '" + rhs.getType() + "'.");
    
    return NULL;
}

/** The default implementation does nothing because we don't have an internal object */
void RevObject::constructInternalObject( void ) {
    // nothing to do
}



/** Convert to type and dim. The caller manages the returned object. */
RevObject* RevObject::convertTo(const TypeSpec& typeSpec) const {
        
    throw RbException("Failed conversion from " + getTypeSpec() + " to " + typeSpec);
    
    return NULL;
}


/**
 * Make a new object that is an indirect deterministic reference to the object.
 * The default implementation throws an error.
 */
RevObject* RevObject::dagReference(void) {
    
    throw RbException( "Dynamic reference to this object type not supported");
}


/**
 * Decrement operation.
 * Since we don't know the types and thus don't know the special behavior we simply throw and error.
 */
void RevObject::decrement( void ) 
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
RevObject* RevObject::divide(const RevObject &rhs) const
{
    throw RbException("Cannot divide a value of type '" + this->getType() + "' to a value of type '" + rhs.getType() + "'.");
    
    return NULL;
}


/** 
 * Execute simple method. 
 */
RevObject* RevObject::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "get") 
    {
        // get the member with give name
        const std::string &varName = static_cast<const RlString &>( args[0].getVariable()->getRevObject() ).getValue();
        
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
const std::string& RevObject::getClassName(void) { 
    
    static std::string rbClassName = "language object";
	return rbClassName; 
}


/** Get class vector describing type of object */
const TypeSpec& RevObject::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec(getClassName() );
	
    return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& RevObject::getMemberRules(void) const {
    
    static const MemberRules rules = MemberRules();
    
    return rules;
}


/** Get a member variable */
RevObject* RevObject::getMember(const std::string& name) const {
    throw RbException("No Member named '" + name + "' available.");
}


/** 
 * Get method specifications of all member objects. 
 * We support two methods:
 * 1) memberNames()
 * 2) get("name")
 */
const MethodTable& RevObject::getMethods(void) const {
    
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
        methods.addFunction("get", new MemberFunction(RevObject::getClassTypeSpec(), getArgRules) );
        
        methodsSet = true;
    }   
    
    return methods;
}


/**
 * Get the internal value type of the object as a string corresponding to the type name.
 * Type <double> should return "double", < std::vector<double> > should return "std::vector<double>" etc.
 * For RevBayesCore value types, the string is the class name. For instance, <RevBayesCore::Mcmc> should
 * return "Mcmc", etc.
 */
const std::string& RevObject::getValueType( void ) const {
    
    throw RbException("No value template type implemented for this object (lazy RevBayes programmers...)");
}


/**
 * Get the value as a DAG node. This default implementation throws an error.
 */
const std::string& RevObject::getType( void ) const {
    
    return getTypeSpec().getType();
}


/**
 * Get the value as a DAG node. This default implementation throws an error.
 */
RevBayesCore::DagNode* RevObject::getDagNode( void ) const {
    
    throw RbException("RevLanguage only objects cannot be used inside DAG's!");
    
    return NULL;
}


/** Does this object have a member called "name" */
bool RevObject::hasMember(std::string const &name) const {
    return false;
}


/** Does this object have an internal value node? Default implementation returns false. */
bool RevObject::hasDagNode(void) const {
    return false;
}


/**
 * Increment operation.
 * Since we don't know the types and thus don't know the special behavior we simply throw and error.
 */
void RevObject::increment( void ) 
{
    throw RbException("Cannot increment a value of type '" + this->getType() + "'.");
    
}


/**
 * The default implementation is that the variable is constant. Only variables which actually store
 * internally DAG nodes have to ask the DAG nodes if they are constant.
 */
bool RevObject::isConstant( void ) const {
    return true;
}


/** Is convertible to type? */
bool RevObject::isConvertibleTo(const TypeSpec& type) const {
    
    return false;
}


/* Are we of specified language type? */
bool RevObject::isTypeSpec(const TypeSpec& typeSpec) const {
    
    return getTypeSpec().isDerivedOf( typeSpec );
}


/**
  * Converting the value of the internal variable to a constant. The default implementation does nothing because we don't have a DAG node as our internal variable.
  * Note, RevLanguage types which can be used as types in the DAG should overwrite this method.
  */
void RevObject::makeConstantValue( void ) {
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
RevObject* RevObject::multiply(const RevObject &rhs) const
{
    throw RbException("Cannot multiply a value of type '" + this->getType() + "' to a value of type '" + rhs.getType() + "'.");
    
    return NULL;
}



/**
 * Replace the variable. This default implementation does nothing.
 */
void RevObject::replaceMe(RevObject *newVar) {
    
}


/** Set a member variable */
void RevObject::setConstMember(const std::string& name, const RevPtr<const Variable> &var) {
    
    // here, we might want to do some general stuff like catching all members so that we can provide general functions as
    // 1) getNames()
    // 2) getMember(name)
    
    setConstMemberVariable(name, var );
    
}


/** Set a member variable */
void RevObject::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    throw RbException("No constant member with name \"" + name + "\" found to set.");
}


/* Set a member variable.
 * In this default implementation, we delegate to setConstMemberVariable.
 * Derived classes of MemberObject who need non-const variable should overwrite this function.
 * If you don't care if the variable is const, then you should only overwrite the setConstMemberVariable.
 */
void RevObject::setMember(const std::string& name, const RevPtr<Variable> &var) {
    
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
void RevObject::setMemberVariable(const std::string& name, const RevPtr<Variable> &var) {
    
    setConstMemberVariable(name, RevPtr<const Variable>( var ) );
    
}



/** 
 * Setting the name of the internal variable. The default implementation does nothing because we don't have a DAG node as our internal variable.
 * Note, RevLanguage types which can be used as types in the DAG should override this method.
 */
void RevObject::setName(std::string const &n) {
    // do nothing
}


/**
 * Setting the value node, potentially replacing the existing value node. The default implementation
 * throws an error because we do not have a value node.
 */
void RevObject::setDagNode(RevBayesCore::DagNode* newVal) {
    
    if ( this->hasDagNode() )
        throw RbException( "Replacing or setting of the internal value node of this type of object not supported yet (lazy developers...)");
    else
        throw RbException( "This language object does not have an internal value node that can be replaced or set");
}


/**
 * Subtraction operation.
 * Since we don't know the types and thus don't know the special behavior we simply throw and error.
 *
 * \param[in]   rhs     The right hand side value of the division operation.
 *
 * \return              NULL
 */
RevObject* RevObject::subtract(const RevObject &rhs) const
{
    throw RbException("Cannot subtract a value of type '" + rhs.getType() + "' from a value of type '" + this->getType() + "'.");
    
    return NULL;
}


/**
 * Get the info about this object as a string.
 * By default we simply call the overloaded operator<<.
 *
 */
std::string RevObject::toString( void ) const
{
    std::stringstream o;
    printValue(o);
    
    return o.str();
}

/** Make sure we can print the value of the object easily */
std::ostream& operator<<(std::ostream& o, const RevObject& x) {
    
    x.printValue(o);
    return o;
}

