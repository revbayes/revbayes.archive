#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_contacts.h"
#include "RbException.h"
#include "RlBoolean.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "RlUserInterface.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_contacts::Func_contacts( void ) : Function() {
    
}


/** Clone object */
Func_contacts* Func_contacts::clone( void ) const {
    
    return new Func_contacts( *this );
}


/** Execute function */
RevPtr<Variable> Func_contacts::execute( void )
{
#if 0
    RBOUT( "00000011111111112222222222333333333344444444445555555555666666666677777777778" );
    RBOUT( "45678901234567890123456789012345678901234567890123456789012345678901234567890" );
#endif
    
    RBOUT( "\n" );
    RBOUT( "Contact details for core contributors\n" );
    RBOUT( "=====================================\n" );
    RBOUT( "\n" );
    RBOUT( "Sebastian Höhna\n" );
    RBOUT( "Department of Statistic,\n" );
    RBOUT( "Department of Integrative Biology,\n" );
    RBOUT( "Miller Institute for Basic Research in Science,\n" );
    RBOUT( "University of California, Berkeley, USA" );
    RBOUT( "Sebastian.Hoehna@gmail.com\n" );
    RBOUT( "\n" );
    
    RBOUT( "Fredrik Ronquist\n" );
    RBOUT( "Department of Bioinformatics and Genetics,\n" );
    RBOUT( "Swedish Museum of Natural History, Stockholm, Sweden" );
    RBOUT( "fredrik.ronquist@nrm.se\n" );
    RBOUT( "\n" );
    
    RBOUT( "Bastien Boussau\n" );
    RBOUT( "Bioinformatics and Evolutionary Genomics,\n" );
    RBOUT( "Université de Lyon, Villeurbanne, France" );
    RBOUT( "boussau@gmail.com\n" );
    RBOUT( "\n" );

    RBOUT( "Tracy A. Heath\n" );
    RBOUT( "Department of Integrative Biology,\n" );
    RBOUT( "University of California, Berkeley, USA" );
    RBOUT( "tracyh@berkeley.edu\n" );
    RBOUT( "\n" );

    RBOUT( "John P. Huelsenbeck\n" );
    RBOUT( "Department of Integrative Biology,\n" );
    RBOUT( "University of California, Berkeley, USA" );
    RBOUT( "and" );
    RBOUT( "Department of Biology,\n" );
    RBOUT( "King Abdulaziz University, Jeddah, Saudi Arabia" );
    RBOUT( "johnh@berkeley.edu\n" );
    RBOUT( "\n" );
    
    RBOUT( "Michael J. Landis\n" );
    RBOUT( "Department of Integrative Biology,\n" );
    RBOUT( "University of California, Berkeley, USA" );
    RBOUT( "mlandis@berkeley.edu\n" );
    RBOUT( "\n" );
    
    RBOUT( "Nicolas Lartillot\n" );
    RBOUT( "Bioinformatics and Evolutionary Genomics,\n" );
    RBOUT( "Université de Lyon, Villeurbanne, France" );
    RBOUT( "nicolas.lartillot@univ-lyon1.fr\n" );
    RBOUT( "\n" );
    RBOUT( "\n" );
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_contacts::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "all", RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_contacts::getClassType(void) {
    
    static std::string revType = "Func_contacts";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_contacts::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}

/** Get type spec */
const TypeSpec& Func_contacts::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_contacts::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

