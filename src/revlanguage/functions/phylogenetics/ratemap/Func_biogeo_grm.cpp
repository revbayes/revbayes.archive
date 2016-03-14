#include "DistanceDependentDispersalFunction.h"
#include "Func_biogeo_grm.h"
#include "GeographyRateModifier.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlAtlas.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "RlGeographyRateModifier.h"
#include "RlRateGeneratorSequence.h"
#include "RlSimplex.h"
#include "TimeAtlas.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_biogeo_grm::Func_biogeo_grm( void ) : TypedFunction< GeographyRateModifier >( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_biogeo_grm* Func_biogeo_grm::clone( void ) const {
    
    return new Func_biogeo_grm( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::GeographyRateModifier >* Func_biogeo_grm::createFunction( void ) const
{
    
    const RevBayesCore::TimeAtlas* atlas = &( static_cast<const RlAtlas&>( this->args[0].getVariable()->getRevObject() ).getValue() );
    
    RevBayesCore::TypedDagNode<double>* dp = static_cast<const Real&>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    bool udd    = static_cast<const RlBoolean &>( this->args[2].getVariable()->getRevObject() ).getValue();
    bool uav    = static_cast<const RlBoolean &>( this->args[3].getVariable()->getRevObject() ).getValue();
    bool uadj   = false; // static_cast<const RlBoolean &>( this->args[4].getVariable()->getRevObject() ).getValue();


    RevBayesCore::DistanceDependentDispersalFunction* f = new RevBayesCore::DistanceDependentDispersalFunction(dp, atlas, uadj, uav, udd);
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_biogeo_grm::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "atlas"        , RlAtlas::getClassTypeSpec()   , "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "distancePower", Real::getClassTypeSpec()      , "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Real(1e-5) ) );
        argumentRules.push_back( new ArgumentRule( "useDistances" , RlBoolean::getClassTypeSpec() , "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RlBoolean(true) ) );
        argumentRules.push_back( new ArgumentRule( "useAvailable" , RlBoolean::getClassTypeSpec() , "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RlBoolean(false) ) );
//        argumentRules.push_back( new ArgumentRule( "useAdjacency" , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RlBoolean(true) ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_biogeo_grm::getClassType(void)
{
    
    static std::string revType = "Func_biogeo_grm";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_biogeo_grm::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_biogeo_grm::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnBiogeoGRM";
    
    return f_name;
}


const TypeSpec& Func_biogeo_grm::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
