#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Func_mapTree.h"
#include "ModelVector.h"
#include "NexusWriter.h"
#include "RbException.h"
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
Func_mapTree* Func_mapTree::clone( void ) const
{
    
    return new Func_mapTree( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_mapTree::execute( void )
{
    
    // get the x% hpd
    double x = 0.95;
    
    const TreeTrace& tt = static_cast<const TreeTrace&>( args[0].getVariable()->getRevObject() );
    const std::string& filename = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue();
    int burnin = static_cast<const Integer &>(args[2].getVariable()->getRevObject()).getValue();
    
    RevBayesCore::TreeSummary summary = RevBayesCore::TreeSummary( tt.getValue() );
    RevBayesCore::Tree* tree = summary.map(burnin);
    
    // get the tree with x% HPD node ages
    summary.annotateHPDAges(*tree, x, burnin);
    
    // get the tree with x% HPD node ages
    summary.annotate(*tree, burnin);
    
    
    if ( filename != "" )
    {
        
        RevBayesCore::NexusWriter writer(filename);
        writer.openStream();
        
        std::vector<std::string> taxa;
        tree->getRoot().getTaxaStringVector(taxa);
        RevBayesCore::Clade c( taxa, 0.0 );
        writer.writeNexusBlock(c);
        
        writer.writeNexusBlock(*tree);
        
        writer.closeStream();
        
    }
    
    return new RevVariable( new Tree( tree ) );
}



/** Get argument rules */
const ArgumentRules& Func_mapTree::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
        
        argumentRules.push_back( new ArgumentRule( "treetrace", TreeTrace::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "file"     , RlString::getClassTypeSpec()           , ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "burnin"   , Integer::getClassTypeSpec()            , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Integer(-1) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_mapTree::getClassType(void)
{
    
    static std::string revType = "Func_mapTree";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_mapTree::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}

/** Get type spec */
const TypeSpec& Func_mapTree::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_mapTree::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = Tree::getClassTypeSpec();
    return returnTypeSpec;
}

