#include "Func_diagonalMatrix.h"
#include "Natural.h"
#include "RlMatrixRealSymmetric.h"

using namespace RevLanguage;

/** default constructor */
Func_diagonalMatrix::Func_diagonalMatrix( void ) : Procedure()
{
    
}


/** Clone object */
Func_diagonalMatrix* Func_diagonalMatrix::clone( void ) const
{
    
    return new Func_diagonalMatrix( *this );
}


RevPtr<RevVariable> Func_diagonalMatrix::execute( void )
{
    
    int n = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
    RevBayesCore::MatrixReal *m = new RevBayesCore::MatrixReal(n, n, 0.0);
    for (size_t i = 0; i < n; ++i)
    {
        (*m)[i][i] = 1.0;
    }
    
    return new RevVariable( new MatrixRealSymmetric( m ) );
}



/* Get argument rules */
const ArgumentRules& Func_diagonalMatrix::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule("n", Natural::getClassTypeSpec(), "The number of rows/columns (dimension).", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_diagonalMatrix::getClassType(void)
{
    
    static std::string revType = "Func_diag";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_diagonalMatrix::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_diagonalMatrix::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "diagonalMatrix";
    
    return f_name;
}


const TypeSpec& Func_diagonalMatrix::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_diagonalMatrix::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = MatrixRealSymmetric::getClassTypeSpec();
    return returnTypeSpec;
}
