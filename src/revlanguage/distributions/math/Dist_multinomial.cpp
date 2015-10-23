#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ContinuousStochasticNode.h"
#include "Dist_multinomial.h"
#include "MultinomialDistribution.h"
#include "Probability.h"
#include "RbVector.h"
#include "RealPos.h"
#include "RlSimplex.h"

using namespace RevLanguage;

Dist_multinomial::Dist_multinomial(void) : TypedDistribution< ModelVector<Natural> >()
{
    
}


Dist_multinomial::~Dist_multinomial(void)
{
    
}



Dist_multinomial* Dist_multinomial::clone( void ) const
{
    
    return new Dist_multinomial(*this);
}


RevBayesCore::MultinomialDistribution* Dist_multinomial::createDistribution( void ) const
{
    
    // get the parameters
    RevBayesCore::TypedDagNode<int>*                              vn = static_cast<const Natural     &>( n->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* vp = static_cast<const Simplex &>( p->getRevObject() ).getDagNode();
    RevBayesCore::MultinomialDistribution*                        d  = new RevBayesCore::MultinomialDistribution( vp, vn );
    return d;
}



/* Get Rev type of object */
const std::string& Dist_multinomial::getClassType(void)
{
    
    static std::string revType = "Dist_multinomial";
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_multinomial::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution< ModelVector<Natural> >::getClassTypeSpec() ) );
    return revTypeSpec;
}




/** Return member rules (no members) */
const MemberRules& Dist_multinomial::getParameterRules(void) const
{
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        distMemberRules.push_back( new ArgumentRule( "p", Simplex::getClassTypeSpec(), "The simplex of probabilities for the categories.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        distMemberRules.push_back( new ArgumentRule( "n", Natural::getClassTypeSpec(), "The number of draws.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        rulesSet = true;
    }
    
    return distMemberRules;
}


const TypeSpec& Dist_multinomial::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    return ts;
}


/** Print value for user */
void Dist_multinomial::printValue(std::ostream& o) const
{
    
    o << "multinomial(p=";
    if ( p != NULL )
        o << p->getName();
    else
        o << "?";
    o << ", n=";
    if ( n != NULL )
        o << n->getName();
    else
        o << "?";
    o << ")";
}


/** Set a member variable */
void Dist_multinomial::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "p" )
    {
        p = var;
    }
    else if ( name == "n" )
    {
        n = var;
    }
    else
    {
        TypedDistribution< ModelVector<Natural> >::setConstParameter(name, var);
    }
}
