#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_contributors.h"
#include "RbException.h"
#include "RlBoolean.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "RlUserInterface.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_contributors::Func_contributors( void ) : Function() {
    
}


/** Clone object */
Func_contributors* Func_contributors::clone( void ) const {
    
    return new Func_contributors( *this );
}


/** Execute function */
RevPtr<Variable> Func_contributors::execute( void )
{
#if 0
    RBOUT( "00000011111111112222222222333333333344444444445555555555666666666677777777778" );
    RBOUT( "45678901234567890123456789012345678901234567890123456789012345678901234567890" );
#endif
    
    RBOUT( "\n" );
    RBOUT( "RevBayes 1.0 was designed & developed by:\n" );
    RBOUT( "=========================================\n" );
    RBOUT( "\n" );
    RBOUT( "Sebastian Höhna\n" );
    RBOUT( "Sebastian.Hoehna@gmail.com\n" );
    RBOUT( "\n" );
    
    RBOUT( "Fredrik Ronquist\n" );
    RBOUT( "fredrik.ronquist@nrm.se\n" );
    RBOUT( "\n" );
    
    RBOUT( "John P. Huelsenbeck\n" );
    RBOUT( "johnh@berkeley.edu\n" );
    RBOUT( "\n" );
    RBOUT( "\n" );
    
    RBOUT( "RevBayes core developers:\n" );
    RBOUT( "=========================\n" );
    
    RBOUT( "Bastien Boussau, Tracy A. Heath, Michael J. Landis and Nicolas Lartillot.\n" );
    RBOUT( "\n" );
    RBOUT( "\n" );
    
    RBOUT( "Thanks to:\n" );
    RBOUT( "==========\n" );
    
    RBOUT( "Lars Arvestad, Paul van der Mark, Brian R. Moore, Tanja Stadler and Chi Zhang.\n" );
    RBOUT( "\n" );
    RBOUT( "\n" );
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_contributors::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "all", RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_contributors::getClassType(void) {
    
    static std::string revType = "Func_contributors";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_contributors::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}

/** Get type spec */
const TypeSpec& Func_contributors::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_contributors::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

