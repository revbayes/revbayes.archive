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
#include "MemberFunction.h"
#include "MethodTable.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbUtil.h"
#include "VectorString.h"

#include <sstream>


// Definition of the static type spec member
const TypeSpec MethodTable::typeSpec(MethodTable_name);

/** Basic constructor, empty table with or without parent */
MethodTable::MethodTable(RbPtr<MethodTable> parent) : FunctionTable( RbPtr<FunctionTable>(parent.get()) ) {

}

/** Assignment operator */
MethodTable& MethodTable::operator=(const MethodTable& x) {
    
    if (this != &x) {
        
        table.clear();
        for (std::multimap<std::string, RbFunction* >::const_iterator i=x.table.begin(); i!=x.table.end(); i++)
            table.insert(std::pair<std::string, RbFunction*>((*i).first, (RbFunction*)((*i).second->clone())));
        
        parentTable = x.parentTable;
    }
    
    return (*this);
}



/** Add function to table */
void MethodTable::addFunction( const std::string name, RbPtr<RbFunction> func ) {

    FunctionTable::addFunction( name, func );

    if ( func->isType( MemberFunction_name ) )
        static_cast<MemberFunction*>( func.get() )->setMethodName( name );
}


/** Return brief info about object */
std::string MethodTable::briefInfo () const {

    std::ostringstream   o;
    o << "MethodTable with " << table.size() << " functions";

    return o.str();
}


/** Get class vector describing type of object */
const VectorString& MethodTable::getClass(void) const { 
    
    static VectorString rbClass = VectorString(MethodTable_name) + FunctionTable::getClass();
	return rbClass; 
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& MethodTable::getTypeSpec(void) const {
    return typeSpec;
}


/** Complete info about object */
std::string MethodTable::richInfo(void) const {

    std::ostringstream o;
    if (table.size() == 0)
        o << "MethodTable with no entries" << std::endl;
    else
        o << "MethodTable with " << table.size() << " entries:" << std::endl;
    printValue(o);

    return o.str();
}

