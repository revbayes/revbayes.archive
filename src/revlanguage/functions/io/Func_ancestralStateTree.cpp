#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Func_ancestralStateTree.h"
#include "ModelVector.h"
#include "NexusWriter.h"
#include "RbException.h"
#include "RevNullObject.h"
#include "RlBranchLengthTree.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "RlTreeTrace.h"
#include "RlAncestralStateTrace.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "TreeSummary.h"
#include "TreeTrace.h"
#include "AncestralStateTrace.h"
#include "WorkspaceVector.h"

#include <map>
#include <set>
#include <sstream>


using namespace RevLanguage;

/** Clone object */
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
    const TreeTrace& tt = static_cast<const TreeTrace&>( args[2].getVariable()->getRevObject() );
    
    // get the filename
    const std::string& filename = static_cast<const RlString&>( args[3].getVariable()->getRevObject() ).getValue();
    
    // make a new tree summary object
    RevBayesCore::TreeSummary summary = RevBayesCore::TreeSummary( tt.getValue() );
    
    int burnin = static_cast<const Integer &>(args[4].getVariable()->getRevObject()).getValue();
    
    // get the tree with ancestral states
    RevBayesCore::Tree* tree = summary.ancestralStateTree(it->getValue(), ancestralstate_traces, burnin);
    
    // return the tree
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
const ArgumentRules& Func_ancestralStateTree::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
        
        argumentRules.push_back( new ArgumentRule( "inputtree", Tree::getClassTypeSpec(), "The input tree.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "ancestralstatetrace_vector", WorkspaceVector<AncestralStateTrace>::getClassTypeSpec(), "A vector of ancestral state traces.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "treetrace", TreeTrace::getClassTypeSpec(), "A vector (trace) of tree samples.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "file"     , RlString::getClassTypeSpec() , "The name of the file where to store the annotated tree.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "burnin"   , Integer::getClassTypeSpec()  , "The number of samples to discard as burnin.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Integer(-1) ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_ancestralStateTree::getClassType(void)
{
    
    static std::string revType = "Func_ancestralStateTree";
    
    return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_ancestralStateTree::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
const std::string& Func_ancestralStateTree::getFunctionName( void ) const
{
    // create a static name variable that is the same for all instance of this class
    static std::string f_name = "ancestralStateTree";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_ancestralStateTree::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_ancestralStateTree::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = Tree::getClassTypeSpec();
    return returnTypeSpec;
}

