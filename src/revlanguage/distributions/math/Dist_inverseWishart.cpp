#include "ArgumentRule.h"
#include "Dist_inverseWishart.h"
#include "Natural.h"
#include "RealPos.h"
#include "ModelVector.h"
#include "RlMatrixRealSymmetric.h"
#include "StochasticNode.h"
#include "InverseWishartDistribution.h"

using namespace RevLanguage;

Dist_inverseWishart::Dist_inverseWishart() : TypedDistribution<MatrixRealSymmetric>()
{
    
}


Dist_inverseWishart::~Dist_inverseWishart()
{
    
}



Dist_inverseWishart* Dist_inverseWishart::clone( void ) const
{
    return new Dist_inverseWishart(*this);
}


RevBayesCore::InverseWishartDistribution* Dist_inverseWishart::createDistribution( void ) const
{
    
    // get the parameters
    RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal>* sg = NULL;
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* dv = NULL;
    RevBayesCore::TypedDagNode<double>* ka = NULL;
    RevBayesCore::TypedDagNode<int>* deg = NULL;
    RevBayesCore::TypedDagNode<int>* dm = NULL;
    
    if ( sigma->getRevObject() != RevNullObject::getInstance() )
    {
        sg = static_cast<const MatrixRealSymmetric &>( sigma->getRevObject() ).getDagNode();
    }
    
    if ( diagonal->getRevObject() != RevNullObject::getInstance() )
    {
        dv = static_cast<const ModelVector<RealPos> &>( diagonal->getRevObject() ).getDagNode();
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
    
    RevBayesCore::InverseWishartDistribution* w =  NULL;

    if ( sg != NULL && sg->getValue().getDim() != 0 )
    {
        // parameter is sigma
        w = new RevBayesCore::InverseWishartDistribution( sg, deg );
    }
    else if (dm == NULL || dm->getValue() == 0)
    {
        // parameter is Diagonal(kappaVector))
        w = new RevBayesCore::InverseWishartDistribution( dv, deg );
    }
    else
    {
        // parameter is kappa * Id
        w = new RevBayesCore::InverseWishartDistribution( dm, ka, deg );
    }
    
    return w;
}



/* Get class name of object */
const std::string& Dist_inverseWishart::getClassType(void) {
    
    static std::string revClassType = "Dist_inverseWishart";
    
	return revClassType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_inverseWishart::getClassTypeSpec(void) {
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return revClassTypeSpec;
}




/** Return member rules (no members) */
const MemberRules& Dist_inverseWishart::getParameterRules(void) const {
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        distMemberRules.push_back( new ArgumentRule( "sigma"   , MatrixRealSymmetric::getClassTypeSpec() , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL  ) );
        distMemberRules.push_back( new ArgumentRule( "diagonal", ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL  ) );
        distMemberRules.push_back( new ArgumentRule( "df"      , Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        distMemberRules.push_back( new ArgumentRule( "kappa"   , RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        distMemberRules.push_back( new ArgumentRule( "dim"     , Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        
        rulesSet = true;
    }
    
    return distMemberRules;
}


const TypeSpec& Dist_inverseWishart::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_inverseWishart::printValue(std::ostream& o) const {
    
    o << " InverseWishart(sigma=";
/*
    if ( sigma != NULL ) {
        o << sigma->getName();
    } else {
*/
    if (kappa != NULL)  {
        if (dim == NULL) {
            throw RbException("error in Wishart distribution: kappa and dim should both be non null");
        }
        o << kappa->getName() << ".I_" << dim->getName();
    } else {
        o << "?";
    }

    o << ")";
}


/** Set a member variable */
void Dist_inverseWishart::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "sigma" ) {
        sigma = var;
    }
    else if ( name == "diagonal" ) {
        diagonal = var;
    }
    else if ( name == "kappa" ) {
        kappa = var;
    }
    else if ( name == "df" ) {
        df = var;
    }
    else if ( name == "dim" ) {
        dim = var;
    }
    else {
        Distribution::setConstParameter(name, var);
    }
}
