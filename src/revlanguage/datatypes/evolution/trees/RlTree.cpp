///**
// * @file
// * This file contains the implementation of RlBoolean, which is
// * a RevBayes wrapper around a regular bool.
// *
// * @brief Implementation of RlBoolean
// *
// * (c) Copyright 2009-
// * @date Last modified: $Date: 2012-09-04 20:14:58 +0200 (Tue, 04 Sep 2012) $
// * @author The RevBayes Development Core Team
// * @license GPL version 3
// * @version 1.0
// * @since 2009-11-20, version 1.0
// * @extends RbObject
// *
// * $Id: RlBoolean.cpp 1793 2012-09-04 18:14:58Z hoehna $
// */
//
//
//#include "Natural.h"
//#include "RlTree.h"
//#include "RbUtil.h"
//#include "RlString.h"
//#include "RealPos.h"
//#include "TopologyNode.h"
//#include "TypeSpec.h"
//#include "Vector.h"
//
//#include <sstream>
//
//using namespace RevLanguage;
//
///** Default constructor */
//Tree::Tree(void) : RlModelVariableWrapper<RevBayesCore::Tree>() {
//    
//}
//
///** Construct from bool */
//Tree::Tree(RevBayesCore::Tree *t) : RlModelVariableWrapper<RevBayesCore::Tree>( t ) {
//    
//}
//
///** Construct from bool */
//Tree::Tree(const RevBayesCore::Tree &t) : RlModelVariableWrapper<RevBayesCore::Tree>( t.clone() ) {
//    
//}
//
///** Construct from bool */
//Tree::Tree(RevBayesCore::TypedDagNode<RevBayesCore::Tree> *n) : RlModelVariableWrapper<RevBayesCore::Tree>( n ) {
//    
//}
//
//
//
///** Construct from bool */
//Tree::Tree(const Tree &t) : RlModelVariableWrapper<RevBayesCore::Tree>( t ) {
//    
//}
//
//
///** Clone object */
//Tree* Tree::clone(void) const {
//    
//	return new Tree(*this);
//}
//
//
///* Map calls to member methods */
//RevLanguage::RbLanguageObject* Tree::executeMethod(std::string const &name, const std::vector<Argument> &args) {
//    
//    if (name == "nnodes") {
//        size_t n = this->value->getValue().getNumberOfNodes();
//        return new Natural( n );
//    }
//    else if (name == "height") {
//        const RevBayesCore::TopologyNode& r = this->value->getValue().getTipNode( 0 );
//        return new RealPos( r.getTime() );
//    } 
//    else if (name == "names") {
//        const std::vector<std::string>& n = this->value->getValue().getNames();
//        return new Vector<RlString>( n );
//    } 
//    
//    return RlModelVariableWrapper<RevBayesCore::Tree>::executeMethod( name, args );
//}
//
//
///** Get class name of object */
//const std::string& Tree::getClassName(void) { 
//    
//    static std::string rbClassName = "Tree";
//    
//	return rbClassName; 
//}
//
///** Get class type spec describing type of object */
//const TypeSpec& Tree::getClassTypeSpec(void) { 
//    
//    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
//    
//	return rbClass; 
//}
//
//
///* Get method specifications */
//const RevLanguage::MethodTable& Tree::getMethods(void) const {
//    
//    static MethodTable    methods                     = MethodTable();
//    static bool           methodsSet                  = false;
//    
//    if ( methodsSet == false ) {
//        
//        ArgumentRules* nnodesArgRules = new ArgumentRules();
//        methods.addFunction("nnodes", new MemberFunction(Natural::getClassTypeSpec(),       nnodesArgRules              ) );
//        
//        ArgumentRules* heightArgRules = new ArgumentRules();
//        methods.addFunction("height", new MemberFunction(Natural::getClassTypeSpec(),       heightArgRules              ) );
//        
//        ArgumentRules* namesArgRules = new ArgumentRules();
//        methods.addFunction("names", new MemberFunction(Vector<RlString>::getClassTypeSpec(),  namesArgRules              ) );
//        
//        // necessary call for proper inheritance
//        methods.setParentTable( &RlModelVariableWrapper<RevBayesCore::Tree>::getMethods() );
//        methodsSet = true;
//    }
//    
//    
//    return methods;
//}
//
//
///** Get type spec */
//const TypeSpec& Tree::getTypeSpec( void ) const {
//    
//    static TypeSpec typeSpec = getClassTypeSpec();
//    
//    return typeSpec;
//}
//
