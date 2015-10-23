#include "PattersonsDFunction.h"
#include "Func_PattersonsD.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "RlString.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_PattersonsD::Func_PattersonsD( void ) : TypedFunction<Real>( )
{
    
}


/** Clone object */
Func_PattersonsD* Func_PattersonsD::clone( void ) const
{
    
    return new Func_PattersonsD( *this );
}


RevBayesCore::TypedFunction< double >* Func_PattersonsD::createFunction( void ) const
{
    std::string p1 = static_cast<const RlString &>( this->args[0].getVariable()->getRevObject() ).getValue();
    std::string p2 = static_cast<const RlString &>( this->args[1].getVariable()->getRevObject() ).getValue();
    std::string p3 = static_cast<const RlString &>( this->args[2].getVariable()->getRevObject() ).getValue();
    std::string og = static_cast<const RlString &>( this->args[3].getVariable()->getRevObject() ).getValue();
    
    RevBayesCore::TypedDagNode<RevBayesCore::AbstractHomologousDiscreteCharacterData >* d = static_cast<const AbstractHomologousDiscreteCharacterData &>( this->args[4].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::PattersonsDFunction* f = new RevBayesCore::PattersonsDFunction( p1, p2, p3, og, d );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_PattersonsD::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "p1"      , RlString::getClassTypeSpec(), "",                                ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        argumentRules.push_back( new ArgumentRule( "p2"      , RlString::getClassTypeSpec(), "",                                ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        argumentRules.push_back( new ArgumentRule( "p3"      , RlString::getClassTypeSpec(), "",                                ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        argumentRules.push_back( new ArgumentRule( "outgroup", RlString::getClassTypeSpec(), "",                                ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        argumentRules.push_back( new ArgumentRule( "data"    , AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_PattersonsD::getClassType(void)
{
    
    static std::string revType = "Func_PattersonsD";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_PattersonsD::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedFunction<Real>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/** Get the help entry for this class */
RevBayesCore::RbHelpFunction* Func_PattersonsD::getHelpEntry( void ) const
{
    
    // create the help function entry that we will fill with some values
    RevBayesCore::RbHelpFunction *help = new RevBayesCore::RbHelpFunction();
//    RevBayesCore::RbHelpFunction &helpEntry = *help;
//    
//    std::string entry = "";
//    
//    // name
//    helpEntry.setName( "name" );
//    
//    // aliases
//    std::vector<std::string> aliases = std::vector<std::string>();
//    //    aliases.push_back( "alias" );
//    helpEntry.setAliases( aliases );
//    
//    // title
//    helpEntry.setTitle( "title" );
//    
//    // description
//    std::vector<std::string> desc = std::vector<std::string>();
//    desc.push_back("");
//    helpEntry.setDescription( desc );
//    
//    // usage
//    helpEntry.setUsage( "usage" );
//    
//    // arguments
//    std::vector<RevBayesCore::RbHelpArgument> arguments = std::vector<RevBayesCore::RbHelpArgument>();
//    RevBayesCore::RbHelpArgument argument = RevBayesCore::RbHelpArgument();
//    argument.setLabel(                  "label"                 );
//    argument.setDescription(            "description"           );
//    argument.setArgumentDagNodeType(    "argumentNodeType"      );
//    argument.setArgumentPassingMethod(  "argumentPassingMethod" );
//    argument.setValueType(              "valueType"             );
//    argument.setDefaultValue(           "defaultValue"          );
//    
//    // loop options
//    std::vector<std::string> options = std::vector<std::string>();
//    std::string option = std::string( "o" );
//    options.push_back( option );
//    argument.setOptions( options );
//    
//    // add the argument to the argument list
//    arguments.push_back( argument );
//    
//    helpEntry.setArguments( arguments );
//    
//    // return value
//    helpEntry.setReturnType( "returnValue" );
//    
//    // details
//    std::vector<std::string> details = std::vector<std::string>();
//    details.push_back( "details" );
//    
//    helpEntry.setDetails( details );
//    
//    // example
//    helpEntry.setExample( "example" );
//    
//    // reference
//    std::vector<RevBayesCore::RbHelpReference> references = std::vector<RevBayesCore::RbHelpReference>();
//    RevBayesCore::RbHelpReference ref = RevBayesCore::RbHelpReference();
//    ref.setCitation( "citation" );
//    ref.setDoi( "doi" );
//    ref.setUrl( "url" );
//    references.push_back( ref );
//    
//    helpEntry.setReferences( references );
//    
//    // author
//    helpEntry.setAuthor( "author" );
//    
//    // see also
//    std::vector<std::string> seeAlso = std::vector<std::string>();
//    seeAlso.push_back( "see also" );
//    
//    helpEntry.setSeeAlso(seeAlso);
    
    return help;
    
}


const TypeSpec& Func_PattersonsD::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}