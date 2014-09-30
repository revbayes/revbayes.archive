/**
 * @file
 * This file contains the implementation of RlTree.
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
 */


#include "ModelVector.h"
#include "Natural.h"
#include "RlTree.h"
#include "RbUtil.h"
#include "RlString.h"
#include "RealPos.h"
#include "TypeSpec.h"
#include "Topology.h"
#include "RlTopology.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
Tree::Tree(void) : ModelObject<RevBayesCore::Tree>() {
    
}

/** Construct from bool */
Tree::Tree(RevBayesCore::Tree *t) : ModelObject<RevBayesCore::Tree>( t ) {
    
}



/** Construct from bool */
Tree::Tree(RevBayesCore::TypedDagNode<RevBayesCore::Tree> *n) : ModelObject<RevBayesCore::Tree>( n ) {
    
}



/** Construct from bool */
Tree::Tree(const Tree &t) : ModelObject<RevBayesCore::Tree>( t ) {
    
}


/** Clone object */
Tree* Tree::clone(void) const {
    
	return new Tree(*this);
}


/* Map calls to member methods */
RevLanguage::RevPtr<RevLanguage::Variable> Tree::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "nnodes") {
        size_t n = this->dagNode->getValue().getNumberOfNodes();
        return new Variable( new Natural( n ) );
    }
    else if (name == "names") {
        const std::vector<std::string>& n = this->dagNode->getValue().getTipNames();
        return new Variable( new ModelVector<RlString>( n ) );
    } 
	else if (name == "topology") {
        const RevBayesCore::Topology& t = this->dagNode->getValue().getTopology();
        return new Variable( new RevLanguage::Topology( t ) );
    } 
    
    return ModelObject<RevBayesCore::Tree>::executeMethod( name, args );
}


/** Get Rev type of object */
const std::string& Tree::getClassType(void) { 
    
    static std::string revType = "Tree";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Tree::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get member methods. We construct the appropriate static member
 * function table here.
 */
const RevLanguage::MethodTable& Tree::getMethods( void ) const
{
    static MethodTable  myMethods   = MethodTable();
    static bool         methodsSet  = false;
    
    if ( !methodsSet )
    {
        myMethods = makeMethods();
        methodsSet = true;
    }
    
    return myMethods;
}


/** Get type spec */
const TypeSpec& Tree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Make member methods for this class */
RevLanguage::MethodTable Tree::makeMethods( void ) const
{
    MethodTable methods = MethodTable();
    
    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(),       nnodesArgRules              ) );
        
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules       ) );
	
	ArgumentRules* topologyArgRules = new ArgumentRules();
    methods.addFunction("topology", new MemberProcedure(RevLanguage::Topology::getClassTypeSpec(),  topologyArgRules       ) );
	
	
    // Insert inherited methods
    methods.insertInheritedMethods( ModelObject<RevBayesCore::Tree>::makeMethods() );
	
    return methods;
}


