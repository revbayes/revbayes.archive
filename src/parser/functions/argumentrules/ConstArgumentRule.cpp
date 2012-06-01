///**
// * @file
// * This file contains the implementation of ConstArgumentRule, which is
// * used for argument rules pertaining to value variables.
// *
// * @brief Implementation of ConstArgumentRule 
// *
// * (c) Copyright 2009-
// * @date Last modified: $Date$
// * @author The RevBayes Development Core Team
// * @license GPL version 3
// *
// * $Id$
// */
//
//#include "RbUtil.h"
//#include "ConstArgumentRule.h"
//#include "ConstantNode.h"
//
//#include <sstream>
//#include <typeinfo>
//
//
///** Construct rule based on default value; use "" for no label. */
//ConstArgumentRule::ConstArgumentRule( const std::string& argName, const RbPtr<RbLanguageObject> &defVal ) : ArgumentRule( argName, defVal->getTypeSpec() ), defaultVar( new ConstantNode( defVal ) ) {
//    defaultVar = Variable( new ConstantNode( defVal ) );
//    hasDefaultVal = true;
//}
//
//
///** Construct rule with default value. We rely on workspace to check the provided type specification. */
//ConstArgumentRule::ConstArgumentRule( const std::string& argName, const TypeSpec& argTypeSp ) : ArgumentRule( argName, argTypeSp ), defaultVar( NULL ) {
//    
//    hasDefaultVal = false;
//    
//}
//
///** Construct rule with default value. We rely on workspace to check the provided type specification. */
//ConstArgumentRule::ConstArgumentRule( const std::string& argName, const TypeSpec& argTypeSp, const RbPtr<RbLanguageObject> &defVal ) : ArgumentRule( argName, argTypeSp ), defaultVar( new ConstantNode( defVal ) ) {
//    
//    hasDefaultVal = true;
//    
//}
//
//
//ConstArgumentRule* ConstArgumentRule::clone( void ) const {
//    return new ConstArgumentRule( *this );
//}
//
//
///** Get class name of object */
//const std::string& ConstArgumentRule::getClassName(void) { 
//    
//    static std::string rbClassName = "constant argument rule";
//    
//	return rbClassName; 
//}
//
///** Get class type spec describing type of object */
//const TypeSpec& ConstArgumentRule::getClassTypeSpec(void) { 
//    
//    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( ArgumentRule::getClassTypeSpec() ) );
//    
//	return rbClass; 
//}
//
///** Get type spec */
//const TypeSpec& ConstArgumentRule::getTypeSpec( void ) const {
//    
//    static TypeSpec typeSpec = getClassTypeSpec();
//    
//    return typeSpec;
//}
//
//const Variable& ConstArgumentRule::getDefaultVariable( void ) const {
//    return defaultVar;
//}
//
//
//
///* Print value for user (in descriptions of functions, for instance) */
//void ConstArgumentRule::printValue(std::ostream &o) const {
//    
//    o << argTypeSpec;
//    o << " \"" << label << "\" = ";
//    defaultVar.printValue( o );
//}
//
//
//
