/**
 * Another doxygen test
 *
 * @file
 * This file contains the implementation of Help, a singleton 
 * class that manages help for the user.
 *
 * @brief 
 * The help files which are displayed by this module consists of xml files 
 * located in <repos_root>/help directory. The name of these files should each
 * correspond to the command to query for help, for example:
 * if the user want help for mcmc function and types ?mcmc in terminal, a file 
 * called mcmc.xml is looked for in the help directory. Note that the file name 
 * is expected to be alla lowercase, the command queried for help is always 
 * converted to lowercase so requesting help for mcmc, McMc both matches the 
 * mcmc.xml file if present.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "Help.h"
#include "RbFileManager.h"
#include "RbUtil.h"
#include "StringUtilities.h"
#include "RlUserInterface.h"
#include <iostream>
#include <sstream>
#include "pugixml.hpp"
#include <algorithm>
#include <string> 
#include <vector>
#include <iterator>
#include <list>


/** This is what turn up in terminal window */
std::string Help::getHelp(std::string qs, int columnWidth)
{

    this->loadHelpFile(qs);
    
    pugi::xpath_node_set nodeSet, subSet;

    // the output to display in terminal
    std::string help = "\n";

    // name
    help += formatOutString(doc.child("help_entry").child("name").child_value(), columnWidth, 0, 2);

    // title
    help += formatOutString(doc.child("help_entry").child("title").child_value(), columnWidth, 0, 2);

    // description
    nodeSet = doc.select_nodes("/help_entry/description/p");
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node node = *it;
        help += formatOutString(node.node().child_value(), columnWidth, 0, 1);
    }
    help += "\n\n";

    // usage
    help += formatOutString("Usage:", columnWidth, 0, 1);
    help += formatOutString(doc.child("help_entry").child("usage").child_value(), columnWidth, 1, 1);
    help += "\n";

    // details
    nodeSet = doc.select_nodes("/help_entry/details/p");
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node node = *it;
        help += formatOutString(node.node().child_value(), columnWidth, 0, 1);
    }
    help += "\n\n";

    // arguments
    help += formatOutString("Arguments:", columnWidth, 0, 1);
    nodeSet = doc.select_nodes("/help_entry/argument");
    int loop = 0;
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node node = *it;

        help += formatOutString(node.node().child_value("label"), columnWidth, 1, 1);
        help += formatOutString(node.node().child_value("description"), columnWidth, 2, 1);

        help += formatOutString("Type: ", columnWidth, 2, -1);
        help += formatOutString(node.node().child_value("type"), columnWidth, -1, 1);

        help += formatOutString("Value type: ", columnWidth, 2, 0);
        help += formatOutString(node.node().child_value("valueType"), columnWidth, -1, 1);

        // loop through the "options" within this "argument" clause
        std::string s = "/help_entry/argument[" + to_string(loop) + "]/option"; // xpath search expression
        subSet = node.node().select_nodes(s.c_str());
        if (subSet.size() > 0)
        {
            help += formatOutString("Options: ", columnWidth, 2, 1);
        }
        for (pugi::xpath_node_set::const_iterator it = subSet.begin(); it != subSet.end(); ++it)
        {
            pugi::xpath_node subNode = *it;
            help += formatOutString(subNode.node().child_value(), columnWidth, 3, 1);
        }

        if (!node.node().child("defaultValue").empty())
        {
            help += formatOutString("Default Value: ", columnWidth, 2, -1);
            help += formatOutString(node.node().child_value("defaultValue"), columnWidth, -1, 1);
        }

        help += "\n";
        loop++;

    }
    help += "\n\n";

    // return value
    help += formatOutString("Return Value:", columnWidth, 0, 2);
    help += formatOutString(doc.child("help_entry").child("value").child_value(), columnWidth, 1, 3);

    // author
    help += formatOutString("Author:", columnWidth, 0, 2);
    help += formatOutString(doc.child("help_entry").child("author").child_value(), columnWidth, 1, 3);

    // reference   
    help += formatOutString("References:", columnWidth, 0, 2);
    nodeSet = doc.select_nodes("/help_entry/reference");
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node node = *it;

        help += formatOutString(node.node().child_value("info"), columnWidth, 1, 1);
        help += formatOutString(node.node().child_value("citation"), columnWidth, 2, 2);
        help += formatOutString(node.node().child_value("url"), columnWidth, 2, 2);
        if (!node.node().child("doi").empty())
        {
            help += formatOutString("Doi: ", columnWidth, 2, 0);
            help += formatOutString(node.node().child_value("doi"), columnWidth, -1, 2);
        }
    }
    help += "\n";

    // see also
    help += formatOutString("See Also:", columnWidth, 0, 2);
    //help += formatOutString("", columnWidth, 1, 0);
    nodeSet = doc.select_nodes("/help_entry/seeAlso/function");
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node node = *it;
        help += formatOutString(node.node().child_value(), columnWidth, 1, 0);
        if(it+1 != nodeSet.end()){
            help += ", ";
        }
        


    }
//    if (!nodeSet.empty())
//    {
//        help = help.substr(0, -3);
//    }
    help += "\n\n";
    
    // examples
    // preserve line breaks from xml for examples clause
    help += formatOutString("Examples:", columnWidth, 0, 2);
    nodeSet = doc.select_nodes("/help_entry/example");
    for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it)
    {
        pugi::xpath_node node = *it;
        help += formatOutString(node.node().child_value(), columnWidth, 1, 2, false);
    }
    help += "\n";

    return help;
}

/** format a string to output*/
std::string Help::formatOutString(std::string s, size_t columnWidth, int indentLevel, int numLineBreaks)
{
    return formatOutString(s, columnWidth, indentLevel, numLineBreaks, true);
}

/** format a string to output*/
std::string Help::formatOutString(std::string s, size_t columnWidth, int indentLevel, int numLineBreaks, bool stripLineBreaks)
{
    //std::cout << s + "<break>\n";
    std::string indent = "";
    for (int i = -1; i < indentLevel; i++)
    {
        indent += RevBayesCore::RbUtils::PAD;
    }

    // -- remove any pre-formatting that may come from the xml-file
    s = replaceString(s, "\t", " ");
    if (stripLineBreaks)
    {
        s = replaceString(s, "\n", " ");
    }

    s = stripConsecutiveSpace(s);

    // do nothing if string is empty
    if (s.length() <= 0)
    {
        return "";
    }

    // remove space at beginning
    if (std::isspace(s.at(0)))
    {
        s.erase(0, 1);
    }

    //-- apply the formatting style we want
    // wrap text
    s = wrapText(s, indent, columnWidth);
    // apply line breaks at end
    for (int i = 0; i < numLineBreaks; i++)
    {
        s += "\n";
    }

    //std::cout << s + "<break>\n";

    return s;

}

/** Used for formatting a string for printing to the screen */
std::string Help::wrapText(const std::string s, std::string padding, size_t w)
{
    std::string wrappedText(padding);
    size_t cc = 0; // character count
    size_t ww = w - padding.size(); // available width with regards to eventual padding

    // loop through every char in string
    for (unsigned i = 0; i < s.size(); i++)
    {
        wrappedText += s[i];
        if (s[i] == ' ')
        {
            // we now have a possible point where to wrap the line.
            // peek ahead and see where next possible wrap point is:
            size_t next = s.substr(i).find_first_of(" ", 1);

            // if next wrap point is beyond the width, then wrap line now
            if (cc + next > ww && next != -1)
            {
                wrappedText += "\n" + padding;
                // reset char count for next line
                cc = 0;
            }
        }

        // increment char counter
        cc++;

        // special case: if were at a '\n' char, then reset char count as we 
        // already skipped on to a new line.
        if (s[i] == '\n')
        {
            wrappedText += padding;
            cc = 0;
        }
    }
    return wrappedText;

}

/** Initialize the help from an XML file */
void Help::initializeHelp(std::string helpDir)
{

    // find the path to the directory containing the help files
    RevBayesCore::RbFileManager fMngr = RevBayesCore::RbFileManager();
    //pathToHelpDir = fMngr.getCurrentDirectory();

   
    fMngr.setFilePath(this->helpDir);
    if (fMngr.testDirectory() == false)
    {
        RBOUT("Warning: Cannot find directory containing help files. User help is unavailable. Path = " + this->helpDir);
        return;
    }
    
    
}


/** Loads (parses) the xml help file into the pugi::doc*/
pugi::xml_parse_result Help::loadHelpFile(const std::string& qs)
{
    // the help file should be all lowercase if to be found
    std::string command = qs;
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);
    std::string helpfile = this->helpDir + command + ".xml";
    
    // try to load the corresponding xml file
    pugi::xml_parse_result result = doc.load_file(helpfile.c_str(), pugi::parse_default);
    if (result.status != pugi::status_ok)
    {
        RBOUT(result.description());
    }
    return result;
}

/** simple find and replace*/
std::string Help::replaceString(std::string subject, const std::string& search, const std::string& replace)
{
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos)
    {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}


bool Help::isHelpAvailableForQuery(std::string query) {
    return false;
}


std::string Help::stripConsecutiveSpace(std::string subject)
{
    std::string search = "  "; // this is 2 spaces
    size_t index;

    while ((index = subject.find(search)) != std::string::npos)
    { // remove 1 character from the string at index
        subject.erase(index, 1);
    }

    return subject;
}

/** home brewed to_string. */
template <typename T>
std::string Help::to_string(T value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}
