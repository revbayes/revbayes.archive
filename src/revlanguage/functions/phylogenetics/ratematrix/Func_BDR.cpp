#include "BDRRateMatrixFunction.h"
#include "Func_BDR.h"
#include "RateMatrix_Chromosomes.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_BDR::Func_BDR( void ) : TypedFunction<RateMatrix>( ) {
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_BDR* Func_BDR::clone( void ) const
{
    
    return new Func_BDR( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_BDR::createFunction( void ) const {
    
    
    RevBayesCore::TypedDagNode< int    >* n         = static_cast<const Natural &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* alpha     = static_cast<const RealPos &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* beta      = static_cast<const RealPos &>( this->args[2].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::TypedDagNode< double >* lambda_i  = static_cast<const RealPos &>( this->args[3].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* mu_i      = static_cast<const RealPos &>( this->args[4].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::TypedDagNode< double >* lambda_a  = static_cast<const RealPos &>( this->args[5].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* mu_a      = static_cast<const RealPos &>( this->args[6].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::TypedDagNode< double >* lambda_ai = static_cast<const RealPos &>( this->args[7].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* lambda_ia = static_cast<const RealPos &>( this->args[8].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::TypedDagNode< double >* denovo_i = static_cast<const RealPos &>( this->args[8].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* denovo_a = static_cast<const RealPos &>( this->args[9].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::BDRRateMatrixFunction* f = new RevBayesCore::BDRRateMatrixFunction( n, alpha, beta, lambda_a, mu_a, lambda_i, mu_i, lambda_ai, lambda_ia, denovo_i, denovo_a );
    
	return f;
}


/* Get argument rules */
const ArgumentRules& Func_BDR::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
      
        argumentRules.push_back( new ArgumentRule( "maxGenes"      , Natural::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        argumentRules.push_back( new ArgumentRule( "alpha"         , RealPos::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "beta"          , RealPos::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        argumentRules.push_back( new ArgumentRule( "lambda_i"      , RealPos::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "mu_i"          , RealPos::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        argumentRules.push_back( new ArgumentRule( "lambda_a"      , RealPos::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "mu_a"          , RealPos::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        argumentRules.push_back( new ArgumentRule( "lambda_ai"     , RealPos::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );
        argumentRules.push_back( new ArgumentRule( "lambda_ia"     , RealPos::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );

        argumentRules.push_back( new ArgumentRule( "denovo_i"     , RealPos::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );
        argumentRules.push_back( new ArgumentRule( "denovo_a"     , RealPos::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );

        rules_set = true;
        
    }
    
    return argumentRules;
}


const std::string& Func_BDR::getClassType(void)
{
    
    static std::string rev_type = "Func_BDR";
    
	return rev_type;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_BDR::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_BDR::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnBDR";
    
    return f_name;
}


const TypeSpec& Func_BDR::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
