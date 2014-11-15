/**
 * @file
 * This file contains the implementation of Func_help, which is
 * the function used to provide basic help.
 *
 * @brief Implementation of Func_help
 *
 * @author Fredrik Ronquist
 * @license GPL version 3
 */

#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_help.h"
#include "RbException.h"
#include "RlBoolean.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "RlUserInterface.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_help::Func_help( void ) : Procedure() {
    
}


/** Clone object */
Func_help* Func_help::clone( void ) const {
    
    return new Func_help( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_help::execute( void ) {
    
    
    RBOUT( "\n" );
    RBOUT( "Welcome to the RevBayes help!\n");
    RBOUT( "=============================\n\n" );

    // Use the lines below to count column positions in the output. The text is currenthly
    // hard-wrapped at 80 columns wide, assuming a three-column padding to the left.
#if 0
    RBOUT( "00000011111111112222222222333333333344444444445555555555666666666677777777778" );
    RBOUT( "45678901234567890123456789012345678901234567890123456789012345678901234567890" );
#endif

    RBOUT( "RevBayes provides an R-like environment for statistical computation. It is\n" );
    RBOUT( "primarily intended for modeling, simulation, and Bayesian inference in evolu-\n" );
    RBOUT( "tionary biology, particularly phylogenetics. However, the environment is\n" );
    RBOUT( "quite general and can be useful in any field dealing with complex stochastic\n" );
    RBOUT( "models.\n" );
    RBOUT( "\n" );
    RBOUT( "RevBayes uses its own language, Rev, which is similar to the language used in\n" );
    RBOUT( "R. Like the R language, Rev is designed to support interactive analysis. Im-\n" );
    RBOUT( "portant differences include the support in Rev for object-oriented program-\n" );
    RBOUT( "ming and for stepwise construction of complex models. Rev supports both func-\n" );
    RBOUT( "tional and procedural programming models, and make a clear distinction be-\n" );
    RBOUT( "tween the two. It is also more strongly typed than R.\n" );
    RBOUT( "\n" );
    RBOUT( "RevBayes uses graphical model concepts like BUGS, JAGS, STAN and related\n" );
    RBOUT( "software. It recognizes three different kinds of model variables: constant,\n" );
    RBOUT( "deterministic, and stochastic. Each kind of model variable is created through\n" );
    RBOUT( "a particular type of assignment. A constant variable is created through a\n" );
    RBOUT( "left-arrow assignment:\n" );
    RBOUT( "\n" );
    RBOUT( "   a <- 1.0\n" );
    RBOUT( "\n" );
    RBOUT( "A deterministic variable is created through an equation assignment using a\n" );
    RBOUT( "colon and an equal sign:\n" );
    RBOUT( "\n" );
    RBOUT( "   b := exp( a )\n" );
    RBOUT( "\n" );
    RBOUT( "A deterministic variable differs from a constant variable in that its value\n" );
    RBOUT( "changes dynamically when the values of the variables in its expression\n" );
    RBOUT( "change.\n" );
    RBOUT( "\n" );
    RBOUT( "Finally, a stochastic variable is created through a tilde assignment:\n" );
    RBOUT( "\n" );
    RBOUT( "   c ~ exponential( b )\n" );
    RBOUT( "\n" );
    RBOUT( "In general, Rev provides distribution functions with the same name as in R.\n" );
    RBOUT( "For instance, 'rnorm' draws samples from the normal distribution, 'dnorm'\n" );
    RBOUT( "calculates the density for a particular value, etc. By just dropping the ini-\n" );
    RBOUT( "tial letter, you get the distribution name used in stochastic assignments,\n" );
    RBOUT( "'norm'. Rev also supports canonical distribution names starting with 'dn' to\n" );
    RBOUT( "better support tab completion. For the normal distribution, this form is\n" );
    RBOUT( "'dnNorm', which is a synonym of 'norm'.\n" );
    RBOUT( "\n" );
    RBOUT( "At any step in the model-building process, you can examine the model pieces\n" );
    RBOUT( "in several different ways. By just typing the name of the variable, you print\n" );
    RBOUT( "its value. The 'structure' or 'str' function prints detailed information on\n" );
    RBOUT( "the structure of the variable. Finally, the 'model' function creates a copy\n" );
    RBOUT( "of the model graph and prints it so that you can examine it. To see the value\n" );
    RBOUT( "of a variable 'x', its structure, and the model it is connected to, use the\n" );
    RBOUT( "following three statements, respectively:\n" );
    RBOUT( "\n" );
    RBOUT( "   x\n" );
    RBOUT( "   str(x)\n" );
    RBOUT( "   model(x)\n" );
    RBOUT( "\n" );
    RBOUT( "To list the workspace objects, use the 'ls()' function. If you pass 'true' as\n" );
    RBOUT( "an argument, 'ls(true)', the listing will include all global workspace ob-\n" );
    RBOUT( "jects in addition to the objects you have defined in your user workspace.\n" );
    RBOUT( "This will provide a complete listing of the available functions. You clear\n" );
    RBOUT( "the user workspace using 'clear()', and read in Rev commands from a file\n" );
    RBOUT( "using 'source(\"myfilename\")'.\n" );
    RBOUT( "\n" );
    RBOUT( "Just typing a function name will result in printing of the call signature. If\n" );
    RBOUT( "you precede the function name with a question mark, you will obtain the help\n" );
    RBOUT( "for that function. Try both with the normal distribution constructor using\n" );
    RBOUT( "\n" );
    RBOUT( "   norm\n" );
    RBOUT( "   ? norm\n" );
    RBOUT( "\n" );
    RBOUT( "For more information on Rev and how to use it for modeling, simulation, and\n" );
    RBOUT( "inference, see the RevBayes website at http://revbayes.net.\n" );
    RBOUT( "\n" );

    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_help::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "all", RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_help::getClassType(void) {
    
    static std::string revType = "Func_help";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_help::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}

/** Get type spec */
const TypeSpec& Func_help::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_help::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

