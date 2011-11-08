/**
 * @file
 * This file contains the implementation of Func_memoryAllocations, which is
 * the function used to list the content of the workspace.
 *
 * @brief Implementation of Func_memoryAllocations
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

#include "Func_memoryAllocations.h"
#include "RbException.h"
#include "RbMemoryManager.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "UserInterface.h"
#include "VectorString.h"

#include <sstream>


// Definition of the static type spec member
const TypeSpec Func_memoryAllocations::typeSpec(Func_memoryAllocations_name);
const TypeSpec Func_memoryAllocations::returnTypeSpec(RbVoid_name);

/** Clone object */
Func_memoryAllocations* Func_memoryAllocations::clone( void ) const {
    
    return new Func_memoryAllocations( *this );
}


/** Execute function */
RbPtr<RbObject> Func_memoryAllocations::execute( void ) {
    
    size_t nObjects = RbMemoryManager::rbMemoryManager().numberOfRegisteredObjects();
    size_t nReferences = RbMemoryManager::rbMemoryManager().numberOfReferences();
    
    std::string s;
    std::stringstream out;
    out << "Number of objects registered:\t\t" << nObjects << "\n";
    out << "Number of references to objects:\t\t" << nReferences;
    s = out.str();
    
    RBOUT(s);
    
    return RbPtr<RbObject>::getNullPtr();
}


/** Get argument rules */
RbPtr<const ArgumentRules> Func_memoryAllocations::getArgumentRules( void ) const {
    
    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    
    return RbPtr<const ArgumentRules>( argumentRules );
}


/** Get class vector describing type of object */
const VectorString& Func_memoryAllocations::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_memoryAllocations_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_memoryAllocations::getReturnType( void ) const {
    
    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_memoryAllocations::getTypeSpec(void) const {
    return typeSpec;
}

