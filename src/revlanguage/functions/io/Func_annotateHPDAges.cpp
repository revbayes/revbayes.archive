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
#include "RlTraceTree.h"
#include "RlUtils.h"
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
Func_annotateHPDAges* Func_annotateHPDAges::clone( void ) const
{
    
    return new Func_annotateHPDAges( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_annotateHPDAges::execute( void )
{
    
    // get the input tree
    RevBayesCore::Tree* tree = static_cast<const Tree&>( this->args[0].getVariable()->getRevObject() ).getValue().clone();
    
    // get the  tree trace
    TraceTree& tt = static_cast<TraceTree&>( args[1].getVariable()->getRevObject() );
    
    // get the x% hpd
    double x = static_cast<const Probability &>(args[2].getVariable()->getRevObject()).getValue();

    // get the filename
    const std::string& filename = static_cast<const RlString&>( args[3].getVariable()->getRevObject() ).getValue();
    
    // get burnin
    //int burnin = static_cast<const Integer &>(args[4].getVariable()->getRevObject()).getValue();
    //summary.setBurnin( burnin );

    RevBayesCore::AnnotationReport report;

    report.ages      = false;
    report.ccp       = false;
    report.hpd       = x;
    report.posterior = false;

    tt.getValue().annotateTree(*tree, report);
    
    // return the tree
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
    
    return new RevVariable( new Tree( tree ) );
}



/** Get argument rules */
const ArgumentRules& Func_annotateHPDAges::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rules_set = false;
    
    if (!rules_set)
    {
        argumentRules.push_back( new ArgumentRule( "tree", Tree::getClassTypeSpec()        , "The input tree which will be annotated.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "trace", TraceTree::getClassTypeSpec()   , "The sample trace.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "hpd"   ,    Probability::getClassTypeSpec() , "The probability contained in the highest posterior density interval.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.95) ) );
        argumentRules.push_back( new ArgumentRule( "file"     , RlString::getClassTypeSpec()    , "The name of the file where to store the tree.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        //argumentRules.push_back( new ArgumentRule( "burnin"   , Integer::getClassTypeSpec()     , "The number of samples to discard as burnin.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Integer(-1) ) );
        rules_set = true;
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
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
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
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get return type */
const TypeSpec& Func_annotateHPDAges::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = TimeTree::getClassTypeSpec();
    return returnTypeSpec;
}

