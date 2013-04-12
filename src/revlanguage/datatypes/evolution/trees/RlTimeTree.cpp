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
#include "RlTimeTree.h"
#include "RbUtil.h"
#include "RlString.h"
#include "RealPos.h"
#include "TopologyNode.h"
#include "TypeSpec.h"
#include "Vector.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
TimeTree::TimeTree(void) : RlModelVariableWrapper<RevBayesCore::TimeTree>() {
    
}

/** Construct from bool */
TimeTree::TimeTree(RevBayesCore::TimeTree *t) : RlModelVariableWrapper<RevBayesCore::TimeTree>( t ) {
    
}

/** Construct from bool */
TimeTree::TimeTree(const RevBayesCore::TimeTree &t) : RlModelVariableWrapper<RevBayesCore::TimeTree>( new RevBayesCore::TimeTree( t ) ) {
    
}

/** Construct from bool */
TimeTree::TimeTree(RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *n) : RlModelVariableWrapper<RevBayesCore::TimeTree>( n ) {
    
}



/** Construct from bool */
TimeTree::TimeTree(const TimeTree &t) : RlModelVariableWrapper<RevBayesCore::TimeTree>( t ) {
    
}


/** Clone object */
TimeTree* TimeTree::clone(void) const {
    
	return new TimeTree(*this);
}


/* Map calls to member methods */
RevLanguage::RbLanguageObject* TimeTree::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "nnodes") {
        size_t n = this->value->getValue().getNumberOfNodes();
        return new Natural( n );
    }
    else if (name == "height") {
        const RevBayesCore::TopologyNode& r = this->value->getValue().getTipNode( 0 );
        return new RealPos( r.getTime() );
    } 
    else if (name == "names") {
        const std::vector<std::string>& n = this->value->getValue().getNames();
        return new Vector<RlString>( n );
    } 
    
    return RlModelVariableWrapper<RevBayesCore::TimeTree>::executeMethod( name, args );
}


/** Get class name of object */
const std::string& TimeTree::getClassName(void) { 
    
    static std::string rbClassName = "TimeTree";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& TimeTree::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get method specifications */
const RevLanguage::MethodTable& TimeTree::getMethods(void) const {
    
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
        methods.setParentTable( &RlModelVariableWrapper<RevBayesCore::TimeTree>::getMethods() );
        methodsSet = true;
    }
    
    
    return methods;
}


/** Get type spec */
const TypeSpec& TimeTree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

