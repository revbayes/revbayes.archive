#include "Func_doubletEpistaticGTRRateMatrix.h"
#include "DoubletEpistaticGTRRateMatrixFunction.h"
#include "RateMatrix_DoubletEpistaticGTR.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_doubletEpistaticGTRRateMatrix::Func_doubletEpistaticGTRRateMatrix( void ) : TypedFunction<RateMatrix>( )
{

}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_doubletEpistaticGTRRateMatrix* Func_doubletEpistaticGTRRateMatrix::clone( void ) const
{

    return new Func_doubletEpistaticGTRRateMatrix( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_doubletEpistaticGTRRateMatrix::createFunction( void ) const
{

    RevBayesCore::TypedDagNode< double >* d = static_cast<const RealPos &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::Simplex >* er = static_cast<const Simplex &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::Simplex >* df = static_cast<const Simplex &>( this->args[2].getVariable()->getRevObject() ).getDagNode();

    if ( df->getValue().size() != 16 )
    {
        throw RbException("The doublet epistatic GTR rate matrix function requires 16 base (doublet) frequencies.");
    }

    if ( er->getValue().size() != 6 )
    {
        throw RbException("The doublet epistatic GTR rate matrix function requires 6 (nucleotide) exchange rates.");
    }

    RevBayesCore::DoubletEpistaticGTRRateMatrixFunction* f = new RevBayesCore::DoubletEpistaticGTRRateMatrixFunction( d, er, df );

    return f;
}


/* Get argument rules */
const ArgumentRules& Func_doubletEpistaticGTRRateMatrix::getArgumentRules( void ) const
{

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;

    if ( !rules_set )
    {

        argumentRules.push_back( new ArgumentRule( "d"                 , RealPos::getClassTypeSpec(), "The relative rate parameter controling doublet substutitions.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "exchangeRates"     , Simplex::getClassTypeSpec(), "The GTR single-nucleotide exchange rates.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "doubletFrequencies", Simplex::getClassTypeSpec(), "The doublet stationary frequencies.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );

        rules_set = true;
    }

    return argumentRules;
}


const std::string& Func_doubletEpistaticGTRRateMatrix::getClassType(void)
{

    static std::string rev_type = "Func_doubletEpistaticGTRRateMatrix";

    return rev_type;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_doubletEpistaticGTRRateMatrix::getClassTypeSpec(void)
{

    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );

    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_doubletEpistaticGTRRateMatrix::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnDoubletEpistaticGTR";

    return f_name;
}


const TypeSpec& Func_doubletEpistaticGTRRateMatrix::getTypeSpec( void ) const
{

    static TypeSpec type_spec = getClassTypeSpec();

    return type_spec;
}
