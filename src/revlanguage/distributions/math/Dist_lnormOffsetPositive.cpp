#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_lnormOffsetPositive.h"
#include "LognormalWithOffsetDistribution.h"
#include "Real.h"
#include "RealPos.h"

using namespace RevLanguage;

Dist_lnormOffsetPositive::Dist_lnormOffsetPositive() : PositiveContinuousDistribution()
{
    
}



Dist_lnormOffsetPositive* Dist_lnormOffsetPositive::clone( void ) const
{
    return new Dist_lnormOffsetPositive(*this);
}


RevBayesCore::LognormalWithOffsetDistribution* Dist_lnormOffsetPositive::createDistribution( void ) const
{
    // get the parameters
    RevBayesCore::TypedDagNode<double>* m   = static_cast<const Real &>( mean->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* s   = static_cast<const RealPos &>( sd->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* o   = static_cast<const RealPos &>( offset->getRevObject() ).getDagNode();
    RevBayesCore::LognormalWithOffsetDistribution* d  = new RevBayesCore::LognormalWithOffsetDistribution(m, s, o);
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_lnormOffsetPositive::getClassType(void)
{
    
    static std::string revType = "Dist_lnormOffsetPositive";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_lnormOffsetPositive::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( PositiveContinuousDistribution::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/** Get the help entry for this class */
RevBayesCore::RbHelpFunction* MemberProcedure::getHelpEntry( void ) const
{
    // create the help function entry that we will fill with some values
    RevBayesCore::RbHelpFunction *help = new RevBayesCore::RbHelpFunction();
    RevBayesCore::RbHelpFunction &helpEntry = *help;
    
    std::string entry = "";
    
    // name
    helpEntry.setName( "name" );
    
    // aliases
    std::vector<std::string> aliases = std::vector<std::string>();
    //    aliases.push_back( "alias" );
    helpEntry.setAliases( aliases );
    
    // title
    helpEntry.setTitle( "title" );
    
    // description
    std::vector<std::string> desc = std::vector<std::string>();
    desc.push_back("");
    helpEntry.setDescription( desc );
    
    // usage
    helpEntry.setUsage( "usage" );
    
    // arguments
    std::vector<RevBayesCore::RbHelpArgument> arguments = std::vector<RevBayesCore::RbHelpArgument>();
    RevBayesCore::RbHelpArgument argument = RevBayesCore::RbHelpArgument();
    argument.setLabel(                  "label"                 );
    argument.setDescription(            "description"           );
    argument.setArgumentDagNodeType(    "argumentNodeType"      );
    argument.setArgumentPassingMethod(  "argumentPassingMethod" );
    argument.setValueType(              "valueType"             );
    argument.setDefaultValue(           "defaultValue"          );
    
    // loop options
    std::vector<std::string> options = std::vector<std::string>();
    std::string option = std::string( "o" );
    options.push_back( option );
    argument.setOptions( options );
    
    // add the argument to the argument list
    arguments.push_back( argument );
    
    helpEntry.setArguments( arguments );
    
    // return value
    helpEntry.setReturnType( "returnValue" );
    
    // details
    std::vector<std::string> details = std::vector<std::string>();
    details.push_back( "details" );
    
    helpEntry.setDetails( details );
    
    // example
    helpEntry.setExample( "example" );
    
    // reference
    std::vector<RevBayesCore::RbHelpReference> references = std::vector<RevBayesCore::RbHelpReference>();
    RevBayesCore::RbHelpReference ref = RevBayesCore::RbHelpReference();
    ref.setCitation( "citation" );
    ref.setDoi( "doi" );
    ref.setUrl( "url" );
    references.push_back( ref );
    
    helpEntry.setReferences( references );
    
    // author
    helpEntry.setAuthor( "author" );
    
    // see also
    std::vector<std::string> seeAlso = std::vector<std::string>();
    seeAlso.push_back( "see also" );
    
    helpEntry.setSeeAlso(seeAlso);
    
    return help;
    
}




/** Return member rules (no members) */
const MemberRules& Dist_lnormOffsetPositive::getParameterRules(void) const
{
    
    static MemberRules distLnormMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        distLnormMemberRules.push_back( new ArgumentRule( "mean",   Real::getClassTypeSpec()   , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distLnormMemberRules.push_back( new ArgumentRule( "sd"  ,   RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distLnormMemberRules.push_back( new ArgumentRule( "offset", RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return distLnormMemberRules;
}


const TypeSpec& Dist_lnormOffsetPositive::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_lnormOffsetPositive::printValue(std::ostream& o) const {
    
    o << "lognormal(mean=";
    if ( mean != NULL ) {
        o << mean->getName();
    } else {
        o << "?";
    }
    o << ", sd=";
    if ( sd != NULL ) {
        o << sd->getName();
    } else {
        o << "?";
    }
    o << ", offset=";
    if ( offset != NULL ) {
        o << offset->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Dist_lnormOffsetPositive::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "mean" )
    {
        mean = var;
    }
    else if ( name == "sd" )
    {
        sd = var;
    }
    else if ( name == "offset" )
    {
        offset = var;
    }
    else
    {
        PositiveContinuousDistribution::setConstParameter(name, var);
    }
}
