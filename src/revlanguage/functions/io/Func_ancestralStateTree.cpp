#include "ArgumentRule.h"
#include "OptionRule.h"
#include "ConstantNode.h"
#include "Func_ancestralStateTree.h"
#include "ModelVector.h"
#include "NexusWriter.h"
#include "RbException.h"
#include "RevNullObject.h"
#include "RlBranchLengthTree.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "RlTraceTree.h"
#include "RlAncestralStateTrace.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "TreeSummary.h"
#include "TraceTree.h"
#include "AncestralStateTrace.h"
#include "WorkspaceVector.h"

#include <map>
#include <set>
#include <sstream>


using namespace RevLanguage;

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_ancestralStateTree* Func_ancestralStateTree::clone( void ) const
{
    
    return new Func_ancestralStateTree( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_ancestralStateTree::execute( void )
{
    
    // get the input tree
    const RevBayesCore::TypedDagNode<RevBayesCore::Tree> *it = static_cast<const Tree&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    
    // get vector of ancestral state traces
    const WorkspaceVector<AncestralStateTrace>& ast_vector = static_cast<const WorkspaceVector<AncestralStateTrace> &>( args[1].getVariable()->getRevObject() );
    std::vector<RevBayesCore::AncestralStateTrace> ancestralstate_traces;
    for (int i = 0; i < ast_vector.size(); ++i)
    {
        ancestralstate_traces.push_back( ast_vector[i].getValue() );
    }
    
    // get the ancestral state tree trace
    const TraceTree& tt = static_cast<const TraceTree&>( args[2].getVariable()->getRevObject() );
    
    // make a new tree summary object
    RevBayesCore::TreeSummary summary;
    if (args[2].getVariable()->getRevObject() != RevNullObject::getInstance())
    {
        summary = tt.getValue();
    }
    else
    {
        summary = RevBayesCore::TreeSummary();
    }
    
    // should we annotate start states?
    bool start_states = static_cast<const RlBoolean &>(args[3].getVariable()->getRevObject()).getValue();
    
    // get the filename
    const std::string& filename = static_cast<const RlString&>( args[4].getVariable()->getRevObject() ).getValue();
    
    int burnin = static_cast<const Integer &>(args[5].getVariable()->getRevObject()).getValue();
    
    std::string summary_stat = static_cast<const RlString&>( args[6].getVariable()->getRevObject() ).getValue();
    
    int site = static_cast<const Integer &>(args[7].getVariable()->getRevObject()).getValue() - 1;
    
    bool verbose = static_cast<const RlBoolean &>(args[8].getVariable()->getRevObject()).getValue();
    
    // get the tree with ancestral states
    RevBayesCore::Tree* tree;
    if (start_states)
    {
        tree = summary.cladoAncestralStateTree(it->getValue(), ancestralstate_traces, burnin, summary_stat, site, verbose);
    }
    else
    {
        tree = summary.ancestralStateTree(it->getValue(), ancestralstate_traces, burnin, summary_stat, site, verbose);
    }
    
    // return the tree
    if ( filename != "" )
    {
        
        RevBayesCore::NexusWriter writer(filename);
        writer.openStream(false);
        
        std::vector<RevBayesCore::Taxon> taxa;
        tree->getRoot().getTaxa(taxa);
        RevBayesCore::Clade c( taxa );
        writer.writeNexusBlock(c);
        
        writer.writeNexusBlock(*tree);
        
        writer.closeStream();
        
    }
    
    return new RevVariable( new Tree( tree ) );
}



/** Get argument rules */
const ArgumentRules& Func_ancestralStateTree::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rules_set = false;
    
    if (!rules_set)
    {
        
        argumentRules.push_back( new ArgumentRule( "tree", Tree::getClassTypeSpec(), "The input tree to summarize ancestral states over.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "ancestral_state_trace_vector", WorkspaceVector<AncestralStateTrace>::getClassTypeSpec(), "A vector of ancestral state traces.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "tree_trace", TraceTree::getClassTypeSpec(), "A trace of tree samples.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        argumentRules.push_back( new ArgumentRule( "include_start_states", RlBoolean::getClassTypeSpec(), "Annotate start states as well as end states for each branch. Only applicable for cladogenetic processes.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean( false ) ) );
        argumentRules.push_back( new ArgumentRule( "file"     , RlString::getClassTypeSpec() , "The name of the file to store the annotated tree.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "burnin"   , Integer::getClassTypeSpec()  , "The number of samples to discard as burnin.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Integer(-1) ) );
        std::vector<std::string> summary_stats;
        summary_stats.push_back( "MAP" );
        summary_stats.push_back( "mean" );
        argumentRules.push_back( new OptionRule( "summary_statistic", new RlString("MAP"), summary_stats, "The statistic used to summarize ancestral states. 'MAP' displays the 3 states with highest posterior probabilities. 'mean' displays the mean value and 95% CI." ) );
        argumentRules.push_back( new ArgumentRule( "site"     , Integer::getClassTypeSpec()  , "The character site to be summarized.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Integer(1) ) );
        argumentRules.push_back( new ArgumentRule( "verbose"   , RlBoolean::getClassTypeSpec()  , "Printing verbose output", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
        
        rules_set = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_ancestralStateTree::getClassType(void)
{
    
    static std::string rev_type = "Func_ancestralStateTree";
    
    return rev_type;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_ancestralStateTree::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_ancestralStateTree::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "ancestralStateTree";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_ancestralStateTree::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get return type */
const TypeSpec& Func_ancestralStateTree::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = Tree::getClassTypeSpec();
    return returnTypeSpec;
}

