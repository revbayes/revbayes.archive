/**
 * @file
 * This file contains the declaration of MethodTable, which is
 * used to hold member functions of complex objects.
 *
 * @brief Implementation of MethodTable
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "DagNodeFunction.h"
#include "MemberFunction.h"
#include "MethodTable.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbUtil.h"

#include <sstream>


/** Basic constructor, empty table with or without parent */
MethodTable::MethodTable(MethodTable* parent) : FunctionTable( parent ) {

}

/** Assignment operator */
MethodTable& MethodTable::operator=(const MethodTable& x) {
    
    if (this != &x) {
        
        FunctionTable::operator=(x);
    }
    
    return (*this);
}



/** Add function to table */
void MethodTable::addFunction( const std::string name, RbFunction* func ) {

    FunctionTable::addFunction( name, func );

    if ( func->isTypeSpec( MemberFunction::getClassTypeSpec() ) )
        static_cast<MemberFunction*>( func )->setMethodName( name );
    
    if ( func->isTypeSpec( DagNodeFunction::getClassTypeSpec() ) )
        static_cast<DagNodeFunction*>( func )->setMethodName( name );
}


/** Get class name of object */
const std::string& MethodTable::getClassName(void) { 
    
    static std::string rbClassName = "Method table";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& MethodTable::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( FunctionTable::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& MethodTable::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Complete info about object */
std::string MethodTable::debugInfo(void) const {

    std::ostringstream o;
    if (table.size() == 0)
        o << "MethodTable with no entries" << std::endl;
    else
        o << "MethodTable with " << table.size() << " entries:" << std::endl;
    printValue(o);

    return o.str();
}

