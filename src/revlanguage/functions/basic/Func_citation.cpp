#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_citation.h"
#include "RbException.h"
#include "RlBoolean.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "RlUserInterface.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_citation::Func_citation( void ) : Procedure()
{
}


/** Clone object */
Func_citation* Func_citation::clone( void ) const
{
    return new Func_citation( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_citation::execute( void )
{
    // Use the lines below to count columns, if you wish
#if 0
    RBOUT( "00000011111111112222222222333333333344444444445555555555666666666677777777778" );
    RBOUT( "45678901234567890123456789012345678901234567890123456789012345678901234567890" );
#endif
    
    RBOUT( "                                                                            \n" );
    RBOUT( "RevBayes has not been officially released yet, so there is no release note  \n" );
    RBOUT( "to cite. However, the basic graphical model concepts underlying the program \n" );
    RBOUT( "have been described in the following paper:                                 \n");
    RBOUT( "                                                                            \n" );
    RBOUT( "Hohna S, Heath TA, Boussau B, Landis M, Ronquist F, Huelsenbeck JP. 2014.   \n");
    RBOUT( "   Probabilistic graphical model representation in phylogenetics. Systematic\n");
    RBOUT( "   Biology 63: 753-771.                                                     \n");
    RBOUT( "                                                                            \n" );
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_citation::getArgumentRules( void ) const
{
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_citation::getClassType(void)
{
    static std::string revType = "Func_citation";
    
    return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_citation::getClassTypeSpec(void)
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/** Get type spec */
const TypeSpec& Func_citation::getTypeSpec( void ) const
{
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_citation::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

