
/**
 * @file
 * This file contains the implementation of RlClade, which is
 * a RevBayes wrapper around a Clade.
 *
 * @brief Implementation of Clade
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

#include "ConstantNode.h"
#include "Ellipsis.h"
#include "ModelVector.h"
#include "RlClade.h"
#include "RbUtil.h"
#include "RlString.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
Clade::Clade(void) : ModelObject<RevBayesCore::Clade>() {
    
}

/** Construct from core Clade */
Clade::Clade(RevBayesCore::Clade *c) : ModelObject<RevBayesCore::Clade>( c ) {
    
}

/** Construct from core Clade */
Clade::Clade(const RevBayesCore::Clade &t) : ModelObject<RevBayesCore::Clade>( new RevBayesCore::Clade( t ) ) {
    
}

/** Construct from DAG node */
Clade::Clade(RevBayesCore::TypedDagNode<RevBayesCore::Clade> *n) : ModelObject<RevBayesCore::Clade>( n ) {
    
}



/** Construct */
Clade::Clade(const Clade &t) : ModelObject<RevBayesCore::Clade>( t ) {
    
}


/** Clone object */
Clade* Clade::clone(void) const {
    
	return new Clade(*this);
}


void Clade::constructInternalObject( void ) 
{
    // we free the memory first
    if ( dagNode != NULL )
    {
        if ( dagNode->decrementReferenceCount() == 0 )
        {
            delete dagNode;
        }
    }
    
    // now allocate a new Clade
    std::vector<std::string> n;
    for (std::vector<RevPtr<const Variable> >::iterator it = names.begin(); it != names.end(); ++it) 
    {
        n.push_back( static_cast<const RlString &>( (*it)->getRevObject() ).getValue() );
    }
    dagNode = new RevBayesCore::ConstantNode<RevBayesCore::Clade>("", new RevBayesCore::Clade(n,0.0));
    dagNode->incrementReferenceCount();
    
}


/* Map calls to member methods */
RevLanguage::RevPtr<Variable> Clade::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
//    if (name == "nnodes") {
//        size_t n = this->value->getValue().getNumberOfNodes();
//        return new Natural( n );
//    }
//    else if (name == "names") {
//        const std::vector<std::string>& n = this->value->getValue().getNames();
//        return new ModelVector<RlString>( n );
//    } 
    
    return ModelObject<RevBayesCore::Clade>::executeMethod( name, args );
}



/** Return member rules (no members) */
const MemberRules& Clade::getMemberRules(void) const {
    
    static MemberRules modelMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        modelMemberRules.push_back( new ArgumentRule("taxonName", true, RlString::getClassTypeSpec() ) );
        modelMemberRules.push_back( new Ellipsis(RlString::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return modelMemberRules;
}


/** Get class name of object */
const std::string& Clade::getClassType(void) { 
    
    static std::string rbClassName = "Clade";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Clade::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get method specifications */
const RevLanguage::MethodTable& Clade::getMethods(void) const {
    
    static MethodTable    methods                     = MethodTable();
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false ) {
        
//        ArgumentRules* nnodesArgRules = new ArgumentRules();
//        methods.addFunction("nnodes", new MemberFunction(Natural::getClassTypeSpec(),       nnodesArgRules              ) );
//        
//        ArgumentRules* namesArgRules = new ArgumentRules();
//        methods.addFunction("names", new MemberFunction(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules              ) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &RevObject::getMethods() );
        methodsSet = true;
    }
    
    
    return methods;
}


/** Get type spec */
const TypeSpec& Clade::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Set a member variable */
void Clade::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "taxonName" || name == "") 
    {
        names.push_back( var );
    } 
    else {
        RevObject::setConstMemberVariable(name, var);
    }
}

