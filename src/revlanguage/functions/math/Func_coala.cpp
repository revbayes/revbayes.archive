#include "ArgumentRule.h"
#include "CoalaFunction.h"
#include "Func_coala.h"
#include "MatrixReal.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlMatrixReal.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;


/** Default constructor */
Func_coala::Func_coala( void ) : TypedFunction< ModelVector<Real> >()
{
    
}

/** Clone object */
Func_coala* Func_coala::clone( void ) const
{
    return new Func_coala( *this );
}


/** Execute function: Construct coala from RealPos values. */
RevBayesCore::TypedFunction< RevBayesCore::RbVector<double> >* Func_coala::createFunction( void ) const
{
    const RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> > *coordinates = static_cast< ModelVector<Real> & >( args[0].getVariable()->getRevObject() ).getDagNode();
    const RevBayesCore::MatrixReal &coa = static_cast< MatrixReal& >( args[1].getVariable()->getRevObject() ).getValue();
    const std::vector<double> &w = static_cast< ModelVector<RealPos>& >( args[2].getVariable()->getRevObject() ).getValue();
    
    
    RevBayesCore::CoalaFunction* func = new RevBayesCore::CoalaFunction( coordinates, coa, w );
    
    return func;
}


/** Get argument rules */
const ArgumentRules& Func_coala::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "coordinates",   ModelVector<Real>::getClassTypeSpec(),    "A vector of coordinates.",  ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "corAnalysis",   MatrixReal::getClassTypeSpec(),           "A correspondence analysis object.",  ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "weights",       ModelVector<RealPos>::getClassTypeSpec(), "A vector of weight for the coordinates.",  ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );

        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object (static) */
const std::string& Func_coala::getClassType( void )
{
    static std::string revType = "Func_coala";
    
    return revType;
}


/** Get Rev type spec of object (static) */
const TypeSpec& Func_coala::getClassTypeSpec( void )
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &Function::getClassTypeSpec() );
    
    return revTypeSpec;
}


/** Get Rev type spec of object from an instance */
const TypeSpec& Func_coala::getTypeSpec( void ) const
{
    return getClassTypeSpec();
}

