#include "RbException.h"
#include "RbFileManager.h"
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
    
    RbHelpFunction helpEntry = parseInternalHelpFunction(doc, "//function-help-entry/");
    
    return helpEntry;
}


RbHelpFunction RbHelpParser::parseInternalHelpFunction(const pugi::xml_document &doc, const std::string &prefix)
{
    // create the help function entry that we will fill with some values later on
    RbHelpFunction helpEntry = RbHelpFunction();
    
    pugi::xpath_node_set nodeSet, subSet;
    
    std::string entry = "";
    
    // name
    entry = prefix + "name";
    helpEntry.setName( doc.select_single_node( entry.c_str() ).node().child_value() );
    
    // aliases
    entry = prefix + "alias";
    nodeSet = doc.select_nodes( entry.c_str() );
    std::vector<std::string> aliases = std::vector<std::string>();
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node subNode = *it;
        std::string alias = std::string(subNode.node().child_value());
        aliases.push_back( alias );
    }
    helpEntry.setAliases( aliases );
    
    // title
    entry = prefix + "title";
    helpEntry.setTitle( doc.select_single_node( entry.c_str() ).node().child_value() );
    
    // description
    std::vector<std::string> desc = std::vector<std::string>();
    entry = prefix + "description/p";
    nodeSet = doc.select_nodes( entry.c_str() );
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node node = *it;
        desc.push_back(node.node().child_value());
    }
    helpEntry.setDescription( desc );
    
    // usage
    entry = prefix + "usage";
    helpEntry.setUsage( doc.select_single_node( entry.c_str() ).node().child_value() );
    
    // arguments
    entry = prefix + "argument";
    nodeSet = doc.select_nodes( entry.c_str() );
    int loop = 1;
    std::vector<RbHelpArgument> arguments = std::vector<RbHelpArgument>();
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node node = *it;
        RbHelpArgument argument = RbHelpArgument();
        argument.setLabel(                  node.node().child_value("label")                    );
        argument.setDescription(            node.node().child_value("description")              );
        argument.setArgumentDagNodeType(    node.node().child_value("argumentNodeType")         );
        argument.setArgumentPassingMethod(  node.node().child_value("argumentPassingMethod")    );
        argument.setValueType(              node.node().child_value("valueType")                );
        argument.setDefaultValue(           node.node().child_value("defaultValue")             );
        
        // loop options
        std::vector<std::string> options = std::vector<std::string>();
        std::string s = prefix + "argument[" + StringUtilities::to_string(loop) + "]/option"; // xpath search expression
        subSet = node.node().select_nodes(s.c_str());
        for (pugi::xpath_node_set::const_iterator it = subSet.begin(); it != subSet.end(); ++it)
        {
            pugi::xpath_node subNode = *it;
            std::string option = std::string(subNode.node().child_value());
            options.push_back( option );
        }
        argument.setOptions( options );
        
        // add the argument to the argument list
        arguments.push_back( argument );
        
        loop++;
    }
    helpEntry.setArguments( arguments );
    
    // return value
    entry = prefix + "returnValue";
    helpEntry.setReturnValue(doc.select_single_node( entry.c_str() ).node().child_value());
    
    // details
    std::vector<std::string> details = std::vector<std::string>();
    entry = prefix + "details/p";
    nodeSet = doc.select_nodes( entry.c_str() );
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
        pugi::xpath_node node = *it;
        details.push_back(node.node().child_value());
    }
    helpEntry.setDetails( details );
    
    // example
    entry = prefix + "example";
    helpEntry.setExample(doc.select_single_node( entry.c_str() ).node().child_value());
    
    // reference
    std::vector<RbHelpReference> references = std::vector<RbHelpReference>();
    entry = prefix + "reference";
    nodeSet = doc.select_nodes( entry.c_str() );
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node node = *it;
        RbHelpReference ref = RbHelpReference();
        
        ref.setCitation(node.node().child_value("citation"));
        ref.setDoi(node.node().child_value("doi"));
        ref.setUrl(node.node().child_value("url"));
        references.push_back(ref);
    }
    helpEntry.setReferences( references );
    
    // author
    entry = prefix + "author";
    helpEntry.setAuthor(doc.select_single_node( entry.c_str() ).node().child_value());
    
    // see also
    std::vector<std::string> seeAlso = std::vector<std::string>();
    entry = prefix + "seeAlso/entry";
    nodeSet = doc.select_nodes( entry.c_str() );
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node node = *it;
        seeAlso.push_back(node.node().child_value());
    }
    helpEntry.setSeeAlso(seeAlso);
    
    
    
    return helpEntry;
}




RbHelpType RbHelpParser::parseHelpType(const std::string &fn)
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
    
    RbHelpType helpEntry = RbHelpType();
    
    std::vector<RbHelpFunction> constructors;
    RbHelpFunction helpEntryConstructor = parseInternalHelpFunction(doc, "//type-help-entry/constructor-help-entry/");
    constructors.push_back( helpEntryConstructor );
    
    helpEntry.setConstructors( constructors );
    
    std::vector<RbHelpFunction> methods;
    RbHelpFunction helpEntryMethod = parseInternalHelpFunction(doc, "//type-help-entry/method-help-entry/");
    methods.push_back( helpEntryMethod );
    
    helpEntry.setMethods( methods );
    
    // now return the help entry
    return helpEntry;
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
    
    
    throw RbException("Unknown help entry type in file '" + fn + "'.");

}

