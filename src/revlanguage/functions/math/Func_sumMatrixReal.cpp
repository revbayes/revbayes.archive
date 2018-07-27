#include "SumMatrixFunction.h"
#include "Func_sumMatrixReal.h"
#include "ModelVector.h"
#include "RlMatrixReal.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_sumMatrixReal::Func_sumMatrixReal( void ) : TypedFunction<MatrixReal>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_sumMatrixReal* Func_sumMatrixReal::clone( void ) const
{
    
    return new Func_sumMatrixReal( *this );
}


RevBayesCore::TypedFunction<RevBayesCore::MatrixReal>* Func_sumMatrixReal::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<RevBayesCore::MatrixReal> >* arg = static_cast<const ModelVector<MatrixReal> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::SumMatrixFunction* f = new RevBayesCore::SumMatrixFunction( arg );
    
    return f;
    
}


/* Get argument rules */
const ArgumentRules& Func_sumMatrixReal::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", ModelVector<MatrixReal>::getClassTypeSpec(), "A vector of matrices.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_sumMatrixReal::getClassType(void)
{
    
    static std::string rev_type = "Func_sumMatrixReal";
    
    return rev_type;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_sumMatrixReal::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( TypedFunction<Natural>::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_sumMatrixReal::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "sum";
    
    return f_name;
}


const TypeSpec& Func_sumMatrixReal::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
