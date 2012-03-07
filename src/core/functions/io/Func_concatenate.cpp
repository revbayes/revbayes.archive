/**
 * @file
 * This file contains the implementation of Func_concatenate, a class
 * used to concatenate character matrices.
 *
 * @brief Implementation of Func_concatenate
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-02-25 01:17:07 -0800 (Sat, 25 Feb 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id: Func_concatenate.cpp 1247 2012-02-25 09:17:07Z hoehna $
 */

#include "CharacterData.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Func_concatenate.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "StringUtilities.h"
#include "UserInterface.h"
#include "ValueRule.h"
#include "VectorString.h"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>


/** Clone object */
Func_concatenate* Func_concatenate::clone(void) const {

    return new Func_concatenate(*this);
}


/** Execute function */
const RbLanguageObject& Func_concatenate::executeFunction(void) {

    // get the information from the arguments for reading the file
    //CharacterData& m = static_cast<CharacterData&>( data->getValue() );
    //m.printValue(std::cout);
    for (size_t i = 0; i < elements.size(); i++)
        elements[i]->getValue().printValue(std::cout);
        
    CharacterData& m0 = static_cast<CharacterData&>( elements[0]->getValue() );

    CharacterData* concatenatedMatrix = new CharacterData( m0.getDataType() );
    
    return *concatenatedMatrix;
}


/** Get argument rules */
const ArgumentRules& Func_concatenate::getArgumentRules(void) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet      = false;

    if (!rulesSet)
        {
        //argumentRules.push_back( new ValueRule( "data",  CharacterData::getClassTypeSpec() ) );
        argumentRules.push_back( new Ellipsis( RbLanguageObject::getClassTypeSpec() ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class name of object */
const std::string& Func_concatenate::getClassName(void) { 
    
    static std::string rbClassName = "Matrix concatenation function";
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_concatenate::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_concatenate::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_concatenate::getReturnType(void) const {
    
    static TypeSpec returnTypeSpec = CharacterData::getClassTypeSpec();
    return returnTypeSpec;
}

/** We catch here the setting of the argument variables to store our parameters. */
void Func_concatenate::setArgumentVariable(std::string const &name, const RbVariablePtr& var) {
    
    if ( name == "data" )
        data = var;
    else if ( name == "" )
        elements.push_back(var);
    else
        RbFunction::setArgumentVariable(name, var);
}
