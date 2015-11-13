#include "DeterministicNode.h"
#include "Func_cpRev.h"
#include "Natural.h"
#include "RateMatrix_CpRev.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_cpRev::Func_cpRev( void ) : Procedure( )
{
    
}


/** Clone object */
Func_cpRev* Func_cpRev::clone( void ) const
{
    
    return new Func_cpRev( *this );
}


RevPtr<RevVariable> Func_cpRev::execute()
{
    
    
    RevBayesCore::RateMatrix_CpRev *rmj = new RevBayesCore::RateMatrix_CpRev();
    RateMatrix* value = new RateMatrix( rmj );
    
    return new RevVariable( value );
}


/* Get argument rules */
const ArgumentRules& Func_cpRev::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& Func_cpRev::getClassType(void)
{
    
    static std::string revType = "Func_cpRev";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_cpRev::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/* Get return type */
const TypeSpec& Func_cpRev::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = RateGenerator::getClassTypeSpec();
    
    return returnTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_cpRev::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnCpRev";
    
    return f_name;
}


const TypeSpec& Func_cpRev::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
