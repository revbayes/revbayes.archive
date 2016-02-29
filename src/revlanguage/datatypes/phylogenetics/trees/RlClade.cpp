
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


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Clade* Clade::clone(void) const
{
    
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
    
    
    RevBayesCore::Clade *c = new RevBayesCore::Clade(n);
    
    // set the age if provided
    if ( age->getRevObject() != RevNullObject::getInstance() )
    {
        double a = static_cast<const RealPos &>( age->getRevObject() ).getValue();
        c->setAge( a );
    }
    
    // set the number of missing if provided
    if ( missing->getRevObject() != RevNullObject::getInstance() )
    {
        int n = static_cast<const Natural &>( missing->getRevObject() ).getValue();
        c->setNumberMissingTaxa( n );
    }

    dagNode = new RevBayesCore::ConstantNode<RevBayesCore::Clade>("", c);
    dagNode->incrementReferenceCount();
    
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string Clade::getConstructorFunctionName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "clade";
    
    return c_name;
}


/** Return member rules (no members) */
const MemberRules& Clade::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        memberRules.push_back( new ArgumentRule("taxonName", RlString::getClassTypeSpec(), "A first taxon.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        memberRules.push_back( new Ellipsis( "Additional taxa.", RlString::getClassTypeSpec() ) );
        memberRules.push_back( new ArgumentRule("age", RealPos::getClassTypeSpec(), "The age of the clade (optional).", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        memberRules.push_back( new ArgumentRule("missing", Natural::getClassTypeSpec(), "Number of missing taxa in the clade (optional).", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        
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
const TypeSpec& Clade::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( ModelObject<RevBayesCore::Clade>::getClassTypeSpec() ) );
    
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
    else if ( name == "missing")
    {
        missing = var;
    }
    else
    {
        RevObject::setConstParameter(name, var);
    }
}

