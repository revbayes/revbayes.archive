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

#include "Argument.h"
#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbObject.h"
#include "VectorString.h"
#include "Workspace.h"

#include <sstream>


/** Construct rule based on default value; use "" for no label. */
ArgumentRule::ArgumentRule(const std::string& argName, RbLanguageObject* defVal) : RbInternal(), label(argName), argSlot(argName, defVal->getTypeSpec()), hasDefaultVal(true) {

    Variable *tmpVar = new Variable( argName, new ConstantNode(defVal) );
    argSlot.setVariable( tmpVar );
}


/**
 * Construct rule without default value; use "" for no label. To make sure that the
 * argument rule is set to the right language type specification, we call the workspace
 * to translate from object type to language type. This is important because
 * otherwise calling the constructor with container types would result in argument
 * rules that had a container type instead of the language type of the container, which
 * would make the rule worthless.
 */
ArgumentRule::ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp) : RbInternal(), label(argName), argSlot(argName,argTypeSp), hasDefaultVal(false) {

}


/** Construct rule with default value. We rely on workspace to check the provided type specification. */
ArgumentRule::ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, RbLanguageObject* defValue) : RbInternal(), label(argName), argSlot(argName,argTypeSp), hasDefaultVal(true) {
    
    Variable *tmpVar = new Variable( new ConstantNode(defValue) );
    argSlot.setVariable( tmpVar );
}


/** Construct rule with default reference or value variable. */
ArgumentRule::ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, DAGNode* defVariable) : RbInternal(), label(argName), argSlot(argName,argTypeSp), hasDefaultVal(true) {
        
    
    Variable *tmpVar = new Variable( defVariable );
    argSlot.setVariable( tmpVar );
}


/** Get class vector describing type of object */
const VectorString& ArgumentRule::getClass(void) const { 

    static VectorString rbClass = VectorString(ArgumentRule_name) + RbInternal::getClass();
	return rbClass; 
}


/** Test if argument is valid */
bool ArgumentRule::isArgumentValid(DAGNode* var, bool& needsConversion, bool once) const {
    
    needsConversion = false;
    if ( var == NULL )
        return true;

    if ( once ) {
        /* We are executing once and match based on current value; error will be thrown if arguments have not been evaluated already */
        const RbLanguageObject* value = var->getValue();
        if ( value->isTypeSpec( argSlot.getSlotTypeSpec() ) ) {
            needsConversion = false;
            return true;
        }

        if ( value->isConvertibleTo( argSlot.getSlotTypeSpec(), once ) == true) {
            needsConversion = true;
            return true;
        }
    }
    else {
        /* We need safe argument matching for repeated evaluation in a function node */
        if ( Workspace::userWorkspace().isXOfTypeY( var->getValueType(), argSlot.getSlotTypeSpec().getType() ) == true ) {
            needsConversion = false;
            return true;
        }

        if ( Workspace::userWorkspace().isXConvertibleToY( var->getValueTypeSpec(), argSlot.getSlotTypeSpec() ) == true) {
            needsConversion = true;
            return true;
        }
    }

    return false;
}


/** Print value for user (in descriptions of functions, for instance) */
void ArgumentRule::printValue(std::ostream &o) const {

    o << argSlot.getSlotType();
    o << " \"" << label << "\"";
    if ( hasDefaultVal ) {
        o << " = ";
        argSlot.printValue( o );
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

