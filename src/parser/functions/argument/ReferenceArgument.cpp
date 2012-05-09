/**
 * @file
 * This file contains the implementation of ReferenceArgument, which is
 * used to hold a potentially labeled ReferenceArgument passed to a
 * function.
 *
 * @brief Implementation of ReferenceArgument
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id: ReferenceArgument.cpp -1   $
 */

#include <sstream>

#include "ReferenceArgument.h"
#include "DAGNode.h"
#include "RbUtil.h"


/** Construct from ReferenceArgument label and DAG node */
ReferenceArgument::ReferenceArgument(const RbPtr<Variable> &v, const std::string& argLabel) : Argument( argLabel ) {
    
    var = v;
    
}


/** Get class name of object */
const std::string& ReferenceArgument::getClassName(void) { 
    
    static std::string rbClassName = "Constant Argument";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ReferenceArgument::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Argument::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& ReferenceArgument::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


const RbPtr<const Variable>& ReferenceArgument::getVariable(void) const {
    return RbPtr<const Variable>( var );
}



/** Complete info about object */
void ReferenceArgument::printValue(std::ostream &o) const {
    
    o << label << " = ";
    var->printValue(o);
    
}

