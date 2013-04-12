/**
 * @file
 * This file contains the implementation of RlBoolean, which is
 * a RevBayes wrapper around a regular bool.
 *
 * @brief Implementation of RlBoolean
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-09-04 20:14:58 +0200 (Tue, 04 Sep 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id: RlBoolean.cpp 1793 2012-09-04 18:14:58Z hoehna $
 */


#include "Natural.h"
#include "RlBranchLengthTree.h"
#include "RbUtil.h"
#include "RlString.h"
#include "RealPos.h"
#include "TypeSpec.h"
#include "Vector.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
BranchLengthTree::BranchLengthTree(void) : RlModelVariableWrapper<RevBayesCore::BranchLengthTree>() {
    
}

/** Construct from bool */
BranchLengthTree::BranchLengthTree(RevBayesCore::BranchLengthTree *t) : RlModelVariableWrapper<RevBayesCore::BranchLengthTree>( t ) {
    
}

/** Construct from bool */
BranchLengthTree::BranchLengthTree(const RevBayesCore::BranchLengthTree &t) : RlModelVariableWrapper<RevBayesCore::BranchLengthTree>( new RevBayesCore::BranchLengthTree( t ) ) {
    
}

/** Construct from bool */
BranchLengthTree::BranchLengthTree(RevBayesCore::TypedDagNode<RevBayesCore::BranchLengthTree> *n) : RlModelVariableWrapper<RevBayesCore::BranchLengthTree>( n ) {
    
}



/** Construct from bool */
BranchLengthTree::BranchLengthTree(const BranchLengthTree &t) : RlModelVariableWrapper<RevBayesCore::BranchLengthTree>( t ) {
    
}


/** Clone object */
BranchLengthTree* BranchLengthTree::clone(void) const {
    
	return new BranchLengthTree(*this);
}


/* Map calls to member methods */
RevLanguage::RbLanguageObject* BranchLengthTree::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "nnodes") {
        size_t n = this->value->getValue().getNumberOfNodes();
        return new Natural( n );
    }
    else if (name == "height") {
        const RevBayesCore::TopologyNode& r = this->value->getValue().getTipNode( 0 );
        return new RealPos( r.getBranchLength() );
    } 
    else if (name == "names") {
        const std::vector<std::string>& n = this->value->getValue().getNames();
        return new Vector<RlString>( n );
    } 
    
    return RlModelVariableWrapper<RevBayesCore::BranchLengthTree>::executeMethod( name, args );
}


/** Get class name of object */
const std::string& BranchLengthTree::getClassName(void) { 
    
    static std::string rbClassName = "BranchLengthTree";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& BranchLengthTree::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get method specifications */
const RevLanguage::MethodTable& BranchLengthTree::getMethods(void) const {
    
    static MethodTable    methods                     = MethodTable();
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false ) {
        
        ArgumentRules* nnodesArgRules = new ArgumentRules();
        methods.addFunction("nnodes", new MemberFunction(Natural::getClassTypeSpec(),       nnodesArgRules              ) );
        
        ArgumentRules* heightArgRules = new ArgumentRules();
        methods.addFunction("height", new MemberFunction(Natural::getClassTypeSpec(),       heightArgRules              ) );
        
        ArgumentRules* namesArgRules = new ArgumentRules();
        methods.addFunction("names", new MemberFunction(Vector<RlString>::getClassTypeSpec(),  namesArgRules              ) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &RbLanguageObject::getMethods() );
        methodsSet = true;
    }
    
    
    return methods;
}


/** Get type spec */
const TypeSpec& BranchLengthTree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

