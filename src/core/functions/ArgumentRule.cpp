/**
 * @file
 * This file contains the implementation of ArgumentRule, which is
 * the base class for objects used to describe rules for arguments
 * passed to functions.
 *
 * @brief Implementation of ArgumentRule 
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "ContainerNode.h"
#include "DeterministicMemberNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "VectorString.h"
#include "Workspace.h"

#include <sstream>


/** Construct rule based on default value; use "" for no label. */
ArgumentRule::ArgumentRule(const std::string& argName, RbObject* defVal)
    : RbInternal(), label(argName), argSlot(defVal->getTypeSpec()), hasDefaultVal(true) {

    argSlot.setValue( defVal );
}


/**
 * Construct rule without default value; use "" for no label. To make sure that the
 * argument rule is set to the right language type specification, we call the workspace
 * to translate from object type to language type. This is important because
 * otherwise calling the constructor with container types would result in argument
 * rules that had a container type instead of the language type of the container, which
 * would make the rule worthless.
 */
ArgumentRule::ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp)
    : RbInternal(), label(argName), argSlot(Workspace::userWorkspace().getTypeSpec(argTypeSp)), hasDefaultVal(false) {
}


/** Construct rule with default value. We rely on workspace to check the provided type specification. */
ArgumentRule::ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, RbObject* defValue)
: RbInternal(), label(argName), argSlot(Workspace::userWorkspace().getTypeSpec(argTypeSp)), hasDefaultVal(true) {

    argSlot.setValue( defValue );
}


/** Construct rule with default reference or value variable. */
ArgumentRule::ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, DAGNode* defVariable)
    : RbInternal(), label(argName), argSlot(Workspace::userWorkspace().getTypeSpec(argTypeSp)), hasDefaultVal(true) {

    argSlot.setVariable( defVariable );
}


/** Convert an argument to a variable that does fit the argument rule */
DAGNode* ArgumentRule::convert(const DAGNode* arg) const {
    
    if ( arg == NULL )
        return NULL;

    RbObject* theConvertedValue = arg->getValue()->convertTo( argSlot.getTypeSpec() );
    
    if ( theConvertedValue->isType( Container_name ) )
        return new ContainerNode( (Container*)( theConvertedValue ) );
    else if ( theConvertedValue->isType( MemberObject_name ) )
        return new DeterministicMemberNode( (MemberObject*)( theConvertedValue ) );
    else
        return new ConstantNode( theConvertedValue );
}


/** Get class vector describing type of object */
const VectorString& ArgumentRule::getClass(void) const { 

    static VectorString rbClass = VectorString(ArgumentRule_name) + RbInternal::getClass();
	return rbClass; 
}


/** Get default value */
RbObject* ArgumentRule::getDefaultValue(void) const {

    if ( !hasDefault() )
        throw RbException("There is no default value for argument '" + label + "'");

    const RbObject* defValue = argSlot.getValue();
    if ( defValue == NULL )
        return NULL;
    else
        return defValue->clone();
}


/** Get default variable */
DAGNode* ArgumentRule::getDefaultVariable(void) const {

    if ( !hasDefault() )
        throw RbException("There is no default value for argument '" + label + "'");

    const DAGNode* defValue = argSlot.getVariable();
    if ( defValue == NULL )
        return NULL;
    else
        return defValue->clone();
}


/** Get default reference (a variable ptr corresponding to a '&' argument) */
DAGNode* ArgumentRule::getDefaultReference(void) {

    if ( !hasDefault() )
        throw RbException("There is no default value for argument '" + label + "'");

    return argSlot.getReference();
}


/** Test if argument is valid */
bool ArgumentRule::isArgValid(const DAGNode* var, bool& needsConversion, bool once) const {
    
    needsConversion = false;
    if ( var == NULL )
        return true;

    if ( once ) {
        /* We are executing once and match based on current value; error will be thrown if arguments have not been evaluated already */
        const RbObject* value = var->getValue();
        if ( value->isType( argSlot.getTypeSpec().getType() ) && value->getDim() == argSlot.getTypeSpec().getDim() ) {
            needsConversion = false;
            return true;
        }

        if ( value->isConvertibleTo( argSlot.getTypeSpec(), once ) == true) {
            needsConversion = true;
            return true;
        }
    }
    else {
        /* We need safe argument matching for repeated evaluation in a function node */
        if ( Workspace::userWorkspace().isXOfTypeY( var->getValueType(), argSlot.getTypeSpec().getType() ) == true &&
            var->getDim() == argSlot.getTypeSpec().getDim()  ) {
            needsConversion = false;
            return true;
        }

        if ( Workspace::userWorkspace().isXConvertibleToY( var->getTypeSpec(), argSlot.getTypeSpec() ) == true) {
            needsConversion = true;
            return true;
        }
    }

    return false;
}


/** Print value for user (in descriptions of functions, for instance) */
void ArgumentRule::printValue(std::ostream &o) const {

    o << argSlot.getTypeSpec().toString();
    o << " \"" << label << "\"";
    if ( hasDefaultVal ) {
        o << " = ";
        if ( argSlot.getVariable() == NULL )
            o << "NULL";
        else
            argSlot.getVariable()->printValue( o );
    }
}


/** Provide complete information about object */
std::string ArgumentRule::richInfo(void) const {

    std::ostringstream o;

    o << "ArgumentRule:" << std::endl;
    o << "label         = " << label << std::endl;
    o << "argSlot       = " << argSlot << std::endl;
    o << "hasDefaultVal = " << hasDefaultVal << std::endl;

    return o.str();
}

