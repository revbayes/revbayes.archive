#include "RbException.h"
#include "RbFileManager.h"
#include "RbHelpDistribution.h"
#include "RbHelpMonitor.h"
#include "RbHelpMove.h"
#include "RbHelpParser.h"
#include "StringUtilities.h"
#include "XmlDocument.h"
#include "XmlParser.h"

#include "pugixml.hpp"


using namespace RevBayesCore;

RbHelpFunction RbHelpParser::parseHelpFunction(const std::string &fn)
{
    
    // first we need to load the file
    std::ifstream readStream;
    RbFileManager fm = RbFileManager(fn);
    fm.openFile( readStream );
    
    // try to load the xml file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fn.c_str(), pugi::parse_default);
    if (result.status != pugi::status_ok)
    {
    std::cerr << "Problem while parsing file " << fn << std::endl;
        throw RbException( result.description() );
    }
    
    std::vector<RbHelpFunction> functions;
//    pugi::xpath_node node = doc.select_single_node( "//function-help-entry" );
    pugi::xpath_node_set nodeSet = doc.select_nodes( "//function-help-entry" );
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node node = *it;
        RbHelpFunction helpEntryFunction = parseInternalHelpFunction( node );
        functions.push_back( helpEntryFunction );
    }

    RbHelpFunction helpEntry = functions[0];
    
    return helpEntry;
}


RbHelpFunction RbHelpParser::parseInternalHelpFunction(const pugi::xpath_node &node)
{
    // create the help function entry that we will fill with some values later on
    RbHelpFunction helpEntry = RbHelpFunction();
    
    pugi::xpath_node_set nodeSet, subSet;
    
    std::string entry = "";
    
    // name
    helpEntry.setName( node.node().select_single_node( "name" ).node().child_value() );
    
    // aliases
    nodeSet = node.node().select_nodes( "alias" );
    std::vector<std::string> aliases = std::vector<std::string>();
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node subNode = *it;
        std::string alias = std::string(subNode.node().child_value());
        aliases.push_back( alias );
    }
    helpEntry.setAliases( aliases );
    
    // title
    helpEntry.setTitle( node.node().select_single_node( "title" ).node().child_value() );
    
    // description
    std::vector<std::string> desc = std::vector<std::string>();
    nodeSet = node.node().select_nodes( "description/p" );
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node subnode = *it;
        desc.push_back(subnode.node().child_value());
    }
    helpEntry.setDescription( desc );
    
    // usage
    helpEntry.setUsage( node.node().select_single_node( "usage" ).node().child_value() );
    
    // arguments
    nodeSet = node.node().select_nodes( "argument" );
    int loop = 1;
    std::vector<RbHelpArgument> arguments = std::vector<RbHelpArgument>();
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node subnode = *it;
        RbHelpArgument argument = RbHelpArgument();
        argument.setLabel(                  subnode.node().child_value("label")                    );
        argument.setDescription(            subnode.node().child_value("description")              );
        argument.setArgumentDagNodeType(    subnode.node().child_value("argumentNodeType")         );
        argument.setArgumentPassingMethod(  subnode.node().child_value("argumentPassingMethod")    );
        argument.setValueType(              subnode.node().child_value("valueType")                );
        argument.setDefaultValue(           subnode.node().child_value("defaultValue")             );
        
        // loop options
        std::vector<std::string> options = std::vector<std::string>();
        std::string s =  "argument[" + StringUtilities::to_string(loop) + "]/option"; // xpath search expression
        subSet = subnode.node().select_nodes( s.c_str() );
        for (pugi::xpath_node_set::const_iterator it = subSet.begin(); it != subSet.end(); ++it)
        {
            pugi::xpath_node subsubNode = *it;
            std::string option = std::string( subsubNode.node().child_value() );
            options.push_back( option );
        }
        argument.setOptions( options );
        
        // add the argument to the argument list
        arguments.push_back( argument );
        
        loop++;
    }
    helpEntry.setArguments( arguments );
    
    // return value
    helpEntry.setReturnType( node.node().select_single_node( "returnValue" ).node().child_value());
    
    // details
    std::vector<std::string> details = std::vector<std::string>();
    nodeSet = node.node().select_nodes( "details/p" );
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
        pugi::xpath_node subnode = *it;
        details.push_back( subnode.node().child_value());
    }
    helpEntry.setDetails( details );
    
    // example
    helpEntry.setExample( node.node().select_single_node( "example" ).node().child_value());
    
    // reference
    std::vector<RbHelpReference> references = std::vector<RbHelpReference>();
    nodeSet = node.node().select_nodes( "reference" );
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node subnode = *it;
        RbHelpReference ref = RbHelpReference();
        
        ref.setCitation( subnode.node().child_value("citation") );
        ref.setDoi( subnode.node().child_value("doi") );
        ref.setUrl( subnode.node().child_value("url") );
        references.push_back( ref );
    }
    helpEntry.setReferences( references );
    
    // author
    helpEntry.setAuthor( node.node().select_single_node( "author" ).node().child_value());
    
    // see also
    std::vector<std::string> seeAlso = std::vector<std::string>();
    nodeSet = node.node().select_nodes( "seeAlso/entry" );
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node subnode = *it;
        seeAlso.push_back( subnode.node().child_value());
    }
    helpEntry.setSeeAlso(seeAlso);
    
    
    
    return helpEntry;
}



RbHelpType* RbHelpParser::parseHelpDistribution(const std::string &fn)
{
    
    // first we need to load the file
    std::ifstream readStream;
    RbFileManager fm = RbFileManager(fn);
    fm.openFile( readStream );
    
    // try to load the xml file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fn.c_str(), pugi::parse_default);
    if (result.status != pugi::status_ok)
    {
        std::cerr << "Problem while parsing file " << fn << std::endl;
        throw RbException( result.description() );
    }
    
    RbHelpDistribution* helpEntry = new RbHelpDistribution();
    
    pugi::xpath_node node = doc.select_single_node( "//distribution-help-entry" );
    parseInternalHelpType( node, helpEntry );
    
    // now return the help entry
    return helpEntry;
}



RbHelpType* RbHelpParser::parseHelpMonitor(const std::string &fn)
{
    
    // first we need to load the file
    std::ifstream readStream;
    RbFileManager fm = RbFileManager(fn);
    fm.openFile( readStream );
    
    // try to load the xml file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fn.c_str(), pugi::parse_default);
    if (result.status != pugi::status_ok)
    {
        std::cerr << "Problem while parsing file " << fn << std::endl;
        throw RbException( result.description() );
    }
    
    RbHelpMonitor* helpEntry = new RbHelpMonitor();
    
    pugi::xpath_node node = doc.select_single_node( "//monitor-help-entry" );
    parseInternalHelpType( node, helpEntry );
    
    // now return the help entry
    return helpEntry;
}


RbHelpType* RbHelpParser::parseHelpMove(const std::string &fn)
{
    
    // first we need to load the file
    std::ifstream readStream;
    RbFileManager fm = RbFileManager(fn);
    fm.openFile( readStream );
    
    // try to load the xml file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fn.c_str(), pugi::parse_default);
    if (result.status != pugi::status_ok)
    {
        std::cerr << "Problem while parsing file " << fn << std::endl;
        throw RbException( result.description() );
    }
    
    RbHelpMove* helpEntry = new RbHelpMove();
    
    pugi::xpath_node node = doc.select_single_node( "//move-help-entry" );
    parseInternalHelpType( node, helpEntry );
    
    // now return the help entry
    return helpEntry;
}


RbHelpType* RbHelpParser::parseHelpType(const std::string &fn)
{
    
    // first we need to load the file
    std::ifstream readStream;
    RbFileManager fm = RbFileManager(fn);
    fm.openFile( readStream );
    
    // try to load the xml file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fn.c_str(), pugi::parse_default);
    if (result.status != pugi::status_ok)
    {
        std::cerr << "Problem while parsing file " << fn << std::endl;
        throw RbException( result.description() );
    }
    
    RbHelpType* helpEntry = new RbHelpType();
    
    pugi::xpath_node node = doc.select_single_node( "//type-help-entry" );
    parseInternalHelpType( node, helpEntry );
    
    // now return the help entry
    return helpEntry;
}

void RbHelpParser::parseInternalHelpType(const pugi::xpath_node &node, RbHelpType *helpEntry)
{
    
    // name
    helpEntry->setName( node.node().select_single_node( "name" ).node().child_value() );
    
    // title
    helpEntry->setTitle( node.node().select_single_node( "title" ).node().child_value() );
    
    // aliases
    pugi::xpath_node_set nodeSet = node.node().select_nodes( "alias" );
    std::vector<std::string> aliases = std::vector<std::string>();
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node subNode = *it;
        std::string alias = std::string(subNode.node().child_value());
        aliases.push_back( alias );
    }
    helpEntry->setAliases( aliases );
    
    // description
    std::vector<std::string> desc = std::vector<std::string>();
    nodeSet = node.node().select_nodes( "description/p" );
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node subnode = *it;
        desc.push_back(subnode.node().child_value());
    }
    helpEntry->setDescription( desc );
    
    
    std::vector<RbHelpFunction> constructors;
    nodeSet = node.node().select_nodes( "constructor-help-entry" );
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node node = *it;
        RbHelpFunction helpEntryConstructor = parseInternalHelpFunction( node );
        constructors.push_back( helpEntryConstructor );
    }
    // set the constructors
    helpEntry->setConstructors( constructors );
    

    std::vector<RbHelpFunction> methods;
    nodeSet = node.node().select_nodes( "method-help-entry" );
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node node = *it;
        RbHelpFunction helpEntryMethod = parseInternalHelpFunction( node );
        methods.push_back( helpEntryMethod );
    }
    // set the methods
    helpEntry->setMethods( methods );
        
}


RbHelpParser::HelpEntryType RbHelpParser::testHelpEntry(const std::string &fn)
{
    // first we need to load the file
    std::ifstream readStream;
    RbFileManager fm = RbFileManager(fn);
    fm.openFile( readStream );
    
    // try to load the xml file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fn.c_str(), pugi::parse_default);
    if (result.status != pugi::status_ok)
    {
        std::cerr << "Problem while parsing file " << fn << std::endl;
        throw RbException( result.description() );
    }
    
    pugi::xpath_node_set nodeSet = doc.select_nodes("//function-help-entry");
    if ( nodeSet.size() > 0 )
    {
        return FUNCTION;
    }
    
    nodeSet = doc.select_nodes("//type-help-entry");
    if ( nodeSet.size() > 0 )
    {
        return TYPE;
    }
    
    nodeSet = doc.select_nodes("//distribution-help-entry");
    if ( nodeSet.size() > 0 )
    {
        return DISTRIBUTION;
    }
    
    nodeSet = doc.select_nodes("//move-help-entry");
    if ( nodeSet.size() > 0 )
    {
        return MOVE;
    }
    
    nodeSet = doc.select_nodes("//monitor-help-entry");
    if ( nodeSet.size() > 0 )
    {
        return MONITOR;
    }
    
    
    throw RbException("Unknown help entry type in file '" + fn + "'.");

}

