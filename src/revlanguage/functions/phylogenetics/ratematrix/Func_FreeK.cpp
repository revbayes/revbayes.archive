#include "Func_FreeK.h"
#include "FreeKRateMatrixFunction.h"
#include "Natural.h"
#include "RateMatrix_JC.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TransitionProbabilityMatrix.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_FreeK::Func_FreeK( void ) : TypedFunction<RateMatrix>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_FreeK* Func_FreeK::clone( void ) const
{
    
    return new Func_FreeK( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_FreeK::createFunction( void ) const
{
    
//    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* tr = static_cast<const ModelVector<Real> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
//    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RbVector<double> > >* tr = static_cast<const ModelVector<ModelVector<Real> > &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
//    RevBayesCore::FreeKRateMatrixFunction* f = new RevBayesCore::FreeKRateMatrixFunction( tr );
    
    // flat transition rates
    if ( this->args[0].getVariable()->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* tr = static_cast<const ModelVector<Real> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
        RevBayesCore::FreeKRateMatrixFunction* f = new RevBayesCore::FreeKRateMatrixFunction( tr );
        return f;
    }
    else
    {
        RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RbVector<double> > >* tr = static_cast<const ModelVector<ModelVector<RealPos> > &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
        RevBayesCore::FreeKRateMatrixFunction* f = new RevBayesCore::FreeKRateMatrixFunction( tr );
        return f;
    }
}


/* Get argument rules */
const ArgumentRules& Func_FreeK::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        std::vector<TypeSpec> transitionRateTypes;
        transitionRateTypes.push_back( ModelVector<ModelVector<RealPos> >::getClassTypeSpec() );
        transitionRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        argumentRules.push_back( new ArgumentRule( "transition_rates", transitionRateTypes, "Transition rates between states.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_FreeK::getClassType(void)
{
    
    static std::string revType = "Func_FreeK";
    
	return revType;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_FreeK::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_FreeK::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnFreeK";
    
    return f_name;
}


const TypeSpec& Func_FreeK::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
