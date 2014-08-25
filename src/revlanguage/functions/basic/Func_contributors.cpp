/**
 * @file
 * This file contains the implementation of Func_contributors, which is
 * the function used to provide information on contributors.
 *
 * @brief Implementation of Func_contributors
 *
 * @author Fredrik Ronquist
 * @license GPL version 3
 */

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
    // Hard-wrapped to 80 columns wide, assuming three-character indent in RBOUT
#if 0
    RBOUT( "00000011111111112222222222333333333344444444445555555555666666666677777777778" );
    RBOUT( "45678901234567890123456789012345678901234567890123456789012345678901234567890" );
#endif
    
    RBOUT( "\n" );
    RBOUT( "Contributors\n" );
    RBOUT( "============\n" );
    RBOUT( "\n" );
    RBOUT( "Sebastian Höhna\n" );
    RBOUT( "Department of Evolution and Ecology,\n" );
    RBOUT( "University of California, Davis, USA" );
    RBOUT( "Sebastian.Hoehna@gmail.com\n" );
    RBOUT( "\n" );
    RBOUT( "Fredrik Ronquist\n" );
    RBOUT( "Department of Bioinformatics and Genetics,\n" );
    RBOUT( "Swedish Museum of Natural History, Stockholm, Sweden" );
    RBOUT( "fredrik.ronquist@nrm.se\n" );
    RBOUT( "\n" );
    RBOUT( "John P. Huelsenbeck\n" );
    RBOUT( "Department of Integrative Biology,\n" );
    RBOUT( "University of California, Berkeley, USA" );
    RBOUT( "johnh@berkeley.edu\n" );
    RBOUT( "\n" );
    RBOUT( "Michael J. Landis\n" );
    RBOUT( "Department of Integrative Biology,\n" );
    RBOUT( "University of California, Berkeley, USA" );
    RBOUT( "mlandis@berkeley.edu\n" );
    RBOUT( "\n" );
    RBOUT( "Tracy A. Heath\n" );
    RBOUT( "Department of Integrative Biology,\n" );
    RBOUT( "University of California, Berkeley, USA" );
    RBOUT( "tracyh@berkeley.edu\n" );
    RBOUT( "\n" );
    RBOUT( "Bastien Boussau\n" );
    RBOUT( "Bioinformatics and Evolutionary Genomics,\n" );
    RBOUT( "Université de Lyon, Villeurbanne, France" );
    RBOUT( "boussau@gmail.com\n" );
    RBOUT( "\n" );
    RBOUT( "Nicolas Lartillot\n" );
    RBOUT( "Bioinformatics and Evolutionary Genomics,\n" );
    RBOUT( "Université de Lyon, Villeurbanne, France" );
    RBOUT( "nicolas.lartillot@univ-lyon1.fr\n" );
    RBOUT( "\n" );
    RBOUT( "\n" );
    RBOUT( "\n" );
    RBOUT( "\n" );
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_contributors::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "all", true, RlBoolean::getClassTypeSpec(), new RlBoolean(false) ) );
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

