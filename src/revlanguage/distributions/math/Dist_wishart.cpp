#include "ArgumentRule.h"
#include "Dist_wishart.h"
#include "Natural.h"
#include "RealPos.h"
#include "RlMatrixRealSymmetric.h"
#include "StochasticNode.h"
#include "WishartDistribution.h"

using namespace RevLanguage;

Dist_wishart::Dist_wishart() : TypedDistribution<MatrixRealSymmetric>()
{
    
}


Dist_wishart::~Dist_wishart()
{
    
}



Dist_wishart* Dist_wishart::clone( void ) const
{
    return new Dist_wishart(*this);
}


RevBayesCore::WishartDistribution* Dist_wishart::createDistribution( void ) const
{
    
    // get the parameters
    RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal>* om = NULL;
    RevBayesCore::TypedDagNode<double>* ka = NULL;
    RevBayesCore::TypedDagNode<long>* deg = NULL;
    RevBayesCore::TypedDagNode<long>* dm = NULL;

    if ( omega->getRevObject() != RevNullObject::getInstance() )
    {
        om = static_cast<const MatrixRealSymmetric &>( omega->getRevObject() ).getDagNode();
    }
    
    if ( kappa->getRevObject() != RevNullObject::getInstance() )
    {
        ka = static_cast<const RealPos&>( kappa->getRevObject() ).getDagNode();
    }
    
    if ( df->getRevObject() != RevNullObject::getInstance() )
    {
        deg = static_cast<const Natural &>( df->getRevObject()).getDagNode();
    }

    if ( dim->getRevObject() != RevNullObject::getInstance() )
    {
        dm = static_cast<const Natural &>( dim->getRevObject()).getDagNode();
    }
    
    RevBayesCore::WishartDistribution* w    =  NULL;
    
    if ( om != NULL && om->getValue().getDim() != 0 )
    {
        // parameter is sigma
        w = new RevBayesCore::WishartDistribution( om, deg );
    }
    else
    {
        // parameter is kappa * Id
        w = new RevBayesCore::WishartDistribution( dm, ka, deg );
    }
    
    return w;
    
}



/* Get Rev type of object */
const std::string& Dist_wishart::getClassType(void)
{
    
    static std::string rev_type = "Dist_wishart";
    
	return rev_type;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_wishart::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rev_type_spec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_wishart::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "Wishart";
    
    return d_name;
}


/** Return member rules (no members) */
const MemberRules& Dist_wishart::getParameterRules(void) const
{
    
    static MemberRules dist_member_rules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        
        dist_member_rules.push_back( new ArgumentRule( "omega", MatrixRealSymmetric::getClassTypeSpec() , "The scale matrix.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL  ) );
        dist_member_rules.push_back( new ArgumentRule( "df"   , Natural::getClassTypeSpec(), "The degrees of dreedom.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        dist_member_rules.push_back( new ArgumentRule( "kappa", RealPos::getClassTypeSpec(), "The scaling parameter.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        dist_member_rules.push_back( new ArgumentRule( "dim"  , Natural::getClassTypeSpec(), "The dimension of the distribution.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        
        rules_set = true;
    }
    
    return dist_member_rules;
}


const TypeSpec& Dist_wishart::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_wishart::printValue(std::ostream& o) const {
    
    o << " Wishart(omega=";
/*
    if ( omega != NULL ) {
        o << omega->getName();
    } else {
*/
 if (kappa != NULL)  {
            if (dim == NULL)    {
                throw RbException("error in Wishart distribution: kappa and dim should both be non null");
            }
            o << kappa->getName() << ".I_" << dim->getName();
        }
        else{
            o << "?";
        }
   // }
    o << ")";
}


/** Set a member variable */
void Dist_wishart::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "omega" )
    {
        omega = var;
    }
    else if ( name == "kappa" )
    {
        kappa = var;
    }
    else if ( name == "df" )
    {
        df = var;
    }
    else if ( name == "dim" )
    {
        dim = var;
    }
    else
    {
        Distribution::setConstParameter(name, var);
    }
}
