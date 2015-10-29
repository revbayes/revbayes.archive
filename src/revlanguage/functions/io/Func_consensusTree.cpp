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
#include "RlTreeTrace.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "TreeSummary.h"
#include "TreeTrace.h"

#include <map>
#include <set>
#include <sstream>

using namespace RevLanguage;


/** Clone object */
Func_consensusTree* Func_consensusTree::clone(void) const
{
    
    return new Func_consensusTree(*this);
}


/** Execute function */
RevPtr<RevVariable> Func_consensusTree::execute(void)
{
    
    const TreeTrace& tt = static_cast<const TreeTrace&>( args[0].getVariable()->getRevObject() );
    const std::string& filename = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue();
    double cutoff = static_cast<const RealPos &>(args[2].getVariable()->getRevObject()).getValue();
    int burnin = static_cast<const Integer &>(args[3].getVariable()->getRevObject()).getValue();
    RevBayesCore::TreeSummary summary = RevBayesCore::TreeSummary( tt.getValue() );
    RevBayesCore::Tree* tree = summary.conTree(cutoff, burnin);
    
    if ( filename != "" )
    {
        
        RevBayesCore::NexusWriter writer(filename);
        writer.openStream();
        
        std::vector<RevBayesCore::Taxon> taxa;
        tree->getRoot().getTaxa(taxa);
        RevBayesCore::Clade c( taxa, 0.0 );
        writer.writeNexusBlock(c);
        
        writer.writeNexusBlock(*tree);
        
        writer.closeStream();
        
    }
    
    return new RevVariable( new Tree( tree ) );
}



/** Get argument rules */
const ArgumentRules& Func_consensusTree::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
        
        argumentRules.push_back( new ArgumentRule( "treetrace", TreeTrace::getClassTypeSpec(), "The trace of tree samples.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "file"     , RlString::getClassTypeSpec() , "The name of the file for storing the tree.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "cutoff"   , RealPos::getClassTypeSpec()  , "The minimum threshold for clade probabilities.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "burnin"   , Integer::getClassTypeSpec()  , "The number of samples to discard as burnin.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Integer(-1) ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_consensusTree::getClassType(void)
{
    
    static std::string revType = "Func_consensusTree";
    
    return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_consensusTree::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
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


/** Get type spec */
const TypeSpec& Func_consensusTree::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_consensusTree::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = Tree::getClassTypeSpec();
    
    return returnTypeSpec;
}

