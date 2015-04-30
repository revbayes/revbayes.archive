#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "CategoricalDistribution.h"
#include "ContinuousStochasticNode.h"
#include "Dist_categorical.h"
#include "RlSimplex.h"
#include "RealPos.h"

using namespace RevLanguage;

Dist_categorical::Dist_categorical() :
    TypedDistribution<Natural>()
{
    setGuiDistributionName("Categorical");
    setGuiDistributionToolTip("Categorical distribution for random variables on the space {1, 2, ..., k}");
}


Dist_categorical::~Dist_categorical()
{
    
}



Dist_categorical* Dist_categorical::clone( void ) const
{
    return new Dist_categorical(*this);
}


RevBayesCore::CategoricalDistribution* Dist_categorical::createDistribution( void ) const
{
    // get the parameters
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >*   q    = static_cast<const Simplex&>( p->getRevObject() ).getDagNode();
    RevBayesCore::CategoricalDistribution*                          d    = new RevBayesCore::CategoricalDistribution( q );
    
    return d;
}


/* Get Rev type of class (static) */
const std::string& Dist_categorical::getClassType( void )
{
    static std::string revType = "Dist_categorical";
    
    return revType;
}


/* Get Rev type spec of class (static) */
const TypeSpec& Dist_categorical::getClassTypeSpec(void)
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/** Return member rules */
const MemberRules& Dist_categorical::getParameterRules(void) const
{
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        memberRules.push_back( new ArgumentRule( "p", Simplex::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get Rev type spec of instance (dynamic) */
const TypeSpec& Dist_categorical::getTypeSpec( void ) const
{
    return getClassTypeSpec();
}


/** Print value for user */
void Dist_categorical::printValue(std::ostream& o) const
{
    
    o << "Categorical( p = ";
    if ( p != NULL )
    {
        if ( p->getName() != "" )
            o << p->getName();
        else
            p->getRevObject().printValue(o);
    }
    else
    {
        o << "?";
    }
    o << " )";
}


/** Set a member variable */
void Dist_categorical::setConstParameter( const std::string& name, const RevPtr<const RevVariable>& var )
{
    if ( name == "p" )
    {
        p = var;
    }
    else
    {
        TypedDistribution<Natural>::setConstParameter( name, var );
    }
}
