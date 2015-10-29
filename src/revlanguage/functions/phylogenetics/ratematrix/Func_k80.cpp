#include "Func_k80.h"
#include "K80RateMatrixFunction.h"
#include "RateMatrix_Kimura80.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_k80::Func_k80( void ) : TypedFunction<RateGenerator>( )
{
    
}


/** Clone object */
Func_k80* Func_k80::clone( void ) const
{
    
    return new Func_k80( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_k80::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode< double >* ka = static_cast<const RealPos &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::K80RateMatrixFunction* f = new RevBayesCore::K80RateMatrixFunction( ka );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_k80::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "kappa" , RealPos::getClassTypeSpec(), "The transition/transversion rate.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_k80::getClassType(void)
{
    
    static std::string revType = "Func_k80";
    
    return revType;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_k80::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_k80::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnK80";
    
    return f_name;
}


const TypeSpec& Func_k80::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
