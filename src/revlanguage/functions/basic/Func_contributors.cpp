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
    RBOUT( "The RevBayes project started as an attempt to provide a more general and\n" );
    RBOUT( "flexible version of the MrBayes software for Bayesian phylogenetic inference.\n" );
    RBOUT( "This future version of MrBayes was named MrBayes 4, as MrBayes was then in\n" );
    RBOUT( "version 3. The initial MrBayes 4 kick-off meeting was held in San Diego in\n" );
    RBOUT( "March of 2005 and involved John Huelsenbeck, Bret Larget, Fredrik Ronquist,\n" );
    RBOUT( "Donald Simon, and Paul van der Mark. Later, Marc Suchard would also contrib-\n" );
    RBOUT( "ute to the development of the project." );
    RBOUT( "\n");
    RBOUT( "A pilot version of MrBayes 4 was completed in 2007 by van der Mark, who was\n" );
    RBOUT( "then a postdoc in the lab of Ronquist. MrBayes 4 was first used in a Bayesian" );
    RBOUT( "island biogeography analysis published in 2008 in Journal of Biogeography by\n" );
    RBOUT( "Isabel Sanmartin, van der Mark and Ronquist. Several related papers using\n" );
    RBOUT( "MrBayes 4 followed.\n" );
    RBOUT( "\n" );
    RBOUT( "In August of 2007, Huelsenbeck, Ronquist, and Suchard reviewed the code of\n" );
    RBOUT( "MrBayes 4. The program then had a simple model-specification language but the\n" );
    RBOUT( "code was experimental and difficult to work with, and the language did not\n" );
    RBOUT( "yet support more generic modeling and computing.\n" );
    RBOUT( "\n" );
    RBOUT( "Ronquist took the lead in rewriting the code and generalizing the model\n" );
    RBOUT( "specification language. The work accelerated in 2009 when Sebastian Höhna\n" );
    RBOUT( "joined the project as a graduate student. In the summer of 2009, the name of\n" );
    RBOUT( "the project was changed to RevBayes, as it now had little resemblance to\n" );
    RBOUT( "MrBayes. Nicolas Lartillot also started contributing to the project. An ini-\n" );
    RBOUT( "tial version of RevBayes was completed by the summer of 2011 and demonstrated\n" );
    RBOUT( "at a workshop in Berkeley in August the same year.\n" );
    RBOUT( "\n" );
    RBOUT( "At that point, RevBayes did not yet support phylogenetic models. Hohna took\n" );
    RBOUT( "over the lead in the development of RevBayes in the summer of 2011, with the\n" );
    RBOUT( "goal of completing the software. Over the coming years, several new people\n" );
    RBOUT( "joined the development effort, including Bastien Boussau, Tracy Heath, and \n" );
    RBOUT( "Michael Landis. Some of the more advanced language features were hammered out\n" );
    RBOUT( "in 2012 in a group consisting of Hohna, Ronquist, and Lars Arvestad.\n" );
    RBOUT( "\n" );
    RBOUT( "A milestone was reached in August of 2014, when the first almost complete\n" );
    RBOUT( "version of RevBayes was presented to a group of students at a NESCent work-\n" );
    RBOUT( "shop. The program then included support for most language features, a wide\n" );
    RBOUT( "range of phylogenetic models, and several popular types of analyses.\n" );
    RBOUT( "\n" );
    RBOUT( "We expect the first version of RevBayes with complete Rev language support\n" );
    RBOUT( "to appear in late 2014, and the release note should appear in print in 2015.\n" );
    RBOUT( "As the project now quickly accumulates supporters, it is getting more and\n" );
    RBOUT( "more challenging to keep track of the contributions. Hopefully, this recount\n" );
    RBOUT( "of the early development of the project at least names all the relevant\n" );
    RBOUT( "people involved up to the summer of 2014.\n" );
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

