/* 
 * File:   HelpParser.h
 * Author: johadunf
 *
 * Created on May 7, 2014, 3:06 PM
 */

#ifndef HELPPARSER_H
#define	HELPPARSER_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "pugixml.hpp"
#include "RbHelpEntry.h"
#include "IHelpRenderer.h"

#include "boost/filesystem.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

#include <boost/format.hpp>
#include <boost/foreach.hpp>


namespace fs = boost::filesystem;

const std::string sectionBreak = "\n\n";
const std::string lineBreak = "\n";
int w = 80;

class HelpParser : public IHelp {
public:

    /**
     * Load help from a specific file
     * @param helpFile
     */
    bool setHelp(std::string helpFile) {
        this->message = "";

        pugi::xml_parse_result result = doc.load_file(helpFile.c_str(), pugi::parse_default);
        if (result.status != pugi::status_ok) {
            this->message.append(result.description()).append("\n");
            return false;
        }

        mapHelpEntity();
        return true;
    }

    /**
     * Query help for a specific command
     * @param query
     * @param columnWidth
     * @return 
     */
    std::string getHelp(std::string query, int columnWidth) {
        w = columnWidth;
        this->message = "";

        if (!isHelpAvailableForQuery(query)) {
            std::string s = "Help is not available for " + query + "\n";
            this->message.append(s);
            return s;
        }

        // we should have a entity loaded from the xml by now.
        RbHelpEntry help = IHelp::getRbHelpEntry();
        FunctionHelpEntry fh = help.GetFunctionHelpEntry();
        TypeHelpEntry th = help.GetTypeHelpEntry();

        if (fh.GetName() == query) {
            return renderFunctionHelp(fh);
        }

        return renderTypeHelp(th);
    }

    /**
     * Check if help can be found form a command
     * @param query
     * @return 
     */
    bool isHelpAvailableForQuery(std::string query) {
        pugi::xml_parse_result result = this->loadDocument(query);
        if (result.status == pugi::status_ok) {
            mapHelpEntity();
            RbHelpEntry help = getRbHelpEntry();
            // case sensitive compare of function/type name with the query
            return (help.GetTypeHelpEntry().GetName() == query) || (help.GetFunctionHelpEntry().GetName() == query);
        }
        return false;
    }

    void setHelpDir(std::string helpDir) {
        this->helpDir = helpDir;
    }

    std::string getHelpDir() const {
        return helpDir;
    }

    void setMessage(std::string message) {
        this->message = message;
    }

    std::string getMessage() const {
        return message;
    }

    void setRenderer(IHelpRenderer *renderer) {
        this->renderer = renderer;
    }

    std::string renderTypeHelp(TypeHelpEntry th) {
        return renderer->renderTypeHelp(th);
    }

    std::string renderFunctionHelp(FunctionHelpEntry fh) {
        return renderer->renderFunctionHelp(fh);
    }


private:
    IHelpRenderer *renderer;
    std::string message;
    std::string helpDir;

    pugi::xml_document doc;

    /**
     * Load a xml document. Result can be checked for status.
     * @param qs
     * @return 
     */
    pugi::xml_parse_result loadDocument(std::string qs) {

        // Try to find corresponding help file,
        // the help file should be all lowercase if to be found
        std::string command = qs;
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);
        std::string xmlFile = command + ".xml";
        fs::path helpPath(this->helpDir);
        std::string helpfile = fs::path(helpPath / xmlFile).normalize().string();

        // try to load the xml file
        pugi::xml_parse_result result = doc.load_file(helpfile.c_str(), pugi::parse_default);
        if (result.status != pugi::status_ok) {
            this->message.append(result.description()).append("\n");
        }
        return result;
    }

    /**
     * Maps the xml data to corresponding POCO
     */
    void mapHelpEntity() {

        FunctionHelpEntry *fh = mapFunctionHelp();
        TypeHelpEntry *th = mapTypeHelp();

        RbHelpEntry *help = new RbHelpEntry;
        help->SetFunctionHelpEntry(*fh);
        help->SetTypeHelpEntry(*th);

        IHelp::setRbHelpEntry(*help);
    }

    /**
     * Map a Function help entry entity object
     * @return 
     */
    FunctionHelpEntry *mapFunctionHelp() {
        pugi::xpath_node_set nodeSet, subSet;
        FunctionHelpEntry *entry = new FunctionHelpEntry();

        // name        
        entry->SetName(doc.select_single_node("//function-help-entry/name").node().child_value());
        // title
        entry->SetTitle(doc.select_single_node("//function-help-entry/title").node().child_value());
        // description
        std::vector<std::string> *desc = new std::vector<std::string>();
        nodeSet = doc.select_nodes("//function-help-entry/description/p");
        for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
            pugi::xpath_node node = *it;
            desc->push_back(node.node().child_value());
        }
        entry->SetDescription(*desc);
        // usage
        entry->SetUsage(doc.select_single_node("//function-help-entry/usage").node().child_value());
        // arguments
        nodeSet = doc.select_nodes("//function-help-entry/argument");
        int loop = 0;
        std::vector<ArgumentHelpEntry> *arguments = new std::vector<ArgumentHelpEntry>();
        for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
            pugi::xpath_node node = *it;
            ArgumentHelpEntry *argument = new ArgumentHelpEntry();
            argument->SetLabel(node.node().child_value("label"));
            argument->SetDescription(node.node().child_value("description"));
            argument->SetArgumentType(node.node().child_value("argumentType"));
            argument->SetValueType(node.node().child_value("valueType"));
            argument->SetDefaultValue(node.node().child_value("defaultValue"));

            // loop options
            std::vector<std::string> *options = new std::vector<std::string>();
            std::string s = "//function-help-entry/argument[" + to_string(loop) + "]/option"; // xpath search expression
            subSet = node.node().select_nodes(s.c_str());
            for (pugi::xpath_node_set::const_iterator it = subSet.begin(); it != subSet.end(); ++it) {
                pugi::xpath_node subNode = *it;
                std::string *option = new std::string(subNode.node().child_value());
                options->push_back(*option);
            }
            argument->SetOptions(*options);
            arguments->push_back(*argument);
            loop++;
        }
        entry->SetArguments(*arguments);

        // return value
        entry->SetReturnValue(doc.select_single_node("//function-help-entry/returnValue").node().child_value());
        // details
        std::vector<std::string> *details = new std::vector<std::string>();
        nodeSet = doc.select_nodes("//function-help-entry/details/p");
        for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
            pugi::xpath_node node = *it;
            details->push_back(node.node().child_value());
        }
        entry->SetDetails(*details);
        // example
        entry->SetExample(doc.select_single_node("//function-help-entry/example").node().child_value());
        // reference
        std::vector<ReferenceHelpEntry> *references = new std::vector<ReferenceHelpEntry>();
        nodeSet = doc.select_nodes("//function-help-entry/reference");
        for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
            pugi::xpath_node node = *it;
            ReferenceHelpEntry *ref = new ReferenceHelpEntry();

            ref->SetCitation(node.node().child_value("citation"));
            ref->SetDoi(node.node().child_value("doi"));
            ref->SetUrl(node.node().child_value("url"));
            references->push_back(*ref);
        }
        entry->SetReferences(*references);
        // author
        entry->SetAuthor(doc.select_single_node("//function-help-entry/author").node().child_value());
        // see also
        std::vector<std::string> *seeAlso = new std::vector<std::string>();
        nodeSet = doc.select_nodes("//function-help-entry/seeAlso/entry");
        for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
            pugi::xpath_node node = *it;
            seeAlso->push_back(node.node().child_value());
        }
        entry->SetSeeAlso(*seeAlso);

        return entry;
    }

    /**
     * Map a Type help entry entity
     * @return 
     */
    TypeHelpEntry *mapTypeHelp() {
        pugi::xpath_node_set nodeSet, subSet;
        TypeHelpEntry *entry = new TypeHelpEntry();

        // name        
        entry->SetName(doc.select_single_node("//type-help-entry/name").node().child_value());
        // title
        entry->SetTitle(doc.select_single_node("//type-help-entry/title").node().child_value());
        // description
        std::vector<std::string> *desc = new std::vector<std::string>();
        nodeSet = doc.select_nodes("//type-help-entry/description/p");
        for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
            pugi::xpath_node node = *it;
            desc->push_back(node.node().child_value());
        }
        entry->SetDescription(*desc);

        // member methods
        nodeSet = doc.select_nodes("//type-help-entry/memberMethod");
        int loop = 0;
        std::vector<MethodMemberHelpEntry> *methodMembers = new std::vector<MethodMemberHelpEntry>();
        for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
            pugi::xpath_node node = *it;

            MethodMemberHelpEntry *methodMember = new MethodMemberHelpEntry();
            methodMember->SetMethodName(node.node().child_value("methodName"));
            methodMember->SetDescription(node.node().child_value("description"));
            methodMember->SetUsage(node.node().child_value("usage"));
            methodMember->SetMethodType(node.node().child_value("methodType"));
            methodMember->SetReturnValue(node.node().child_value("returnValue"));

            std::vector<ArgumentHelpEntry> *arguments = new std::vector<ArgumentHelpEntry>();
            std::string s = "//type-help-entry/memberMethod[" + to_string(loop) + "]/argument";
            subSet = node.node().select_nodes(s.c_str());
            for (pugi::xpath_node_set::const_iterator it = subSet.begin(); it != subSet.end(); ++it) {
                pugi::xpath_node subNode = *it;
                ArgumentHelpEntry *argument = new ArgumentHelpEntry();
                argument->SetLabel(subNode.node().child_value("label"));
                argument->SetDescription(subNode.node().child_value("description"));
                argument->SetArgumentType(subNode.node().child_value("argumentType"));
                argument->SetValueType(subNode.node().child_value("valueType"));
                //argument->SetDefaultValue(subNode.node().child_value("defaultValue"));
                arguments->push_back(*argument);
            }

            methodMembers->push_back(*methodMember);
            loop++;
        }
        entry->SetMethodMembers(*methodMembers);

        // details
        std::vector<std::string> *details = new std::vector<std::string>();
        nodeSet = doc.select_nodes("//type-help-entry/details/p");
        for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
            pugi::xpath_node node = *it;
            details->push_back(node.node().child_value());
        }
        entry->SetDetails(*details);

        // author
        entry->SetAuthor(doc.select_single_node("//type-help-entry/author").node().child_value());
        // citation
        std::vector<std::string> *citation = new std::vector<std::string>();
        nodeSet = doc.select_nodes("//type-help-entry/citation");
        for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
            pugi::xpath_node node = *it;
            citation->push_back(node.node().child_value());
        }
        entry->SetCitation(*citation);

        return entry;
    }

    /**
     * Generic to_string function
     * @param value
     * @return 
     */
    template <typename T>
    std::string to_string(T value) {
        std::ostringstream os;
        os << value;
        return os.str();
    }
};

#endif	/* HELPPARSER_H */
