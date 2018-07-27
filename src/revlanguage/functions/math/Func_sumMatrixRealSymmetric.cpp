#include "SumMatrixFunction.h"
#include "Func_sumMatrixRealSymmetric.h"
#include "ModelVector.h"
#include "RlMatrixRealSymmetric.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_sumMatrixRealSymmetric::Func_sumMatrixRealSymmetric( void ) : TypedFunction<MatrixRealSymmetric>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_sumMatrixRealSymmetric* Func_sumMatrixRealSymmetric::clone( void ) const
{
    
    return new Func_sumMatrixRealSymmetric( *this );
}


RevBayesCore::TypedFunction<RevBayesCore::MatrixReal>* Func_sumMatrixRealSymmetric::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<RevBayesCore::MatrixReal> >* arg = static_cast<const ModelVector<MatrixReal> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::SumMatrixFunction* f = new RevBayesCore::SumMatrixFunction( arg );
    
    return f;
    
}


/* Get argument rules */
const ArgumentRules& Func_sumMatrixRealSymmetric::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", ModelVector<MatrixRealSymmetric>::getClassTypeSpec(), "A vector of symmetric matrices.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_sumMatrixRealSymmetric::getClassType(void)
{
    
    static std::string rev_type = "Func_sumMatrixRealSymmetric";
    
    return rev_type;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_sumMatrixRealSymmetric::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( TypedFunction<Natural>::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_sumMatrixRealSymmetric::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "sum";
    
    return f_name;
}


const TypeSpec& Func_sumMatrixRealSymmetric::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
