#include "DeterministicNode.h"
#include "Func_mtMam.h"
#include "Natural.h"
#include "RateMatrix_MtMam.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_mtMam::Func_mtMam( void ) : Procedure( )
{
    
}


/** Clone object */
Func_mtMam* Func_mtMam::clone( void ) const
{
    
    return new Func_mtMam( *this );
}


RevPtr<RevVariable> Func_mtMam::execute()
{
    
    
    RevBayesCore::RateMatrix_MtMam *rmj = new RevBayesCore::RateMatrix_MtMam();
    RateMatrix* value = new RateMatrix( rmj );
    
    return new RevVariable( value );
}


/* Get argument rules */
const ArgumentRules& Func_mtMam::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& Func_mtMam::getClassType(void)
{
    
    static std::string revType = "Func_mtMam";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_mtMam::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get the primary Rev name for this function.
 */
const std::string& Func_mtMam::getFunctionName( void ) const
{
    // create a static name variable that is the same for all instance of this class
    static std::string f_name = "fnMtMam";
    
    return f_name;
}


/* Get return type */
const TypeSpec& Func_mtMam::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = RateGenerator::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_mtMam::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
