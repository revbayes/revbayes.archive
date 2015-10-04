
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
#include "RealPos.h"
#include "RlString.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
Clade::Clade(void) : ModelObject<RevBayesCore::Clade>()
{
    
}

/** Construct from core Clade */
Clade::Clade(RevBayesCore::Clade *c) : ModelObject<RevBayesCore::Clade>( c )
{
    
}

/** Construct from core Clade */
Clade::Clade(const RevBayesCore::Clade &t) : ModelObject<RevBayesCore::Clade>( new RevBayesCore::Clade( t ) )
{
    
}

/** Construct from DAG node */
Clade::Clade(RevBayesCore::TypedDagNode<RevBayesCore::Clade> *n) : ModelObject<RevBayesCore::Clade>( n )
{
    
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
    std::vector<RevBayesCore::Taxon> n;
    for (std::vector<RevPtr<const RevVariable> >::iterator it = names.begin(); it != names.end(); ++it) 
    {
        RevBayesCore::Taxon t = RevBayesCore::Taxon(static_cast<const RlString &>( (*it)->getRevObject() ).getValue());
        n.push_back( t );
    }
    
    double a = static_cast<const RealPos &>( age->getRevObject() ).getValue();
    
    dagNode = new RevBayesCore::ConstantNode<RevBayesCore::Clade>("", new RevBayesCore::Clade(n, a));
    dagNode->incrementReferenceCount();
    
}



/** Return member rules (no members) */
const MemberRules& Clade::getParameterRules(void) const {
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        memberRules.push_back( new ArgumentRule("taxonName", RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        memberRules.push_back( new Ellipsis(RlString::getClassTypeSpec() ) );
        memberRules.push_back( new ArgumentRule("age", RealPos::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RealPos(0) ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get Rev type of object */
const std::string& Clade::getClassType(void)
{
    
    static std::string revType = "Clade";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Clade::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& Clade::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Set a member variable */
void Clade::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "taxonName" || name == "") 
    {
        names.push_back( var );
    } 
    else if ( name == "age")
    {
        age = var;
    }
    else
    {
        RevObject::setConstParameter(name, var);
    }
}

