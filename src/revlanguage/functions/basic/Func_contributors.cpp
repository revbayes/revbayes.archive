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
Func_contributors::Func_contributors( void ) : Procedure()
{
}


/** Clone object */
Func_contributors* Func_contributors::clone( void ) const
{
    return new Func_contributors( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_contributors::execute( void )
{
    // Use the lines below to count columns, if you wish
#if 0
    RBOUT( "00000011111111112222222222333333333344444444445555555555666666666677777777778" );
    RBOUT( "45678901234567890123456789012345678901234567890123456789012345678901234567890" );
#endif
    
    RBOUT( "\n" );
    RBOUT( "Contributors\n" );
    RBOUT( "============\n" );
    RBOUT( "                                                                            \n");
    RBOUT( "The RevBayes project started as an attempt to provide a more general and    \n");
    RBOUT( "flexible version of the MrBayes software for Bayesian phylogenetic          \n");
    RBOUT( "inference. This development version of MrBayes was named MrBayes 4, as      \n");
    RBOUT( "MrBayes was then in version 3. The initial MrBayes 4 kick-off meeting       \n");
    RBOUT( "was held at the University of California, San Diego, in March of 2005       \n");
    RBOUT( "and involved John Huelsenbeck, Bret Larget, Fredrik Ronquist, Donald        \n");
    RBOUT( "Simon, and Paul van der Mark. Later, Marc Suchard would also contribute     \n");
    RBOUT( "to the development of the project.                                          \n");
    RBOUT( "                                                                            \n");
    RBOUT( "A pilot version of MrBayes 4 was completed in 2007 by Paul van der Mark,    \n");
    RBOUT( "who at the time was a postdoc in the lab of Fredrik Ronquist. MrBayes 4     \n");
    RBOUT( "was first used in a Bayesian island biogeography analysis published in      \n");
    RBOUT( "2008 in the Journal of Biogeography by Isabel Sanmartin, Paul van der       \n");
    RBOUT( "Mark, and Fredrik Ronquist. Several related papers using MrBayes 4          \n");
    RBOUT( "followed.                                                                   \n");
    RBOUT( "                                                                            \n");
    RBOUT( "In August of 2007, Huelsenbeck, Ronquist, and Suchard reviewed the code     \n");
    RBOUT( "of MrBayes 4. The program then had a simple model-specification language    \n");
    RBOUT( "but the code was experimental and difficult to work with, and the           \n");
    RBOUT( "language did not yet support more generic modeling and computing.           \n");
    RBOUT( "                                                                            \n");
    RBOUT( "Ronquist took the lead in rewriting the code and generalizing the model     \n");
    RBOUT( "specification language. The work accelerated in 2009 when Sebastian         \n");
    RBOUT( "Höhna joined the project as a graduate student. In the summer of 2009,      \n");
    RBOUT( "the name of the project was changed to RevBayes, as it now had little       \n");
    RBOUT( "resemblance to MrBayes. Moreover, the MrBayes program was irretrievably     \n");
    RBOUT( "associated with Huelsenbeck and Ronquist, whereas the new code-base had     \n");
    RBOUT( "many new contributors who were adding significantly to the code; these new  \n");
    RBOUT( "contributors needed to be acknowledged. (The new name 'RevBayes' also       \n");
    RBOUT( "solved a criticism Joe Felsenstein made about MrBayes, that Thomas Bayes    \n");
    RBOUT( "was a reverend and that calling him a mister seemed, somehow,               \n");
    RBOUT( "disrespectful. Of course, this was the least of his criticisms as he        \n");
    RBOUT( "isn’t a fan of Bayesian inference in general.)                              \n");
    RBOUT( "                                                                            \n");
    RBOUT( "The new code base and repository, then at SourceForge, was created by       \n");
    RBOUT( "Höhna, Huelsenbeck and Ronquist. The same summer, Nicolas Lartillot         \n");
    RBOUT( "started contributing to the discussion on RevBayes. A prototype version     \n");
    RBOUT( "of RevBayes was completed by the summer of 2011 and demonstrated at a       \n");
    RBOUT( "workshop in Berkeley in August the same year.                               \n");
    RBOUT( "                                                                            \n");
    RBOUT( "At that time, RevBayes did not yet support phylogenetic models. The         \n");
    RBOUT( "prototype merely showed the syntax of the Rev language and the ideas        \n");
    RBOUT( "behind graphical models. Höhna took over the lead in the development of     \n");
    RBOUT( "RevBayes in the summer of 2011. Höhna rewrote the entire software once      \n");
    RBOUT( "again to facilitate phylogenetic models and fast computation of the         \n");
    RBOUT( "model graphs. Over the coming years, several new people joined the          \n");
    RBOUT( "development effort, including Bastien Boussau, Tracy Heath, and Michael     \n");
    RBOUT( "Landis. Some of the more advanced language features were hammered out in    \n");
    RBOUT( "2012 in a group consisting of Höhna, Ronquist, and Lars Arvestad.           \n");
    RBOUT( "                                                                            \n");
    RBOUT( "A milestone was reached in August of 2014, when the first working           \n");
    RBOUT( "version of RevBayes was presented to a group of students at a NESCent       \n");
    RBOUT( "workshop. The program then included support for most language features,     \n");
    RBOUT( "a wide range of phylogenetic models, and several popular types of           \n");
    RBOUT( "analyses.                                                                   \n");
    RBOUT( "                                                                            \n");
    RBOUT( "We expect the first version of RevBayes to be officially released in late   \n");
    RBOUT( "2014, and the release note should appear in print in 2015. As the project   \n");
    RBOUT( "now quickly accumulates supporters, it is getting more and more challenging \n");
    RBOUT( "to keep track of the contributions. Hopefully, this recount of the early    \n");
    RBOUT( "development of the project at least names all the relevant people involved  \n");
    RBOUT( "up to the summer of 2014.                                                   \n");
    RBOUT( "                                                                            \n");
    RBOUT( "The github page                                                             \n\n");
    RBOUT( "   https://github.com/revbayes/revbayes/graphs/contributors                 \n\n");
    RBOUT( "shows all code contributions since the RevBayes code repository was         \n");
    RBOUT( "started on SourceForge in August of 2009. The repository was moved to       \n");
    RBOUT( "github in July of 2014.                                                     \n");
    RBOUT( "                                                                            \n");
    RBOUT( "For more information, go to the program websites (http://revbayes.net and   \n");
    RBOUT( "http:revbayes.com).                                                         \n\n");
    RBOUT( "                                                                            \n");

    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_contributors::getArgumentRules( void ) const
{
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_contributors::getClassType(void)
{
    static std::string revType = "Func_contributors";
    
	return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_contributors::getClassTypeSpec(void)
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/** Get type spec */
const TypeSpec& Func_contributors::getTypeSpec( void ) const
{
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_contributors::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

