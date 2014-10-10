/**
 * @file
 * This file contains the implementation of RlTopology, which is
 * a RevBayes wrapper around a Topology.
 *
 * @brief Implementation of Topology
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
#include "RlTopology.h"
#include "RbUtil.h"
#include "RlString.h"
#include "RealPos.h"
#include "TopologyNode.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
Topology::Topology(void) : ModelObject<RevBayesCore::Topology>() {
    
}

/** Construct from core topology */
Topology::Topology(RevBayesCore::Topology *t) : ModelObject<RevBayesCore::Topology>( t ) {
    
}

/** Construct from core topology */
Topology::Topology(const RevBayesCore::Topology &t) : ModelObject<RevBayesCore::Topology>( new RevBayesCore::Topology( t ) ) {
    
}

/** Construct from DAG node */
Topology::Topology(RevBayesCore::TypedDagNode<RevBayesCore::Topology> *n) : ModelObject<RevBayesCore::Topology>( n ) {
    
}



/** Construct */
Topology::Topology(const Topology &t) : ModelObject<RevBayesCore::Topology>( t ) {
    
}


/** Clone object */
Topology* Topology::clone(void) const {
    
	return new Topology(*this);
}


/* Map calls to member methods */
RevLanguage::RevPtr<Variable> Topology::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "nnodes") {
        size_t n = this->dagNode->getValue().getNumberOfNodes();
        return new Variable( new Natural( n ) );
    }
    else if (name == "names") {
        const std::vector<std::string>& n = this->dagNode->getValue().getTipNames();
        RevBayesCore::RbVector<std::string> tmp = RevBayesCore::RbVector<std::string>(n);
        return new Variable( new ModelVector<RlString>( tmp ) );
    } 
    
    return ModelObject<RevBayesCore::Topology>::executeMethod( name, args );
}


/** Get Rev type of object */
const std::string& Topology::getClassType(void) { 
    
    static std::string revType = "Topology";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Topology::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& Topology::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/* Make member methods for this class */
void Topology::initializeMethods( void ) const
{
    // Insert inherited methods
    ModelObject<RevBayesCore::Topology>::initializeMethods();
    
    ArgumentRules* nnodesArgRules = new ArgumentRules();
    this->methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(),       nnodesArgRules              ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    this->methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules              ) );
    
}

