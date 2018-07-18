#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "Func_matrixInverse.h"
#include "Integer.h"
#include "NormalizeVectorFunction.h"
#include "MatrixReal.h"
#include "ModelVector.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "RlMatrixRealSymmetric.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "MatrixInverse.h"


using namespace RevLanguage;

Func_matrixInverse::Func_matrixInverse() : TypedFunction< MatrixRealSymmetric >() {
    
}

/* Clone object */
Func_matrixInverse* Func_matrixInverse::clone( void ) const
{
    return new Func_matrixInverse( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::MatrixReal >* Func_matrixInverse::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal >* m = static_cast<const MatrixReal& >( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::MatrixInverse *func = new RevBayesCore::MatrixInverse( m );
    
    return func;
    
}


/** Get argument rules */
const ArgumentRules& Func_matrixInverse::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        argumentRules.push_back( new ArgumentRule( "matrix", MatrixReal::getClassTypeSpec(), "A matrix.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        rules_set = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_matrixInverse::getClassName(void)
{
    
    static std::string rbClassName = "Func_matrixInverse";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
const RevLanguage::TypeSpec& Func_matrixInverse::getClassTypeSpec(void)
{
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_matrixInverse::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnMatrixInverse";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_matrixInverse::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
