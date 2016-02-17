#include "Dist_phyloDistanceGamma.h"
#include "PhyloDistanceGamma.h"
#include "OptionRule.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlRateGenerator.h"
#include "RlString.h"
#include "RlTree.h"

using namespace RevLanguage;


Dist_phyloDistanceGamma::Dist_phyloDistanceGamma() : TypedDistribution< RlDistanceMatrix >()
{
    
}


Dist_phyloDistanceGamma::~Dist_phyloDistanceGamma()
{
    
}



Dist_phyloDistanceGamma* Dist_phyloDistanceGamma::clone( void ) const
{
    
    return new Dist_phyloDistanceGamma(*this);
}


RevBayesCore::TypedDistribution< RevBayesCore::DistanceMatrix >* Dist_phyloDistanceGamma::createDistribution( void ) const
{
    
    // get the parameters tau, nam, varianceNode and distanceNode that will be used to create the actual distribution.
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const Tree &>( tree->getRevObject() ).getDagNode();
    
    const std::vector<std::string>      &nam  = static_cast<const ModelVector<RlString> &>( names->getRevObject() ).getDagNode()->getValue();
    
    RevBayesCore::TypedDagNode< RevBayesCore::DistanceMatrix >* varianceNode = NULL;
    varianceNode = static_cast<const RlDistanceMatrix &>( varianceMatrix->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode< RevBayesCore::DistanceMatrix >* distanceNode = NULL;
    distanceNode = static_cast<const RlDistanceMatrix &>( distanceMatrix->getRevObject() ).getDagNode();
    
    RevBayesCore::PhyloDistanceGamma* d = new RevBayesCore::PhyloDistanceGamma( tau );
    
    d->setNames( nam );
    d->setVarianceMatrix( varianceNode );
    d->setDistanceMatrix( distanceNode );
    
    d->redrawValue();
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_phyloDistanceGamma::getClassType(void)
{
    
    static std::string revType = "Dist_phyloDistanceGamma";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_phyloDistanceGamma::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution< RlDistanceMatrix >::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_phyloDistanceGamma::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "PhyloDistanceGamma";
    
    return d_name;
}


/** Return member rules (no members) */
const MemberRules& Dist_phyloDistanceGamma::getParameterRules(void) const
{
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        distMemberRules.push_back( new ArgumentRule( "tree"             , Tree::getClassTypeSpec()                  , "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distMemberRules.push_back( new ArgumentRule( "distanceMatrix"   , RlDistanceMatrix::getClassTypeSpec()      , "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distMemberRules.push_back( new ArgumentRule( "varianceMatrix"   , RlDistanceMatrix::getClassTypeSpec()      , "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distMemberRules.push_back( new ArgumentRule( "names"            , ModelVector<RlString>::getClassTypeSpec() , "", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return distMemberRules;
}


const TypeSpec& Dist_phyloDistanceGamma::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_phyloDistanceGamma::printValue(std::ostream& o) const
{
    
    o << "Distance-Matrix-Generation-Along-Tree Process(tree=";
    if ( tree != NULL ) {
        o << tree->getName();
    } else {
        o << "?";
    }
    o << ", distanceMatrix=";
    if ( distanceMatrix != NULL ) {
        o << distanceMatrix->getName();
    } else {
        o << "?";
    }
    o << ", varianceMatrix=";
    if ( varianceMatrix != NULL ) {
        o << varianceMatrix->getName();
    } else {
        o << "?";
    }
    o << ", names=";
    if ( names != NULL ) {
        o << names->getName();
    } else {
        o << "?";
    }
    o << ")";
    
}


/** Set a member variable */
void Dist_phyloDistanceGamma::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "distanceMatrix" )
    {
        distanceMatrix = var;
    }
    else if ( name == "varianceMatrix" )
    {
        varianceMatrix = var;
    }
    else if ( name == "names" )
    {
        names = var;
    }
    else
    {
        TypedDistribution< RlDistanceMatrix >::setConstParameter(name, var);
    }
    
}

