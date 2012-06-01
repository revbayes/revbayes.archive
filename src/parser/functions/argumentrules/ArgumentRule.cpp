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
#include "Container.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "ParserDistribution.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbUtil.h"
#include "RbObject.h"
#include "RlVector.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "Workspace.h"

#include <sstream>



/**
 * Construct rule without default value; use "" for no label.
 */
ArgumentRule::ArgumentRule(const std::string& argName, const bool &c, const TypeSpec& argTypeSp) : RbInternal(), label(argName), argTypeSpec( argTypeSp ), isConst( c ), hasDefaultVal(false), defaultVar( NULL ) {

}


/**
 * Construct rule without default value; use "" for no label.
 */
ArgumentRule::ArgumentRule(const std::string& argName, const bool &c, const TypeSpec& argTypeSp, const RbPtr<RbLanguageObject> &defVal) : RbInternal(), label(argName), argTypeSpec( argTypeSp ), isConst( c ), hasDefaultVal( true ), defaultVar( new ConstantNode( defVal ) ) {
    
}



ArgumentRule* ArgumentRule::clone( void ) const {
    return new ArgumentRule( *this );
}


const std::string& ArgumentRule::getArgumentLabel(void) const {
    return label;
}


const TypeSpec& ArgumentRule::getArgumentTypeSpec(void) const {
    return argTypeSpec;
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


const Variable& ArgumentRule::getDefaultVariable( void ) const {
    return defaultVar;
}


/** Get type spec */
const TypeSpec& ArgumentRule::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


bool ArgumentRule::hasDefault(void) const {
    return hasDefaultVal;
}


/** Test if argument is valid */
bool ArgumentRule::isArgumentValid(const RbPtr<const Variable> &var, bool convert) const {
    
    if ( var == NULL )
        return false;
    
    // first we check if the type we want is already guaranteed by the variable
    if ( var->getValueTypeSpec().isDerivedOf( argTypeSpec ) ) {
        return true;
    }

    // we can only change the required value type of the variable if we want a derived type of the current value type
    if ( argTypeSpec.isDerivedOf( var->getValueTypeSpec() ) ) {
        
        // We check first if the variable is constant
        if ( var->getDagNode()->isTypeSpec( ConstantNode::getClassTypeSpec() ) ) {
            // since the node is constant, we can actually change its value if we need to
            
            
            // first we check if the argument needs to be converted
            if ( var->getValue().isTypeSpec( argTypeSpec ) ) {
                // No, we don't.
                
                // do the conversion if we are actually asked to
                if ( convert ) {
                    // this is a safe const cast (Sebastian)
                    const_cast<Variable*>( (const Variable *)var )->setValueTypeSpec( argTypeSpec );
                }
                
                return true;
            } else if ( var->getValue().isConvertibleTo( argTypeSpec ) ) {
                // Yes, we can and have to convert
                
                // should we do the type conversion?
                if ( convert ) {
                    ConstantNode* theConstNode = static_cast<ConstantNode*>( (DAGNode *) const_cast<Variable*>( (const Variable *) var )->getDagNode() );
                    // Do the type conversion
                    RlValue<RbObject> tmp = var->getValue().convertTo( argTypeSpec );
                                    
                    std::vector<RbPtr<RbLanguageObject> > vals;
                    for (std::vector<RbPtr<RbObject> >::iterator i = tmp.value.begin(); i != tmp.value.end(); ++i) {
                        vals.push_back( RbPtr<RbLanguageObject>( static_cast<RbLanguageObject *>( (RbObject *) *i ) ) );
                    }
                    
                    RlValue<RbLanguageObject> convertedObj = RlValue<RbLanguageObject>(vals, tmp.lengths);
                    theConstNode->setValue( convertedObj );
                    
                    // set the new type spec of the variable
                    const_cast<Variable*>( (const Variable *) var )->setValueTypeSpec( argTypeSpec );
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
                
                const DeterministicNode* theDetNode = static_cast<const DeterministicNode*>( (const DAGNode *) var->getDagNode() );
                const RbFunction& theFunction = theDetNode->getFunction();
                
                // first we check if the argument needs to be converted
                if ( theFunction.getReturnType().isDerivedOf( argTypeSpec ) ) {
                    // Yes, the function guarantees to return a value of the required type (or any derived type)
                    
                    // do the conversion if we are actually asked to
                    if ( convert ) {
                        const_cast<Variable*>( (const Variable *) var )->setValueTypeSpec( argTypeSpec );
                    }
                    
                    return true;
                }
            }
            else { // if ( var->getDagNode()->isTypeSpec( StochasticNode::getClassTypeSpec() ) ) {
                
                const StochasticNode* theStocNode = static_cast<const StochasticNode*>( (const DAGNode *) var->getDagNode() );
                const ParserDistribution& theDistribution = theStocNode->getDistribution();
                
                // first we check if the argument needs to be converted
                if ( theDistribution.getVariableType().isDerivedOf( argTypeSpec ) ) {
                    // Yes, the function guarantees to return a value of the required type (or any derived type)
                    
                    // do the conversion if we are actually asked to
                    if ( convert ) {
                        const_cast<Variable*>( (const Variable *) var )->setValueTypeSpec( argTypeSpec );
                    }
                    
                    return true;
                }
            }
            
        }
        
        
    }
    
    return false;
}


/** Test if argument is valid */
bool ArgumentRule::isArgumentValid(const RbObject& var, bool &conversionNeeded, TypeSpec &conversionType) const {
    
    // first we check if the type we want is already guaranteed by the variable
    if ( var.getTypeSpec().isDerivedOf( argTypeSpec ) ) {
        return true;
    }
    
    if ( var.isConvertibleTo( argTypeSpec ) ) {
        // Yes, we can and have to convert
        // should we do the type conversion?
        conversionNeeded = true;
        conversionType.setElementType( new TypeSpec( argTypeSpec ) );
        
        return true;
    }
    
    return false;
}


bool ArgumentRule::isConstant( void ) const {
    return isConst;
}
 


/** Print value for user (in descriptions of functions, for instance) */
void ArgumentRule::printValue(std::ostream &o) const {

    o << argTypeSpec;
    o << " \"" << label << "\"";
}


/** Provide complete information about object */
std::string ArgumentRule::debugInfo(void) const {

    std::ostringstream o;

    o << "ArgumentRule:" << std::endl;
    o << "label             = " << label << std::endl;
    o << "hasDefaultVal     = " << hasDefaultVal << std::endl;
    o << std::endl;

    return o.str();
}

