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
#include "MethodTable.h"
#include "RbException.h"
#include "Function.h"
#include "RbUtil.h"
#include "MemberProcedure.h"

#include <sstream>

using namespace RevLanguage;

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
void MethodTable::addFunction( const std::string name, Function *func ) {

    FunctionTable::addFunction( name, func );

    if ( func->isTypeSpec( MemberProcedure::getClassTypeSpec() ) )
        static_cast<MemberProcedure*>( (Function*)func )->setMethodName( name );
    
//    if ( func->isTypeSpec( DagNodeFunction::getClassTypeSpec() ) )
//        static_cast<DagNodeFunction*>( (Function*)func )->setMethodName( name );
}

