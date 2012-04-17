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
#include "Distribution.h"
#include "RbConstVariablePtr.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbUtil.h"
#include "RbObject.h"
#include "RbVector.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "Workspace.h"

#include <sstream>


/** Construct rule based on default value; use "" for no label. */
ArgumentRule::ArgumentRule(const std::string& argName, RbLanguageObject *defVal) : RbInternal(), label(argName), argTypeSpec( defVal->getTypeSpec() ), hasDefaultVal(true), defaultVariable( new Variable( new ConstantNode( defVal ) ) ) {

    defaultVariable->incrementReferenceCount();
}


/**
 * Construct rule without default value; use "" for no label.
 */
ArgumentRule::ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, bool opt) : RbInternal(), label(argName), argTypeSpec( argTypeSp ), hasDefaultVal(false), defaultVariable( NULL ), optional( opt ) {
    
}


/** Construct rule with default value. We rely on workspace to check the provided type specification. */
ArgumentRule::ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, RbLanguageObject *defValue) : RbInternal(), label(argName), argTypeSpec( argTypeSp ), hasDefaultVal(true), defaultVariable( new Variable( new ConstantNode( defValue ) ) ) {
    
    defaultVariable->incrementReferenceCount();
}


/** Construct rule with default reference or value variable. */
ArgumentRule::ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, DAGNode* defVariable) : RbInternal(), label(argName), argTypeSpec( argTypeSp ), hasDefaultVal(true), defaultVariable( new Variable( defVariable ) ) {
    
    defaultVariable->incrementReferenceCount();
}


/** Copy constructor */
ArgumentRule::ArgumentRule(const ArgumentRule& a) : RbInternal( a ), argTypeSpec( a.argTypeSpec ) {
    
    label               = a.label;
    defaultVariable     = a.defaultVariable;
    if ( defaultVariable != NULL )
        defaultVariable->incrementReferenceCount();
    
    hasDefaultVal       = a.hasDefaultVal;
    optional            = a.optional;
}


/** Destructor. */
ArgumentRule::~ArgumentRule() {
    
    if ( defaultVariable != NULL && defaultVariable->decrementReferenceCount() == 0) {
        delete defaultVariable;
    }
}


/** Assignment operator */
ArgumentRule& ArgumentRule::operator=(const ArgumentRule &a) {
    
    if ( this != &a ) {
        
        argTypeSpec         = a.argTypeSpec;
        label               = a.label;
        if (defaultVariable != NULL && defaultVariable->decrementReferenceCount() == 0 ) {
            delete defaultVariable;
        }
        
        defaultVariable     = a.defaultVariable;
        if ( defaultVariable != NULL )
            defaultVariable->incrementReferenceCount();
        
        hasDefaultVal       = a.hasDefaultVal;
        optional            = a.optional;
    }
    
    return *this;
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

/** Get type spec */
const TypeSpec& ArgumentRule::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


const Variable& ArgumentRule::getDefaultVariable(void) const {
    return *defaultVariable; 
}


Variable& ArgumentRule::getDefaultVariable(void) {
    return *defaultVariable; 
}


bool ArgumentRule::hasDefault(void) const {
    return hasDefaultVal;
}


/** Test if argument is valid */
bool ArgumentRule::isArgumentValid(const Variable* var, bool convert) const {
    
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
                    const_cast<Variable*>( var )->setValueTypeSpec( argTypeSpec );
                }
                
                return true;
            } else if ( var->getValue().isConvertibleTo( argTypeSpec ) ) {
                // Yes, we can and have to convert
                
                // should we do the type conversion?
                if ( convert ) {
                    ConstantNode* theConstNode = static_cast<ConstantNode*>( const_cast<Variable*>( var )->getDagNode() );
                    // Do the type conversion
                    RbLanguageObject* convertedObj = static_cast<RbLanguageObject*>( var->getValue().convertTo( argTypeSpec ) );
                    theConstNode->setValue( convertedObj );
                    
                    // set the new type spec of the variable
                    const_cast<Variable*>( var )->setValueTypeSpec( argTypeSpec );
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
                if ( theFunction.getReturnType().isDerivedOf( argTypeSpec ) ) {
                    // Yes, the function guarantees to return a value of the required type (or any derived type)
                    
                    // do the conversion if we are actually asked to
                    if ( convert ) {
                        const_cast<Variable*>( var )->setValueTypeSpec( argTypeSpec );
                    }
                    
                    return true;
                }
            }
            else { // if ( var->getDagNode()->isTypeSpec( StochasticNode::getClassTypeSpec() ) ) {
                
                const StochasticNode* theStocNode = static_cast<const StochasticNode*>( var->getDagNode() );
                const Distribution& theDistribution = theStocNode->getDistribution();
                
                // first we check if the argument needs to be converted
                if ( theDistribution.getVariableType().isDerivedOf( argTypeSpec ) ) {
                    // Yes, the function guarantees to return a value of the required type (or any derived type)
                    
                    // do the conversion if we are actually asked to
                    if ( convert ) {
                        const_cast<Variable*>( var )->setValueTypeSpec( argTypeSpec );
                    }
                    
                    return true;
                }
            }
            
        }
        
        
    }
    
    // Finally, we test if this is a vector of types which we accept
    if ( var->getValue().getTypeSpec().isDerivedOf( Container::getClassTypeSpec() ) ) {
        const Container& c = static_cast<const Container&>( var->getValue() );
        bool constant = var->getDagNode()->isTypeSpec( ConstantNode::getClassTypeSpec() );
        bool valid = true;
        bool conversionNeeded = false;
        TypeSpec conversionType = TypeSpec( RbVector<RbObject>::getClassName() );
        for (size_t i = 0; i < c.size(); i++) {
            valid &= isArgumentValid( c.getElement(i), conversionNeeded, conversionType );
        }
        if ( constant && conversionNeeded && convert ) {
            
            ConstantNode* theConstNode = static_cast<ConstantNode*>( const_cast<Variable*>( var )->getDagNode() );
            // Do the type conversion
            RbLanguageObject* convertedObj = static_cast<RbLanguageObject*>( var->getValue().convertTo( conversionType ) );
            theConstNode->setValue( convertedObj );
                
            // set the new type spec of the variable
            const_cast<Variable*>( var )->setValueTypeSpec( argTypeSpec );
            
        }
        
        return valid && ( constant == conversionNeeded );
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
    
    
    // Finally, we test if this is a vector of types which we accept
    if ( var.getTypeSpec().isDerivedOf( Container::getClassTypeSpec() ) ) {
        TypeSpec* et = new TypeSpec( RbVector<RbObject>::getClassName() );
        conversionType.setElementType( et );
        return isArgumentValid( static_cast<const Container&>( var ).getElement(0), conversionNeeded, *et );
    }
    
    
    
    return false;
}


/* Is optional? */
bool ArgumentRule::isOptional(void) const {
    return optional;
}
 


/** Print value for user (in descriptions of functions, for instance) */
void ArgumentRule::printValue(std::ostream &o) const {

    o << argTypeSpec;
    o << " \"" << label << "\"";
    if ( hasDefaultVal ) {
        o << " = ";
        defaultVariable->printValue( o );
    }
}


/** Provide complete information about object */
std::string ArgumentRule::debugInfo(void) const {

    std::ostringstream o;

    o << "ArgumentRule:" << std::endl;
    o << "label             = " << label << std::endl;
    o << "hasDefaultVal     = " << hasDefaultVal << std::endl;
    o << "defaultVaribale   = ";
    if ( defaultVariable != NULL && defaultVariable->getDagNode() != NULL ) {
        defaultVariable->getValue().printValue(o);
    } 
    else {
        o << "NULL";
    }
    o << std::endl;

    return o.str();
}

