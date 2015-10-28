#include "Func_freeSymmetricRateMatrix.h"
#include "FreeSymmetricRateMatrixFunction.h"
#include "Natural.h"
#include "RateMatrix_JC.h"
#include "Real.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TransitionProbabilityMatrix.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_freeSymmetricRateMatrix::Func_freeSymmetricRateMatrix( void ) : TypedFunction<RateGenerator>( )
{
    
}


/** Clone object */
Func_freeSymmetricRateMatrix* Func_freeSymmetricRateMatrix::clone( void ) const
{
    
    return new Func_freeSymmetricRateMatrix( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_freeSymmetricRateMatrix::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* tr = static_cast<const ModelVector<RealPos> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    bool r = static_cast<const RlBoolean &>( this->args[1].getVariable()->getRevObject() ).getDagNode()->getValue();
    
    RevBayesCore::FreeSymmetricRateMatrixFunction* f = new RevBayesCore::FreeSymmetricRateMatrixFunction( tr, r );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_freeSymmetricRateMatrix::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "transitionRates",   ModelVector<RealPos>::getClassTypeSpec(),   "The transition rates between states.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "rescaled",          RlBoolean::getClassTypeSpec(),              "Should the matrix be normalized?", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_freeSymmetricRateMatrix::getClassType(void)
{
    
    static std::string revType = "Func_freeSymmetricRateMatrix";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_freeSymmetricRateMatrix::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
const std::string& Func_freeSymmetricRateMatrix::getFunctionName( void ) const
{
    // create a static name variable that is the same for all instance of this class
    static std::string f_name = "fnFreeSymmetricRateMatrix";
    
    return f_name;
}


const TypeSpec& Func_freeSymmetricRateMatrix::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
