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
#include "Frame.h"
#include "MemberFunction.h"
#include "MemberObject.h"
#include "MemberSlot.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbNames.h"
#include "VectorString.h"
#include "VariableNode.h"


/** Constructor: we set member variables here from member rules */
MemberObject::MemberObject(const MemberRules& memberRules) : RbComplex(), members() {

    /* Fill member table (frame) based on member rules */
    for ( MemberRules::const_iterator i = memberRules.begin(); i != memberRules.end(); i++ ) {

        members.push_back( (*i)->getArgLabel(), new MemberSlot( *i ) );
    }
}


/** Convert to type: throw an error */
MemberObject* MemberObject::convertTo(const std::string& type, int dim) const {

    std::ostringstream msg;
    msg << "Type conversion of " << getTypeSpec() << " to " << TypeSpec(type, dim) << " not supported (yet)";
    throw RbException(msg);
}


/** Execute member method: delegate to method table. */
DAGNode* MemberObject::executeMethod(const std::string& name, const std::vector<Argument>& args) {

    return getMethods().executeFunction(name, args);
}


/** Map member method call to internal function call. This is used as an alternative mechanism to providing a complete
 *  RbFunction object to execute a member method call. We throw an error here to capture cases where this mechanism
 *  is used without the appropriate mapping to internal function calls being present. */
DAGNode* MemberObject::executeOperation(const std::string& name, ArgumentFrame& args) {

    throw RbException( "No mapping from member method " + name + " to internal function call provided" );
}


/** Get class vector describing type of object */
const VectorString& MemberObject::getClass(void) const {

    static VectorString rbClass = VectorString(MemberObject_name) + RbComplex::getClass();
    return rbClass;
}


/** Get constant value (evaluate all member variables and replace with constants) of member object */
MemberObject* MemberObject::getConstValue( void ) const {

    MemberObject* temp = clone();

    for ( size_t i = 0; i < temp->members.size(); i++ ) {
    
        RbObject* constValue = temp->members[i].getValue()->clone();
        temp->members[i].setReferenceFlag( false );
        temp->setValue( temp->members[i].getName(), constValue );
    }

    return temp;
}


/** Return member rules (no members) */
const MemberRules& MemberObject::getMemberRules(void) const {

    throw RbException( "Object does not have members" );
}


/** Get type specification for a member variable */
const TypeSpec& MemberObject::getMemberTypeSpec(const std::string& name) const {

    return members[name].getTypeSpec();
}


/** Get method specifications (no methods) */
const MethodTable& MemberObject::getMethods(void) const {

    static MethodTable methods;
    
    return methods;
}


/**
 * Get subscript element. We are responsible for finding element
 * index[0] using our subscript operator. If there are more indices
 * in the index array, we delegate the job to our subelement
 * after removing index[0] from the index array.
 */
DAGNode* MemberObject::getSubelement( size_t i ) {

    throw RbException( "Object does not support subscripting" );
}


/** Get value of a member variable */
const RbObject* MemberObject::getValue(const std::string& name) {

    return members.getValue(name);
}


/** Get value of a member variable (const) */
const RbObject* MemberObject::getValue(const std::string& name) const {

    return members.getValue(name);
}


/** Get a member variable */
const DAGNode* MemberObject::getVariable(const std::string& name) const {

    return members.getVariable(name);
}


/** Get a member variable (non-const, for derived classes) */
DAGNode* MemberObject::getVariable(const std::string& name) {

    return const_cast<DAGNode*>(members.getVariable(name));
}


/** Is the object constant? */
bool MemberObject::isConstant( void ) const {

    for ( size_t i = 0; i < members.size(); i++ ) {
        if ( !members[i].getValue()->isConstant() )
            return false;
    }

    return true;
}


/** Is convertible to type and dim? Default is false for member objects; override if you want to support type conversion */
bool MemberObject::isConvertibleTo(const std::string& type, int dim, bool once) const {

	return false;
}


/** Print value for user */
void MemberObject::printValue(std::ostream& o) const {

    for ( size_t i = 0; i < members.size(); i++ ) {

        o << "." << members[i].getName() << std::endl;
        if ( members[i].getValue() == NULL)
            o << "NULL";
        else
            members[i].getValue()->printValue(o);
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


/** Set subscript element, or element of subscript element */
void MemberObject::setElement( VectorNatural& index, DAGNode* var, bool convert ) {

    throw RbException( "Unexpected call to setElement for a " + getTypeSpec().toString() + " value" );
}


/** Set subscript element, or element of subscript element */
void MemberObject::setSubelement( size_t index, DAGNode* var, bool convert ) {

    throw RbException( "Unexpected call to setSubelement for a " + getTypeSpec().toString() + " value" );
}


/** Set value of a member variable */
void MemberObject::setValue(const std::string& name, RbObject* val) {

    members[name].setValue(val);
}


/** Set a member variable */
void MemberObject::setVariable(const std::string& name, DAGNode* var) {

    members[name].setVariable(var);
}

