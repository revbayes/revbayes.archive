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


#include "ModelVector.h"
#include "Natural.h"
#include "RlBranchLengthTree.h"
#include "RbUtil.h"
#include "RlString.h"
#include "RealPos.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
BranchLengthTree::BranchLengthTree(void) : ModelObject<RevBayesCore::BranchLengthTree>() {
    
}

/** Construct from bool */
BranchLengthTree::BranchLengthTree(RevBayesCore::BranchLengthTree *t) : ModelObject<RevBayesCore::BranchLengthTree>( t ) {
    
}

/** Construct from bool */
BranchLengthTree::BranchLengthTree(const RevBayesCore::BranchLengthTree &t) : ModelObject<RevBayesCore::BranchLengthTree>( new RevBayesCore::BranchLengthTree( t ) ) {
    
}

/** Construct from bool */
BranchLengthTree::BranchLengthTree(RevBayesCore::TypedDagNode<RevBayesCore::BranchLengthTree> *n) : ModelObject<RevBayesCore::BranchLengthTree>( n ) {
    
}



/** Construct from bool */
BranchLengthTree::BranchLengthTree(const BranchLengthTree &t) : ModelObject<RevBayesCore::BranchLengthTree>( t ) {
    
}


/** Clone object */
BranchLengthTree* BranchLengthTree::clone(void) const {
    
	return new BranchLengthTree(*this);
}


/* Map calls to member methods */
RevLanguage::RevPtr<RevLanguage::Variable> BranchLengthTree::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "nnodes") {
        size_t n = this->dagNode->getValue().getNumberOfNodes();
        return new Variable( new Natural( n ) );
    }
    else if (name == "height") {
        const RevBayesCore::TopologyNode& r = this->dagNode->getValue().getTipNode( 0 );
        return new Variable( new RealPos( r.getBranchLength() ) );
    } 
    else if (name == "names") {
        const std::vector<std::string>& n = this->dagNode->getValue().getTipNames();
        return new Variable( new ModelVector<RlString>( n ) );
    } 
    
    return ModelObject<RevBayesCore::BranchLengthTree>::executeMethod( name, args );
}


/** Get Rev type of object */
const std::string& BranchLengthTree::getClassType(void) { 
    
    static std::string revType = "BranchLengthTree";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& BranchLengthTree::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& BranchLengthTree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Make member methods for this class */
void BranchLengthTree::initializeMethods( void ) const
{

    // Insert inherited methods
    ModelObject<RevBayesCore::BranchLengthTree>::initializeMethods();
    
    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(),       nnodesArgRules              ) );
    
    ArgumentRules* heightArgRules = new ArgumentRules();
    methods.addFunction("height", new MemberProcedure(Natural::getClassTypeSpec(),       heightArgRules              ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules       ) );
    
}


