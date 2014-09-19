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
    // create the help function entry that we will fill with some values later on
    RbHelpFunction helpEntry = RbHelpFunction();
    
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

    
    pugi::xpath_node_set nodeSet, subSet;
    
    // name
    helpEntry.setName( doc.select_single_node("//function-help-entry/name").node().child_value() );
    
    // aliases
    nodeSet = doc.select_nodes("//function-help-entry/alias");
    std::vector<std::string> aliases = std::vector<std::string>();
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node subNode = *it;
        std::string alias = std::string(subNode.node().child_value());
        aliases.push_back( alias );
    }
    helpEntry.setAliases( aliases );
    
    // title
    helpEntry.setTitle( doc.select_single_node("//function-help-entry/title").node().child_value() );
    
    // description
    std::vector<std::string> desc = std::vector<std::string>();
    nodeSet = doc.select_nodes("//function-help-entry/description/p");
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
        pugi::xpath_node node = *it;
        desc.push_back(node.node().child_value());
    }
    helpEntry.setDescription( desc );
    
    // usage
    helpEntry.setUsage( doc.select_single_node("//function-help-entry/usage").node().child_value() );
    
    // arguments
    nodeSet = doc.select_nodes("//function-help-entry/argument");
    int loop = 1;
    std::vector<RbHelpArgument> arguments = std::vector<RbHelpArgument>();
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node node = *it;
        RbHelpArgument argument = RbHelpArgument();
        argument.setLabel(         node.node().child_value("label")         );
        argument.setDescription(   node.node().child_value("description")   );
        argument.setArgumentType(  node.node().child_value("argumentType")  );
        argument.setValueType(     node.node().child_value("valueType")     );
        argument.setDefaultValue(  node.node().child_value("defaultValue")  );
        
        // loop options
        std::vector<std::string> options = std::vector<std::string>();
        std::string s = "//function-help-entry/argument[" + StringUtilities::to_string(loop) + "]/option"; // xpath search expression
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
    helpEntry.setReturnValue(doc.select_single_node("//function-help-entry/returnValue").node().child_value());
    
    // details
    std::vector<std::string> details = std::vector<std::string>();
    nodeSet = doc.select_nodes("//function-help-entry/details/p");
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
        pugi::xpath_node node = *it;
        details.push_back(node.node().child_value());
    }
    helpEntry.setDetails( details );
    
    // example
    helpEntry.setExample(doc.select_single_node("//function-help-entry/example").node().child_value());
    
    // reference
    std::vector<RbHelpReference> references = std::vector<RbHelpReference>();
    nodeSet = doc.select_nodes("//function-help-entry/reference");
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
        pugi::xpath_node node = *it;
        RbHelpReference ref = RbHelpReference();
        
        ref.setCitation(node.node().child_value("citation"));
        ref.setDoi(node.node().child_value("doi"));
        ref.setUrl(node.node().child_value("url"));
        references.push_back(ref);
    }
    helpEntry.setReferences( references );
    
    // author
    helpEntry.setAuthor(doc.select_single_node("//function-help-entry/author").node().child_value());
    
    // see also
    std::vector<std::string> seeAlso = std::vector<std::string>();
    nodeSet = doc.select_nodes("//function-help-entry/seeAlso/entry");
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
        pugi::xpath_node node = *it;
        seeAlso.push_back(node.node().child_value());
    }
    helpEntry.setSeeAlso(seeAlso);

    
    
    return helpEntry;
}

