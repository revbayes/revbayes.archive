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
#include "DeterministicNode.h"
#include "Distribution.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbObject.h"
#include "StochasticNode.h"
#include "VectorString.h"
#include "Workspace.h"

#include <sstream>


/** Construct rule based on default value; use "" for no label. */
ArgumentRule::ArgumentRule(const std::string& argName, RbLanguageObject *defVal) : RbInternal(), label(argName), argSlot(argName, defVal->getTypeSpec()), hasDefaultVal(true) {

    Variable* tmpVar = new Variable( new ConstantNode(defVal) );
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
ArgumentRule::ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, RbLanguageObject *defValue) : RbInternal(), label(argName), argSlot(argName,argTypeSp), hasDefaultVal(true) {
    
    Variable* tmpVar = new Variable( new ConstantNode(defValue) );
    argSlot.setVariable( tmpVar );
}


/** Construct rule with default reference or value variable. */
ArgumentRule::ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, DAGNode* defVariable) : RbInternal(), label(argName), argSlot(argName,argTypeSp), hasDefaultVal(true) {
        
    Variable* tmpVar = new Variable( defVariable );
    argSlot.setVariable( tmpVar );
}


const std::string& ArgumentRule::getArgumentLabel(void) const {
    return label;
}


const std::string& ArgumentRule::getArgumentType(void) const {
    return argSlot.getSlotType();
}


const TypeSpec& ArgumentRule::getArgumentTypeSpec(void) const {
    return argSlot.getSlotTypeSpec();
}


/** Get class name of object */
const std::string& ArgumentRule::getClassName(void) { 
    
    static std::string rbClassName = "Argument rule";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ArgumentRule::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbInternal::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& ArgumentRule::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


const Variable& ArgumentRule::getDefaultVariable(void) const {
    return argSlot.getVariable(); 
}


Variable& ArgumentRule::getDefaultVariable(void) {
    return argSlot.getVariable(); 
}


bool ArgumentRule::hasDefault(void) const {
    return hasDefaultVal;
}


/** Test if argument is valid */
bool ArgumentRule::isArgumentValid(const RbVariablePtr& var, bool convert) const {
    
    if ( var == NULL )
        return false;
    
    // first we check if the type we want is already guaranteed by the variable
    if ( var->getValueTypeSpec().isDerivedOf( argSlot.getSlotTypeSpec() ) ) {
        return true;
    }

    // we can only change the required value type of the variable if we want a derived type of the current value type
    if ( argSlot.getSlotTypeSpec().isDerivedOf( var->getValueTypeSpec() ) ) {
        
        // We check first if the variable is constant
        if ( var->getDagNode()->isTypeSpec( ConstantNode::getClassTypeSpec() ) ) {
            // since the node is constant, we can actually change its value if we need to
            
            
            // first we check if the argument needs to be converted
            if ( var->getValue().isTypeSpec( argSlot.getSlotTypeSpec() ) ) {
                // No, we don't.
                
                // do the conversion if we are actually asked to
                if ( convert ) {
                    var->setValueTypeSpec( argSlot.getSlotTypeSpec() );
                }
                
                return true;
            } else if ( var->getValue().isConvertibleTo( argSlot.getSlotTypeSpec() ) ) {
                // Yes, we can and have to convert
                
                // should we do the type conversion?
                if ( convert ) {
                    ConstantNode* theConstNode = static_cast<ConstantNode*>( var->getDagNode() );
                    // Do the type conversion
                    RbLanguageObject* convertedObj = static_cast<RbLanguageObject*>( var->getValue().convertTo( argSlot.getSlotTypeSpec() ) );
                    theConstNode->setValue( convertedObj );
                    
                    // set the new type spec of the variable
                    var->setValueTypeSpec( argSlot.getSlotTypeSpec() );
                }
                return true;
            }
        }
        else {
            // We have a variable node (either stochastic or deterministic).
            // We cannot change the value of a variable node permanently.
            // So just make sure that the return type of the function or distribution fit.
            
            // first handle deterministic nodes
            if ( var->getDagNode()->isTypeSpec( DeterministicNode::getClassTypeSpec() ) ) {
                
                const DeterministicNode* theDetNode = static_cast<const DeterministicNode*>( var->getDagNode() );
                const RbFunction& theFunction = theDetNode->getFunction();
                
                // first we check if the argument needs to be converted
                if ( theFunction.getReturnType().isDerivedOf( argSlot.getSlotTypeSpec() ) ) {
                    // Yes, the function guarantees to return a value of the required type (or any derived type)
                    
                    // do the conversion if we are actually asked to
                    if ( convert ) {
                        var->setValueTypeSpec( argSlot.getSlotTypeSpec() );
                    }
                    
                    return true;
                }
            }
            else { // if ( var->getDagNode()->isTypeSpec( StochasticNode::getClassTypeSpec() ) ) {
                
                const StochasticNode* theStocNode = static_cast<const StochasticNode*>( var->getDagNode() );
                const Distribution& theDistribution = theStocNode->getDistribution();
                
                // first we check if the argument needs to be converted
                if ( theDistribution.getVariableType().isDerivedOf( argSlot.getSlotTypeSpec() ) ) {
                    // Yes, the function guarantees to return a value of the required type (or any derived type)
                    
                    // do the conversion if we are actually asked to
                    if ( convert ) {
                        var->setValueTypeSpec( argSlot.getSlotTypeSpec() );
                    }
                    
                    return true;
                }
            }
            
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
std::string ArgumentRule::debugInfo(void) const {

    std::ostringstream o;

    o << "ArgumentRule:" << std::endl;
    o << "label         = " << label << std::endl;
    o << "argSlot       = " << argSlot << std::endl;
    o << "hasDefaultVal = " << hasDefaultVal << std::endl;

    return o.str();
}

