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
#include "RlTimeTree.h"
#include "RbUtil.h"
#include "RlString.h"
#include "RealPos.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
TimeTree::TimeTree(void) : ModelObject<RevBayesCore::TimeTree>() {
    
}

/** Construct from bool */
TimeTree::TimeTree(RevBayesCore::TimeTree *t) : ModelObject<RevBayesCore::TimeTree>( t ) {
    
}

/** Construct from bool */
TimeTree::TimeTree(const RevBayesCore::TimeTree &t) : ModelObject<RevBayesCore::TimeTree>( new RevBayesCore::TimeTree( t ) ) {
    
}

/** Construct from bool */
TimeTree::TimeTree(RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *n) : ModelObject<RevBayesCore::TimeTree>( n ) {
    
}



/** Construct from bool */
TimeTree::TimeTree(const TimeTree &t) : ModelObject<RevBayesCore::TimeTree>( t ) {
    
}


/** Clone object */
TimeTree* TimeTree::clone(void) const {
    
	return new TimeTree(*this);
}


/* Map calls to member methods */
RevLanguage::RevPtr<RevLanguage::Variable> TimeTree::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "nnodes") 
    {
        size_t n = this->dagNode->getValue().getNumberOfNodes();
        return new Variable( new Natural( n ) );
    }
    else if (name == "height") 
    {
        const RevBayesCore::TopologyNode& r = this->dagNode->getValue().getTipNode( 0 );
        return new Variable( new RealPos( r.getTime() ) );
    } 
    else if (name == "names") 
    {
        const std::vector<std::string>& n = this->dagNode->getValue().getTipNames();
        return new Variable( new ModelVector<RlString>( n ) );
    } 
    else if (name == "rescale")
    {
        double f = static_cast<const RealPos&>( args[0].getVariable()->getRevObject() ).getValue();
        RevBayesCore::TimeTree &tree = dagNode->getValue();
        RevBayesCore::TreeUtilities::rescaleTree(&tree, &tree.getRoot(), f);
        
        return NULL;
    }
    
    return ModelObject<RevBayesCore::TimeTree>::executeMethod( name, args );
}


/** Get class name of object */
const std::string& TimeTree::getClassType(void) { 
    
    static std::string rbClassName = "TimeTree";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& TimeTree::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get method specifications */
const RevLanguage::MethodTable& TimeTree::getMethods(void) const 
{
    
    static MethodTable    methods                     = MethodTable();
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false ) 
    {
        
        ArgumentRules* nnodesArgRules = new ArgumentRules();
        methods.addFunction("nnodes", new MemberFunction(Natural::getClassTypeSpec(),          nnodesArgRules   ) );

        ArgumentRules* heightArgRules = new ArgumentRules();
        methods.addFunction("height", new MemberFunction(Natural::getClassTypeSpec(),          heightArgRules   ) );

        ArgumentRules* namesArgRules = new ArgumentRules();
        methods.addFunction("names", new MemberFunction(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules    ) );

        ArgumentRules* rescaleArgRules = new ArgumentRules();
        rescaleArgRules->push_back( new ArgumentRule( "factor", true, RealPos::getClassTypeSpec() ) );
        methods.addFunction("rescale", new MemberFunction(RlUtils::Void,                       rescaleArgRules  ) );

        // necessary call for proper inheritance
        methods.setParentTable( &ModelObject<RevBayesCore::TimeTree>::getMethods() );
        methodsSet = true;
    }
    
    
    return methods;
}


/** Get type spec */
const TypeSpec& TimeTree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

