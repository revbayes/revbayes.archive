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
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "Workspace.h"

#include <sstream>



/**
 * Construct rule without default value; use "" for no label.
 */
RevLanguage::ArgumentRule::ArgumentRule(const std::string& argName, bool c, const TypeSpec& argTypeSp) : label(argName), argTypeSpec( argTypeSp ), isConst( c ), hasDefaultVal(false), defaultVar( NULL ) {

}


/**
 * Construct rule without default value; use "" for no label.
 */
RevLanguage::ArgumentRule::ArgumentRule(const std::string& argName, bool c, const TypeSpec& argTypeSp, RbLanguageObject *defVal) : label(argName), argTypeSpec( argTypeSp ), isConst( c ), hasDefaultVal( true ), defaultVar( new Variable( defVal ) ) {
    
}



RevLanguage::ArgumentRule* RevLanguage::ArgumentRule::clone( void ) const {
    return new ArgumentRule( *this );
}


const std::string& RevLanguage::ArgumentRule::getArgumentLabel(void) const {
    return label;
}


const RevLanguage::TypeSpec& RevLanguage::ArgumentRule::getArgumentTypeSpec(void) const {
    return argTypeSpec;
}



const RevLanguage::Variable& RevLanguage::ArgumentRule::getDefaultVariable( void ) const {
    if ( defaultVar == NULL ) {
        throw RbException("Cannot get default variable \"" + label + "\"");
    }
    return *defaultVar;
}


bool RevLanguage::ArgumentRule::hasDefault(void) const {
    return hasDefaultVal;
}


/** Test if argument is valid */
bool RevLanguage::ArgumentRule::isArgumentValid(const RbPtr<const Variable> &var, bool convert) const {
    
    if ( var == NULL )
        return false;
    
    // first we check if the type we want is already guaranteed by the variable
    if ( var->getValueTypeSpec().isDerivedOf( argTypeSpec ) ) {
        return true;
    }

    // we can only change the required value type of the variable if we want a derived type of the current value type
    if ( argTypeSpec.isDerivedOf( var->getValueTypeSpec() ) ) {
                          
        // first we check if the argument needs to be converted
        if ( var->getValue().isTypeSpec( argTypeSpec ) ) {
            // No, we don't.
            
            // do the conversion if we are actually asked to
            if ( convert ) {
                // this is a safe const cast (Sebastian)
                const_cast<Variable*>( (const Variable *)var )->setValueTypeSpec( argTypeSpec );
            }
            
            return true;
        } else if ( var->getValue().isConstant() && var->getValue().isConvertibleTo( argTypeSpec ) ) {
            // Yes, we can and have to convert
            
            // should we do the type conversion?
            if ( convert ) {
                
                RbLanguageObject* convObj = var->getValue().convertTo( argTypeSpec );
                const_cast<Variable*>( (const Variable *) var )->setValue( convObj );

                // set the new type spec of the variable
                const_cast<Variable*>( (const Variable *) var )->setValueTypeSpec( argTypeSpec );
            }
            return true;
        }
        
    }
    
    return false;
}


/** Test if argument is valid */
bool RevLanguage::ArgumentRule::isArgumentValid(const RbLanguageObject& var, bool &conversionNeeded, TypeSpec &conversionType) const {
    
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


bool RevLanguage::ArgumentRule::isConstant( void ) const {
    return isConst;
}



bool RevLanguage::ArgumentRule::isEllipsis( void ) const {
    return false;
}
 


/** Print value for user (in descriptions of functions, for instance) */
void RevLanguage::ArgumentRule::printValue(std::ostream &o) const {

    o << argTypeSpec;
    o << " \"" << label << "\"";
}


/** Provide complete information about object */
std::string RevLanguage::ArgumentRule::debugInfo(void) const {

    std::ostringstream o;

    o << "ArgumentRule:" << std::endl;
    o << "label             = " << label << std::endl;
    o << "hasDefaultVal     = " << hasDefaultVal << std::endl;
    o << std::endl;

    return o.str();
}

