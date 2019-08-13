#include "Func_experimentalCodonModelRateMatrix.h"
#include "ExperimentalCodonModelRateMatrixFunction.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_experimentalCodonModelRateMatrix::Func_experimentalCodonModelRateMatrix( void ) : TypedFunction<RateMatrix>( )
{

}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_experimentalCodonModelRateMatrix* Func_experimentalCodonModelRateMatrix::clone( void ) const
{

    return new Func_experimentalCodonModelRateMatrix( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_experimentalCodonModelRateMatrix::createFunction( void ) const
{

    RevBayesCore::TypedDagNode< double >* be = static_cast<const RealPos &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* om = static_cast<const RealPos &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* ka = static_cast<const RealPos &>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::Simplex >* aap = static_cast<const Simplex &>( this->args[3].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::Simplex >* bf = static_cast<const Simplex &>( this->args[4].getVariable()->getRevObject() ).getDagNode();

    if ( bf->getValue().size() != 4 )
    {
        throw RbException("The expCM rate matrix can only use 4 (nucleotide frequencies) base fequencies.");
    }

    if ( aap->getValue().size() != 20 )
    {
        throw RbException("The expCM rate matrix requires 20 preferences.");
    }

    RevBayesCore::ExperimentalCodonModelRateMatrixFunction* f = new RevBayesCore::ExperimentalCodonModelRateMatrixFunction( be, om, ka, aap, bf );

    return f;
}


/* Get argument rules */
const ArgumentRules& Func_experimentalCodonModelRateMatrix::getArgumentRules( void ) const
{

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;

    if ( !rules_set )
    {

        argumentRules.push_back( new ArgumentRule( "beta"                , RealPos::getClassTypeSpec(), "The stringency parameter.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "omega"               , RealPos::getClassTypeSpec(), "The dN / dS rate ratio.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "kappa"               , RealPos::getClassTypeSpec(), "The transition-transversion rate ratio.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "aminoAcidPreferences", Simplex::getClassTypeSpec(), "The (experimentally measured) amino acid preferences.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "baseFrequencies"     , Simplex::getClassTypeSpec(), "The stationary frequencies.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );

        rules_set = true;
    }

    return argumentRules;
}


const std::string& Func_experimentalCodonModelRateMatrix::getClassType(void)
{

    static std::string rev_type = "Func_experimentalCodonModelRateMatrix";

    return rev_type;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_experimentalCodonModelRateMatrix::getClassTypeSpec(void)
{

    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );

    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_experimentalCodonModelRateMatrix::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnExpCM";

    return f_name;
}


const TypeSpec& Func_experimentalCodonModelRateMatrix::getTypeSpec( void ) const
{

    static TypeSpec type_spec = getClassTypeSpec();

    return type_spec;
}
