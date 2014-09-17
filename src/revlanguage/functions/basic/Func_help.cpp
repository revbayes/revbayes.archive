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
Func_help::Func_help( void ) : Function() {
    
}


/** Clone object */
Func_help* Func_help::clone( void ) const {
    
    return new Func_help( *this );
}


/** Execute function */
RevPtr<Variable> Func_help::execute( void ) {
    
    
    RBOUT( "\n" );
    RBOUT( "Welcome to the RevBayes help!\n");
    RBOUT( "=============================\n\n" );

    std::string helpContent;
    helpContent.append( "RevBayes provides an R-like environment for statistical computation. " );
    helpContent.append( "It is primarily intended for modeling, simulation, and Bayesian inference in evolutionary biology, particularly phylogenetics. " );
    helpContent.append( "However, the environment is quite general and can be useful in any field dealing with complex stochastic models." );
    RBOUT( helpContent );
    RBOUT( "\n" );
    
    helpContent = "";
    helpContent.append( "RevBayes uses its own language, Rev, which is similar to the language used in R. " );
    helpContent.append( "Like the R language, Rev is designed to support interactive analysis. " );
    helpContent.append( "Important differences include the support in Rev for object-oriented programing and for stepwise construction of complex models. ");
    helpContent.append( "Rev supports both functional and procedural programming models, and make a clear distinction between the two. ");
    helpContent.append( "It is also more strongly typed than R." );
    RBOUT( helpContent );
    RBOUT( "\n" );
    
    helpContent = "";
    helpContent.append( "RevBayes uses graphical model concepts like BUGS, JAGS, STAN and related software. " );
    helpContent.append( "It recognizes three different kinds of model variables: constant, deterministic, and stochastic. " );
    helpContent.append( "Each kind of model variable is created through a particular type of assignment. " );
    helpContent.append( "A constant variable is created through a left-arrow assignment:" );
    RBOUT( helpContent );
    RBOUT( "\n" );
    
    RBOUT( "   a <- 1.0\n" );
    RBOUT( "\n" );
    
    helpContent = "";
    helpContent.append( "A deterministic variable is created through an equation assignment using a colon and an equal sign:" );
    RBOUT( helpContent );
    RBOUT( "\n" );
    
    helpContent.append( "\n" );
    helpContent.append( "   b := exp( a )\n" );
    helpContent.append( "\n" );
    
    helpContent = "";
    helpContent.append( "A deterministic variable differs from a constant variable in that its value changes dynamically when the values of the variables in its expression change." );
    RBOUT( helpContent );
    RBOUT( "\n" );
    
    helpContent = "";
    helpContent.append( "Finally, a stochastic variable is created through a tilde assignment:" );
    RBOUT( helpContent );
    RBOUT( "\n" );
    
    helpContent.append( "   c ~ dnExp( b )\n" );
    helpContent.append( "\n" );
    
    helpContent = "";
    helpContent.append( "In general, Rev provides distribution functions with the same name as in R." );
    helpContent.append( "For instance, 'rnorm' draws samples from the normal distribution, 'dnorm' calculates the density for a particular value, etc. " );
    helpContent.append( "By just dropping the initial letter, you get the distribution name used in stochastic assignments, 'norm'. " );
    helpContent.append( "Rev also supports canonical distribution names starting with 'dn' to better support tab completion. " );
    helpContent.append( "For the normal distribution, this form is 'dnNorm', which is a synonym of 'norm'." );
    RBOUT( helpContent );
    RBOUT( "\n" );
    
    helpContent = "";
    helpContent.append( "At any step in the model-building process, you can examine the model pieces in several different ways. " );
    helpContent.append( "By just typing the name of the variable, you print its value. " );
    helpContent.append( "The 'structure' or 'str' function prints detailed information on the structure of the variable. " );
    helpContent.append( "Finally, the 'model' function creates a copy of the model graph and prints it so that you can examine it. " );
    helpContent.append( "To see the value of a variable 'x', its structure, and the model it is connected to, use the following three statements, respectively:" );
    RBOUT( helpContent );
    RBOUT( "\n" );
    
    RBOUT( "   x\n" );
    RBOUT( "   str(x)\n" );
    RBOUT( "   model(x)\n" );
    RBOUT( "\n" );
    
    helpContent = "";
    helpContent.append( "To list the workspace objects, use the 'ls()' function. " );
    helpContent.append( "If you pass 'true' as an argument, 'ls(true)', the listing will include all global workspace objects in addition to the objects you have defined in your user workspace." );
    helpContent.append( "This will provide a complete listing of the available functions. " );
    helpContent.append( "You clear the user workspace using 'clear()', and read in Rev commands from a file using 'source(\"myfilename\")'." );
    RBOUT( helpContent );
    RBOUT( "\n" );
    
    helpContent = "";
    helpContent.append( "Just typing a function name will result in printing of the call signature. " );
    helpContent.append( "If you precede the function name with a question mark, you will obtain the help for that function. " );
    helpContent.append( "Try both with the normal distribution constructor using" );
    RBOUT( helpContent );
    RBOUT( "\n" );
    
    RBOUT( "   norm\n" );
    RBOUT( "   ? norm\n" );
    RBOUT( "\n" );
    
    helpContent = "";
    helpContent.append( "For more information on RevBayes and how to use it for modeling, simulation, and inference, see the RevBayes website at http://revbayes.net." );
    RBOUT( helpContent );
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

