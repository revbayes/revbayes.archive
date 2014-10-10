#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "Func_simplex.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "RlSimplex.h"
#include "SimplexFunction.h"


using namespace RevLanguage;


/** Default constructor */
Func_simplex::Func_simplex( void ) :
    Function()
{
}

/** Clone object */
Func_simplex* Func_simplex::clone( void ) const
{
    return new Func_simplex( *this );
}


/** Execute function: Construct simplex from RealPos values. */
RevPtr<Variable> Func_simplex::execute( void )
{
    std::vector<const RevBayesCore::TypedDagNode<double>* > params;
    for ( size_t i = 0; i < args.size(); i++ )
    {
        const RealPos &val = static_cast<const RealPos &>( args[i].getVariable()->getRevObject() );
        params.push_back( val.getDagNode() );
    }
    
    RevBayesCore::SimplexFunction* func = new RevBayesCore::SimplexFunction( params );
    
    DeterministicNode< RevBayesCore::RbVector<double> >* detNode = new DeterministicNode< RevBayesCore::RbVector<double> >( "", func, this->clone() );
    
    Simplex* theSimplex = new Simplex( detNode );
        
    return new Variable( theSimplex );
}


/** Get argument rules */
const ArgumentRules& Func_simplex::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "x1", RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "x2", RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new Ellipsis ( RealPos::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object (static) */
const std::string& Func_simplex::getClassType( void )
{
    static std::string revType = "Func_simplex";
    
	return revType; 
}


/** Get Rev type spec of object (static) */
const TypeSpec& Func_simplex::getClassTypeSpec( void )
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &Function::getClassTypeSpec() );
    
	return revTypeSpec; 
}


/** Get Rev type spec of object from an instance */
const TypeSpec& Func_simplex::getTypeSpec( void ) const
{
    return getClassTypeSpec();
}


/** Get return type of function */
const TypeSpec& Func_simplex::getReturnType( void ) const
{
    return Simplex::getClassTypeSpec();
}

