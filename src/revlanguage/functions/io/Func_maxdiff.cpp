#include <stddef.h>
#include <sstream>
#include <string>
#include <vector>

#include "ArgumentRule.h"
#include "Func_maxdiff.h"
#include "Real.h"
#include "RlTraceTree.h"
#include "RlTree.h"
#include "TypeSpec.h"
#include "WorkspaceVector.h"


using namespace RevLanguage;


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_maxdiff* Func_maxdiff::clone( void ) const
{
    
    return new Func_maxdiff( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_maxdiff::execute( void )
{
    size_t arg_index = 0;

    std::vector<TraceTree>& tt = static_cast<const WorkspaceVector<TraceTree> &>(args[arg_index++].getVariable()->getRevObject()).getValue();
    
    std::vector<RevBayesCore::TraceTree* > vec;
    for(size_t i = 0; i < tt.size(); i++)
    {
        vec.push_back(&(tt[i].getValue()));
    }

    RevBayesCore::TreeSummary summary( vec, tt.front().getValue().isClock() );

    return new RevVariable( new Real( summary.maxdiff(true) ) );
}



/** Get argument rules */
const ArgumentRules& Func_maxdiff::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rules_set = false;
    
    if (!rules_set)
    {
        
        std::vector<TypeSpec> traceArgRules;
        traceArgRules.push_back( WorkspaceVector<TraceTree>::getClassTypeSpec() );
        argumentRules.push_back( new ArgumentRule( "traces", traceArgRules, "The samples of trees from the posterior.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );

        rules_set = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_maxdiff::getClassType(void)
{
    
    static std::string revType = "Func_maxdiff";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_maxdiff::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the author(s) of this function so they can receive credit (and blame) for it.
 */
std::vector<std::string> Func_maxdiff::getHelpAuthor(void) const
{
    // create a vector of authors for this function
    std::vector<std::string> authors;
    authors.push_back( "Will Pett" );
    
    return authors;
}


/**
 * Get the (brief) description for this function
 */
std::string Func_maxdiff::getHelpDescription(void) const
{
    std::string description = "";
    description += "Finds the maximum difference in clade probabilities between two posterior samples.";
    
    return description;
}


/**
 * Get an executable and instructive example.
 * These example should help the users to show how this function works but
 * are also used to test if this function still works.
 */
std::string Func_maxdiff::getHelpExample(void) const
{
    // create an example as a single string variable.
    std::string example = "";
    
    example += "# Read in tree trace\n";
    example += "tree_trace = readTreeTrace(\"output/my.trees\", burnin=0.25, nruns=2)\n";
    example += "\n";
    example += "# Compute the maxdiff statistic\n";
    example += "maxdiff = maxdiff(traces=tree_trace)\n";
    
    return example;
}


/**
 * Get the names of similar and suggested other functions
 */
std::vector<std::string> Func_maxdiff::getHelpSeeAlso(void) const
{
    // create an entry for each suggested function
    std::vector<std::string> see_also;
    see_also.push_back( "readTreeTrace" );
    
    
    return see_also;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_maxdiff::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "maxdiff";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_maxdiff::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get return type */
const TypeSpec& Func_maxdiff::getReturnType( void ) const
{
    
    static TypeSpec return_typeSpec = Tree::getClassTypeSpec();
    return return_typeSpec;
}

