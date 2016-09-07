#include "BiogeographyRateMapFunction.h"
#include "Func_FreeBinary.h"
#include "Func_biogeo_de.h"
#include "GeographyRateModifier.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RlRateMatrix.h"
#include "RateMatrix_FreeBinary.h"
#include "RateMap_Biogeography.h"
#include "Real.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "RlGeographyRateModifier.h"
#include "RlRateMap.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_biogeo_de::Func_biogeo_de( void ) : TypedFunction<RateMap>( ) {
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_biogeo_de* Func_biogeo_de::clone( void ) const {
    
    return new Func_biogeo_de( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateMap >* Func_biogeo_de::createFunction( void ) const
{
    
//    RevBayesCore::TypedDagNode<std::vector<double> >* glr = static_cast<const ModelVector<RealPos> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    //    RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix&>( gainLossRates->getRevObject() ).getDagNode();
    //    RevBayesCore::TypedDagNode<std::vector<double> >* rf = static_cast<const Simplex &>( root_frequencies->getRevObject() ).getDagNode();
    
    //    if ( this->args[2] != NULL && origin->getRevObject() != RevNullObject::getInstance() )
    //    RevObject* tmp = &this->args[2].getVariable()->getRevObject();
    //    if ( tmp == NULL )
    //        std::cout << "what!\n";
    
    //    RevBayesCore::TypedDagNode<RevBayesCore::GeographyRateModifier>* grm = NULL;
    //    if ( geoRateMod != NULL && geoRateMod->getRevObject() != RevNullObject::getInstance() )
    //    {
    //        grm = static_cast<const RlGeographyRateModifier &>( geoRateMod->getRevObject() ).getDagNode();
    //    }

    
    
    RevBayesCore::TypedDagNode< RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* rf = static_cast<const Simplex &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode<RevBayesCore::GeographyRateModifier>* grm = NULL;

    if (this->args[2].getVariable()->getRevObject() != RevNullObject::getInstance())
    {
        grm = static_cast<const GeographyRateModifier&>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    }

    unsigned nc  = static_cast<const Natural&>( this->args[3].getVariable()->getRevObject() ).getValue();
    bool fe      = static_cast<const RlBoolean &>( this->args[4].getVariable()->getRevObject() ).getValue();
//    unsigned mrs = static_cast<const Natural&>( this->args[6].getVariable()->getRevObject() ).getValue();
//    if (mrs == 0)
//        mrs = nc;
    
    RevBayesCore::BiogeographyRateMapFunction* f = new RevBayesCore::BiogeographyRateMapFunction(nc,fe,nc); //(nc, true);
    f->setRateMatrix(rm);
    f->setRootFrequencies(rf);
    if (grm != NULL)
        f->setGeographyRateModifier(grm);
    
    if ( this->args[5].getVariable()->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* clockRates = static_cast<const ModelVector<RealPos> &>( this->args[5].getVariable()->getRevObject() ).getDagNode();
        //
        //        // sanity check
        //        size_t nRates = clockRates->getValue().size();
        //        if ( (nNodes-1) != nRates )
        //        {
        //            throw RbException( "The number of clock rates does not match the number of branches" );
        //        }
        
        f->setClockRate( clockRates );
    }
    else
    {
        RevBayesCore::TypedDagNode<double>* clockRate = static_cast<const RealPos &>( this->args[5].getVariable()->getRevObject() ).getDagNode();
        f->setClockRate( clockRate );
    }
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_biogeo_de::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        
//        argumentRules.push_back( new ArgumentRule( "gainLossRates"   , ModelVector<RealPos>::getClassTypeSpec()   , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "gainLossRates"   , RateGenerator::getClassTypeSpec()          , "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "root_frequencies" , Simplex::getClassTypeSpec()                , "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Simplex( std::vector<double>(2,0.5)) ) );
        argumentRules.push_back( new ArgumentRule( "geoRateMod"      , GeographyRateModifier::getClassTypeSpec()  , "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ));
        argumentRules.push_back( new ArgumentRule( "numAreas"        , Natural::getClassTypeSpec()                , "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "forbidExtinction", RlBoolean::getClassTypeSpec()              , "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RlBoolean(true) ) );
        
        std::vector<TypeSpec> branchRateTypes;
        branchRateTypes.push_back( RealPos::getClassTypeSpec() );
        branchRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        argumentRules.push_back( new ArgumentRule( "branchRates"     , branchRateTypes, "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
//        argumentRules.push_back( new ArgumentRule( "maxRangeSize"    , Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Natural(0)  ) );

        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_biogeo_de::getClassType(void)
{
    
    static std::string revType = "Func_biogeo_de";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_biogeo_de::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_biogeo_de::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnBiogeoDE";
    
    return f_name;
}


const TypeSpec& Func_biogeo_de::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}