/**
 * @file
 * This file contains the implementation of Func_ls, which is
 * the function used to list the content of the workspace.
 *
 * @brief Implementation of Func_ls
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "RbBoolean.h"
#include "DAGNode.h"
#include "Func_ls.h"
#include "Model.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"

#include <fstream>


// Definition of the static type spec member
const TypeSpec Func_ls::typeSpec(Func_ls_name);
const TypeSpec Func_ls::returnTypeSpec(RbVoid_name);

/** Clone object */
Func_ls* Func_ls::clone( void ) const {

    return new Func_ls( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_ls::execute( void ) {

    /* Open file */
    const bool printAll = static_cast<const RbBoolean*>( (RbLanguageObject*)(*args)[0]->getValue() )->getValue();
    
    std::cout << "User workspace:" << std::endl;
    std::cout << "===============" << std::endl;
    std::cout << std::endl;

    Workspace::userWorkspace()->printValue( std::cout );
    std::cout << std::endl;
    
    if ( printAll == true ) {
        
        std::cout << "Global workspace:" << std::endl;
        std::cout << "===============" << std::endl;
        std::cout << std::endl;

        Workspace::globalWorkspace()->printValue( std::cout );
        std::cout << std::endl;
    }

    return RbPtr<RbLanguageObject>::getNullPtr();
}


/** Get argument rules */
RbPtr<const ArgumentRules> Func_ls::getArgumentRules( void ) const {

    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    static bool          rulesSet = false;

    if ( !rulesSet ) {

        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "all", RbPtr<RbLanguageObject>( new RbBoolean( false ) ) ) ) );
        rulesSet = true;
    }

    return RbPtr<const ArgumentRules>( argumentRules );
}


/** Get class vector describing type of object */
const VectorString& Func_ls::getClass( void ) const {

    static VectorString rbClass = VectorString( Func_ls_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_ls::getReturnType( void ) const {

    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_ls::getTypeSpec(void) const {
    return typeSpec;
}

