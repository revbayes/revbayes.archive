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
 * mcmc.xml file.
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
#include "../libs/pugixml_1.2/src/pugixml.hpp"
#include <algorithm>
#include <string> 

/** Default constructor */
Help::Help(void) {    
    
    isHelpInitialized = false;
}

/** Destructor */
Help::~Help(void) {
    
}

/** This is what turn up in terminal window */
std::string Help::formatHelpString(const std::string& qs, size_t columnWidth) {
    
    this->loadHelpFile(qs);
   
    // the output to display in terminal
    std::string help = "\n";
    
    help += formatOutString(doc.child("help_entry").child("name").child_value(), columnWidth, 0, 2);
    help += formatOutString(doc.child("help_entry").child("succinct").child_value(), columnWidth, 0, 2);
    help += formatOutString(doc.child("help_entry").child("foo").child_value(), columnWidth, 0, 2); // No such tag, shouldn't affect output
    help += formatOutString(doc.child("help_entry").child("verbose").child_value(), columnWidth, 0, 2);      
    
    // arguments
    help += formatOutString("Arguments:", columnWidth, 0, 2);
    
    pugi::xpath_node_set args = doc.select_nodes("/help_entry/argument");
    for (pugi::xpath_node_set::const_iterator it = args.begin(); it != args.end(); ++it) {
        pugi::xpath_node node = *it;        
       
        help += formatOutString(node.node().child_value("arg_name"), columnWidth, 1, 1);
        help += formatOutString(node.node().child_value("arg_description"), columnWidth, 2, 2);
    }
    help += "\n\n";
    
    
    // usage   
    args = doc.select_nodes("/help_entry/usage");
    help += formatOutString("Usages:", columnWidth, 0, 2);
    
    for (pugi::xpath_node_set::const_iterator it = args.begin(); it != args.end(); ++it) {
        pugi::xpath_node node = *it;   
        
        help += formatOutString(node.node().child_value("theory"), columnWidth, 1, 2);
        help += formatOutString("Example:", columnWidth, 1, 1);
        help += formatOutString(node.node().child_value("example"), columnWidth, 1, 2);
    }
    help += "\n\n";
    
    // author
    help += formatOutString("Author:", columnWidth, 0, 2);    
    help += formatOutString(doc.child("help_entry").child("author").child_value(), columnWidth, 1, 4);
    
    // reference   
    help += formatOutString("References:", columnWidth, 0, 2);    
    args = doc.select_nodes("/help_entry/reference");
    for (pugi::xpath_node_set::const_iterator it = args.begin(); it != args.end(); ++it) {
        pugi::xpath_node node = *it;        
       
        help += formatOutString(node.node().child_value(), columnWidth, 1, 2);
    }
    help += "\n\n";
    
    return help;   
}

/** format a string to output*/
std::string Help::formatOutString(std::string s, size_t columnWidth, int indentLevel, int numLineBreaks){
    
    // remove any pre-formatting that may come from the xml-file
    s = replaceString(s, "\t", " ");
    s = replaceString(s, "\n", " ");
    s = replaceString(s, "  ", " ");
    
    // do nothing if string is empty
    if(s.length() <= 0){
        return "";
    }
    
    // apply the formatting style we want
    std::string indent = RevBayesCore::RbUtils::PAD;
    for(int i=0; i<indentLevel; i++){
        indent += RevBayesCore::RbUtils::PAD;
    }
    
    s = formatStringWithBreaks(s, indent, columnWidth);
    
    for(int i=0; i<numLineBreaks; i++){
        s += "\n";
    }
    return s;
}

/** Used for formatting a string for printing to the screen */
std::string Help::formatStringWithBreaks(const std::string s, std::string padding, size_t w) {
    
    std::vector<std::string> stringList;
    StringUtilities::stringSplit(s, " ", stringList);
    
    std::string nStr = padding;
    size_t cnt = 0;
    for (std::vector<std::string>::iterator it = stringList.begin(); it != stringList.end(); it++)
    {
        if (cnt + (*it).size() > w && cnt != 0)
        {
            cnt = 0;
            nStr += "\n" + padding;
        }
        cnt += (*it).size();
        nStr += (*it) + " ";
    }
    
    return nStr;
}


/** Initialize the help from an XML file */
void Help::initializeHelp() {

    // find the path to the directory containing the help files
    RevBayesCore::RbFileManager fMngr = RevBayesCore::RbFileManager();
    pathToHelpDir = fMngr.getCurDirectory();
        
    pathToHelpDir +=  + "/../../help/";
    fMngr.setFilePath( pathToHelpDir );
    if ( fMngr.testDirectory() == false )
    {
        RBOUT("Warning: Cannot find directory containing help files. User help is unavailable. Path = " + pathToHelpDir);
        return;
    }    
    
    isHelpInitialized = true;
}


/** Returns whether there is help available for a query */
bool Help::isHelpAvailableForQuery(const std::string& qs) {    
    pugi::xml_parse_result result = loadHelpFile(qs);
    return pugi::status_ok == result.status;
}

/** Loads (parses) the xml help file into the pugi::doc*/
pugi::xml_parse_result Help::loadHelpFile(const std::string& qs){
    // the help file should be all lowercase if to be found
    std::string command = qs;
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);
    std::string helpfile = pathToHelpDir + command + ".xml";
    
    // try to load the corresponding xml file
    pugi::xml_parse_result result = doc.load_file(helpfile.c_str(), pugi::parse_default);   
    return result;
}

/** simple find and replace*/
std::string Help::replaceString(std::string subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

