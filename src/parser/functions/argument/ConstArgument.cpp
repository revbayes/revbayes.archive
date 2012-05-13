/**
 * @file
 * This file contains the implementation of ConstArgument, which is
 * used to hold a potentially labeled ConstArgument passed to a
 * function.
 *
 * @brief Implementation of ConstArgument
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id: ConstArgument.cpp -1   $
 */

#include "ConstArgument.h"
#include "DAGNode.h"
#include "RbPtr.h"
#include "RbUtil.h"
#include "Variable.h"

#include <sstream>


/** Construct from ConstArgument label and DAG node */
ConstArgument::ConstArgument(const RbPtr<const Variable> &v, const std::string& argLabel) : Argument( argLabel ) {
    
    var = v;
    
}


ConstArgument* ConstArgument::clone( void ) const {
    return new ConstArgument( *this );
}


/** Get class name of object */
const std::string& ConstArgument::getClassName(void) { 
    
    static std::string rbClassName = "Constant Argument";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ConstArgument::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Argument::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& ConstArgument::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


const RbPtr<const Variable>& ConstArgument::getVariable(void) const {
    return var;
}



/** Complete info about object */
void ConstArgument::printValue(std::ostream &o) const {
    
    o << label << " = const ";
    var->printValue(o);
    
}

