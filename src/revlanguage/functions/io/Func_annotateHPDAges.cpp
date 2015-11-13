#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Func_annotateHPDAges.h"
#include "ModelVector.h"
#include "NexusWriter.h"
#include "Probability.h"
#include "RbException.h"
#include "RevNullObject.h"
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
Func_annotateHPDAges* Func_annotateHPDAges::clone( void ) const
{
    
    return new Func_annotateHPDAges( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_annotateHPDAges::execute( void )
{
    
    // get the x% hpd
    double x = static_cast<const Probability &>(args[0].getVariable()->getRevObject()).getValue();
    
    // get the input tree
    RevBayesCore::Tree* tree = static_cast<const Tree&>( this->args[1].getVariable()->getRevObject() ).getValue().clone();
    
    // get the  tree trace
    const TreeTrace& tt = static_cast<const TreeTrace&>( args[2].getVariable()->getRevObject() );
    
    // get the filename
    const std::string& filename = static_cast<const RlString&>( args[3].getVariable()->getRevObject() ).getValue();
    
    // get burnin
    int burnin = static_cast<const Integer &>(args[4].getVariable()->getRevObject()).getValue();
    
    // make a new tree summary object
    RevBayesCore::TreeSummary summary = RevBayesCore::TreeSummary( tt.getValue() );
    
    // get the tree with x% HPD node ages
    summary.annotateHPDAges(*tree, x, burnin);
    
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
const ArgumentRules& Func_annotateHPDAges::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
        argumentRules.push_back( new ArgumentRule( "hpd"   ,    Probability::getClassTypeSpec() , "The probability contained in the highest posterior density interval.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.95) ) );
        argumentRules.push_back( new ArgumentRule( "inputtree", Tree::getClassTypeSpec()        , "The input tree which will be annotated.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "treetrace", TreeTrace::getClassTypeSpec()   , "The sample trace.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "file"     , RlString::getClassTypeSpec()    , "The name of the file where to store the tree.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "burnin"   , Integer::getClassTypeSpec()     , "The number of samples to discard as burnin.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Integer(-1) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_annotateHPDAges::getClassType(void)
{
    
    static std::string revType = "Func_annotateHPDAges<" + TimeTree::getClassType() + ">";
    
    return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_annotateHPDAges::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_annotateHPDAges::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "annotateHPDAges";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_annotateHPDAges::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_annotateHPDAges::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = TimeTree::getClassTypeSpec();
    return returnTypeSpec;
}

