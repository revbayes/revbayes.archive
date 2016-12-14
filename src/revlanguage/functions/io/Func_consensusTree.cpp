#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Func_consensusTree.h"
#include "ModelVector.h"
#include "NexusWriter.h"
#include "RbException.h"
#include "RealPos.h"
#include "RevNullObject.h"
#include "RlBranchLengthTree.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "RlTraceTree.h"
#include "RlUtils.h"
#include "Probability.h"
#include "StringUtilities.h"
#include "TreeSummary.h"
#include "TraceTree.h"

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
Func_consensusTree* Func_consensusTree::clone(void) const
{
    
    return new Func_consensusTree(*this);
}


/** Execute function */
RevPtr<RevVariable> Func_consensusTree::execute(void)
{
    size_t arg_index = 0;

    TraceTree& tt = static_cast<TraceTree&>( args[arg_index++].getVariable()->getRevObject() );
    
    double cutoff = static_cast<const Probability &>(args[arg_index++].getVariable()->getRevObject()).getValue();

    const std::string& filename = static_cast<const RlString&>( args[arg_index++].getVariable()->getRevObject() ).getValue();


    RevBayesCore::AnnotationReport report;

    report.hpd       = static_cast<const Probability &>(args[arg_index++].getVariable()->getRevObject()).getValue();
    report.mean      = static_cast<const RlBoolean &>( this->args[arg_index++].getVariable()->getRevObject() ).getValue();
    report.sa        = static_cast<const RlBoolean &>( this->args[arg_index++].getVariable()->getRevObject() ).getValue();

    RevBayesCore::Tree* tree = tt.getValue().mrTree(report, cutoff);
    
    if ( filename != "" )
    {
        
        RevBayesCore::NexusWriter writer(filename);
        writer.openStream();
        
        std::vector<RevBayesCore::Taxon> taxa;
        tree->getRoot().getTaxa(taxa);
        RevBayesCore::Clade c( taxa );
        writer.writeNexusBlock(c);
        
        writer.writeNexusBlock(*tree);
        
        writer.closeStream();
        
    }
    
    Tree* t;
    if( tt.getValue().getTreeTrace().isClock() )
    {
        t = new TimeTree( tree );
    }
    else
    {
        t = new BranchLengthTree( tree );
    }

    return new RevVariable( t );
}



/** Get argument rules */
const ArgumentRules& Func_consensusTree::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rules_set = false;
    
    if (!rules_set)
    {
        
        argumentRules.push_back( new ArgumentRule( "trace", TraceTree::getClassTypeSpec(), "The trace of tree samples.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "cutoff"   , Probability::getClassTypeSpec()  , "The minimum threshold for clade probabilities.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.5) ) );
        argumentRules.push_back( new ArgumentRule( "file"     , RlString::getClassTypeSpec() , "The name of the file for storing the tree.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("") ) );
        
        argumentRules.push_back( new ArgumentRule( "hpd"   ,    Probability::getClassTypeSpec() , "The probability mass of the highest posterior density age interval.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.95) ) );
        argumentRules.push_back( new ArgumentRule( "mean" , RlBoolean::getClassTypeSpec() , "Annotate node ages using the mean age instead of the median?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        argumentRules.push_back( new ArgumentRule( "sampledAncestors" , RlBoolean::getClassTypeSpec() , "Annotate sampled ancestor probs?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );

        rules_set = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_consensusTree::getClassType(void)
{
    
    static std::string rev_type = "Func_consensusTree";
    
    return rev_type;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_consensusTree::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_consensusTree::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "consensusTree";
    
    return f_name;
}

/**
 * Get the name for this procedure.
 */
std::vector<std::string> Func_consensusTree::getFunctionNameAliases( void ) const
{
    std::vector<std::string> aliases;

    aliases.push_back("conTree");
    aliases.push_back("sumt");

    return aliases;
}


/** Get type spec */
const TypeSpec& Func_consensusTree::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get return type */
const TypeSpec& Func_consensusTree::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = Tree::getClassTypeSpec();
    
    return returnTypeSpec;
}

