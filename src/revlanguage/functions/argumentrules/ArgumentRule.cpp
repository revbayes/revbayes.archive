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

using namespace RevLanguage;

/**
 * Construct rule without default value; use "" for no label.
 */
ArgumentRule::ArgumentRule(const std::string& argName, bool c, const TypeSpec& argTypeSp) :
    argTypeSpecs( 1, argTypeSp ),
    defaultVar( NULL ),
    isConst( c ),
    label(argName),
    hasDefaultVal(false)
{

}


/**
 * Construct rule without default value; use "" for no label.
 */
ArgumentRule::ArgumentRule(const std::string& argName, bool c, const TypeSpec& argTypeSp, RevObject *defVal) :
    argTypeSpecs( 1, argTypeSp ),
    defaultVar( new Variable( defVal ) ),
    isConst( c ),
    label(argName),
    hasDefaultVal( true )
{
    
}


/**
 * Construct rule without default value; use "" for no label.
 */
ArgumentRule::ArgumentRule(const std::string& argName, bool c, const std::vector<TypeSpec>& argTypeSp) :
    argTypeSpecs( argTypeSp ),
    defaultVar( NULL ),
    isConst( c ),
    label(argName),
    hasDefaultVal(false)
{
    
}


/**
 * Construct rule without default value; use "" for no label.
 */
ArgumentRule::ArgumentRule(const std::string& argName, bool c, const std::vector<TypeSpec>& argTypeSp, RevObject *defVal) :
    argTypeSpecs( argTypeSp ),
    defaultVar( new Variable( defVal ) ),
    isConst( c ),
    label(argName),
    hasDefaultVal( true )
{
    
}



ArgumentRule* RevLanguage::ArgumentRule::clone( void ) const
{

    return new ArgumentRule( *this );
}


const std::string& ArgumentRule::getArgumentLabel(void) const {
    return label;
}


const std::vector<TypeSpec>& ArgumentRule::getArgumentTypeSpec(void) const {
    return argTypeSpecs;
}



const Variable& ArgumentRule::getDefaultVariable( void ) const {
    
    if ( defaultVar == NULL ) 
    {
        throw RbException("Cannot get default variable \"" + label + "\"");
    }
    
    return *defaultVar;
}


bool ArgumentRule::hasDefault(void) const {
    
    return hasDefaultVal;
}


/** Test if argument is valid */
bool ArgumentRule::isArgumentValid(const RevPtr<const Variable> &var, bool convert) const {
    
    if ( var == NULL )
        return false;
    
    for ( std::vector<TypeSpec>::const_iterator it = argTypeSpecs.begin(); it != argTypeSpecs.end(); ++it )
    {
        // first we check if the type we want is already guaranteed by the variable
        if ( var->getValueTypeSpec().isDerivedOf( *it ) ) 
        {
            return true;
        }

        // we can only change the REQUIRED value type of the variable if we want a derived type of the current value type
        // the actual variable type may be different to the required type
        if ( it->isDerivedOf( var->getValueTypeSpec() ) ) 
        {
                          
            // first we check if the argument needs to be converted
            if ( var->getValue().isTypeSpec( *it ) ) 
            {
                // No, we don't.
            
                // do the conversion if we are actually asked to
                if ( convert ) 
                {
                    // this is a safe const cast (Sebastian)
                    const_cast<Variable*>( (const Variable *)var )->setValueTypeSpec( *it );
                }
            
                return true;
            } 
            else if ( var->getValue().isConstant() && var->getValue().isConvertibleTo( *it ) ) 
            {
                // Yes, we can and have to convert
            
                // should we do the type conversion?
                if ( convert ) 
                {
                
                    RevObject* convObj = var->getValue().convertTo( *it );
                    const_cast<Variable*>( (const Variable *) var )->setValue( convObj );

                    // set the new type spec of the variable
                    const_cast<Variable*>( (const Variable *) var )->setValueTypeSpec( *it );
                }
                return true;
            }
        
        }
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

    for ( std::vector<TypeSpec>::const_iterator it = argTypeSpecs.begin(); it != argTypeSpecs.end(); ++it )
    {
        if ( it != argTypeSpecs.begin() )
            o << "|";
        o << *it;
    }
    o << " " << label;
}

