#include "ArgumentRule.h"
#include "Func_simplexFromVector.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlSimplex.h"
#include "SimplexFromVectorFunction.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;


/** Default constructor */
Func_simplexFromVector::Func_simplexFromVector( void ) :
    TypedFunction<Simplex>()
{
}


/** Clone object */
Func_simplexFromVector* Func_simplexFromVector::clone( void ) const
{
    return new Func_simplexFromVector( *this );
}


/** Execute function: Compute simplex from vector of RealPos values */
RevBayesCore::TypedFunction< RevBayesCore::RbVector<double> >* Func_simplexFromVector::createFunction( void ) const
{
    
    const RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* vec;
    vec = static_cast< const ModelVector<RealPos>& >( args[0].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::SimplexFromVectorFunction*    func    = new RevBayesCore::SimplexFromVectorFunction( vec );
    
    return func;
}


/** Get argument rules */
const ArgumentRules& Func_simplexFromVector::getArgumentRules( void ) const
{
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "x", ModelVector<RealPos>::getClassTypeSpec(), "A vector of numbers.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object (static version) */
const std::string& Func_simplexFromVector::getClassType( void )
{
    static std::string revType = "Func_simplexFromVector";
    
	return revType;
}


/** Get Rev type spec of object (static version) */
const TypeSpec& Func_simplexFromVector::getClassTypeSpec( void )
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &Function::getClassTypeSpec() );
    
	return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
const std::string& Func_simplexFromVector::getFunctionName( void ) const
{
    // create a static name variable that is the same for all instance of this class
    static std::string f_name = "simplex";
    
    return f_name;
}


/** Get Rev type spec from an instance of the object */
const TypeSpec& Func_simplexFromVector::getTypeSpec( void ) const
{
    return getClassTypeSpec();
}

