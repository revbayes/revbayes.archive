/**
 * @file
 * This file contains the implementation of Help, a singleton 
 * class that manages help for the user.
 *
 * @brief Implementation of Help
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
#include "HelpNode.h"
#include "RbFileManager.h"
#include "RbNames.h"
#include "StringUtilities.h"
#include "UserInterface.h"
#include <iostream>
#include <sstream>

#undef DEBUG_HELP_SYSTEM



/** Default constructor */
Help::Help(void) {

    numHelpNodes = 0;
    helpRoot = NULL;
    isHelpInitialized = false;
}


/** Destructor */
Help::~Help(void) {

    for (std::vector<HelpNode*>::iterator p = helpNodes.begin(); p != helpNodes.end(); p++)
        delete (*p);
}


/** Format a string for printing to the terminal */
std::string Help::formatHelpString(const std::string& qs, size_t columnWidth) {

    // get the base node in the tree for the query
    HelpNode* h = getHelpNodeForQuery(qs);
    if (h == NULL)
        return "";
        
    // make certain that the column width is something somewhat reasonable
    if (columnWidth < 30)
        columnWidth = 30;
    
    // format the string
    std::string hStr = "\n";
    
    // name (this tag must be present, or we would never have made it this
    // far in the function)
    if ( h->hasChildWithTag("name") == true )
        {
        HelpNode* hn = h->getChildWithTag("name");
        std::string tempStr = hn->getHelpEntry();
        
        // succinct
        if ( h->hasChildWithTag("succinct") == true )
            {
            HelpNode* hn = h->getChildWithTag("succinct");
            tempStr += ": " + hn->getHelpEntry();
            }
        hStr += formatStringWithBreaks(tempStr, pad, columnWidth);
        hStr += "\n";
        }
    
    // alias
    
    // verbose
    if ( h->hasChildWithTag("verbose") == true )
        {
        HelpNode* hn = h->getChildWithTag("verbose");
        hStr += "\n";
        hStr += formatStringWithBreaks(hn->getHelpEntry(), pad, columnWidth);
        hStr += "\n";
        }

    // arguments
    size_t nArgs = h->getNumChildrenWithTag("argument");
    if (nArgs > 0)
        {
        hStr += "\n";
        if (nArgs == 1)
            hStr += formatStringWithBreaks("Argument:\n", pad, columnWidth);
        else 
            hStr += formatStringWithBreaks("Arguments:\n", pad, columnWidth);
        size_t longestArgName = 0;
        for (size_t i=0; i<nArgs; i++)
            {
            HelpNode* hn1 = h->getChildWithTag("argument", i)->getChildWithTag("arg_name");
            std::string temp = hn1->getHelpEntry();
            if (temp.size() > longestArgName)
                longestArgName = temp.size();
            }
        longestArgName += 1;
        for (size_t i=0; i<nArgs; i++)
            {
            HelpNode* hn1 = h->getChildWithTag("argument", i)->getChildWithTag("arg_name");
            HelpNode* hn2 = h->getChildWithTag("argument", i)->getChildWithTag("arg_description");
            hStr += "\n";
            hStr += formatStringWithBreaks(hn1->getHelpEntry() + " " + hn2->getHelpEntry(), pad, longestArgName, columnWidth);
            }
        hStr += "\n";
        }
    
    // usage (theory)
    size_t nUsage = h->getNumChildrenWithTag("usage");
    if ( nUsage > 0 )
        {
        hStr += "\n";
        hStr += formatStringWithBreaks("Usage:\n", pad, columnWidth);
        for (size_t i=0; i<nUsage; i++)
            {
            HelpNode* hn = h->getChildWithTag("usage", i)->getChildWithTag("theory");
            hStr += "\n";
            hStr += formatStringWithBreaks(hn->getHelpEntry(), pad, columnWidth);
            }
        hStr += "\n";
        }

    // usage (example)
    if ( nUsage > 0 )
        {
        hStr += "\n";
        if (nUsage == 1)
            hStr += formatStringWithBreaks("Usage example:\n", pad, columnWidth);
        else
            hStr += formatStringWithBreaks("Usage examples:\n", pad, columnWidth);
        for (size_t i=0; i<nUsage; i++)
            {
            HelpNode* hn = h->getChildWithTag("usage", i)->getChildWithTag("example");
            hStr += "\n";
            hStr += formatStringWithBreaks(hn->getHelpEntry(), pad, columnWidth);
            }
        hStr += "\n";
        }
                
    // author
    if ( h->hasChildWithTag("author") == true )
        {
        HelpNode* hn = h->getChildWithTag("author");
        hStr += "\n";
        hStr += formatStringWithBreaks("Author: " + hn->getHelpEntry(), pad, columnWidth);
        hStr += "\n";
        }
    
    // reference
    size_t nRef = h->getNumChildrenWithTag("reference");
    if ( nRef > 0 )
        {
        hStr += "\n";
        hStr += formatStringWithBreaks("References:\n", pad, columnWidth);
        for (size_t i=0; i<nRef; i++)
            {
            HelpNode* hn = h->getChildWithTag("reference", i);
            hStr += "\n";
            hStr += formatStringWithBreaks(hn->getHelpEntry(), pad, pad+"   ", columnWidth);
            }
        hStr += "\n";
        }
    
    return hStr;
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


/** Used for formatting a string for printing to the screen */
std::string Help::formatStringWithBreaks(const std::string s, std::string padding1, std::string padding2, size_t w) {

    std::vector<std::string> stringList;
    StringUtilities::stringSplit(s, " ", stringList);

    std::string nStr = padding1;
    size_t cnt = 0;
    for (std::vector<std::string>::iterator it = stringList.begin(); it != stringList.end(); it++)
        {
        if (cnt + (*it).size() > w && cnt != 0)
            {
            cnt = 0;
            nStr += "\n" + padding2;
            }
        cnt += (*it).size();
        nStr += (*it) + " ";
        }
    
    return nStr;
}


/** Used for formatting a string for printing to the screen */
std::string Help::formatStringWithBreaks(const std::string s, std::string padding, size_t col, size_t w) {

    std::vector<std::string> stringList;
    StringUtilities::stringSplit(s, " ", stringList);

    std::string nStr = padding;
    size_t cnt = 0, elemNum = 0;
    for (std::vector<std::string>::iterator it = stringList.begin(); it != stringList.end(); it++)
        {
        if (cnt + (*it).size() > w && cnt != 0)
            {
            cnt = 0;
            nStr += "\n" + padding;
            for (size_t i=0; i<col; i++)
                nStr += " ";
            }
        cnt += (*it).size();
        nStr += (*it);
        if (elemNum == 0)
            {
            for (size_t i=0; i<col-(*it).size(); i++)
                nStr += " ";
            }
        else
            nStr += " ";
        elemNum++;
        }
    
    return nStr;
}


/** Returns the help node for a query */
HelpNode* Help::getHelpNodeForQuery(const std::string& qs) {

    std::string theString = qs;
    StringUtilities::toLower(theString);

    for (size_t i=0; i<helpRoot->getNumChildren(); i++)
        {
        HelpNode* h = helpRoot->getChildIndexed(i);
        if ( h->getTagName() == "help_entry" )
            {
            // look for the "name" tag
            for ( size_t j=0; j<h->getNumChildren(); j++)
                {
                std::string tn = h->getChildIndexed(j)->getTagName();
                std::string te = h->getChildIndexed(j)->getHelpEntry();
                StringUtilities::toLower(te);
                if (tn == "name" && te == theString)
                    return h;
                }
            }
        }

    return NULL;
}


/** Gets the next XML tag when parsing an XML help file */
std::string Help::getNextTag(HelpNode* p, std::string& s) {

    std::stringstream myStrm(s);
    return getNextTag(p, myStrm);
}


/** Gets the next XML tag when parsing an XML help file */
std::string Help::getNextTag(HelpNode* p, std::istream& inStream) {

    // remove leading white space
    skipWhiteSpace(inStream);

    // set flags and variables
    std::string theTagName = "";
    std::string tagName = "";
    std::string tagContents = "";
    bool readingTag = false, isClosingTag = false, isPreviousWhiteSpace = false;
    
    // read the stream
    int ch;
    while ( (ch = inStream.get()) != EOF)
        {
        // read a character, treating end of line characters as spaces
        char c = (char)ch;
        if (ch == '\n' || ch == '\r'  || ch == '\t' || ch == EOF)
            c = ' ';
            
        // deal with special characters
        if (c == '&')
            c = getSpecialCharacter(inStream);
            
        // we don't allow white spaces to pile up
        if (c == ' ')
            {
            if (isPreviousWhiteSpace == true)
                continue;
            isPreviousWhiteSpace = true;
            }
        else
            isPreviousWhiteSpace = false;
            
        // interpret the XML tag
        if (c == '<')
            {
            tagName = "";
            readingTag = true;
            isClosingTag = false;
            }
        else if (c == '>')
            {
            readingTag = false;
            if (theTagName == "" && isClosingTag == false)
                {
                theTagName = tagName;
                }
            else if (theTagName == "" && isClosingTag == true)
                {
                return "";
                }
            else 
                {
                if (tagName == theTagName && isClosingTag == true)
                    {
                    // make a new help node
                    HelpNode* newNode = new HelpNode;
                    newNode->setIndex( numHelpNodes++ );
                    helpNodes.push_back( newNode );
                    StringUtilities::toLower(theTagName);
                    newNode->setTagName( theTagName );
                    newNode->setHelpEntry(tagContents);
                    p->addChild(newNode);
                    newNode->setParent(p);
                    
                    // read the tag contents
                    getNextTag(newNode, tagContents);
                    
                    // read the remaining stream
                    getNextTag(p, inStream);
                    }
                else if (tagName == theTagName && isClosingTag == false)
                    {
                    return "";
                    }
                else
                    {
                    tagContents += "<";
                    if (isClosingTag == true)
                        tagContents += "/";
                    tagContents += tagName + ">";
                    }
                }
            }
        else if (c == '/' && readingTag == true)
            {
            isClosingTag = true;
            }
        else
            {
            if (readingTag == true && c != ' ')
                tagName += c;
            else   
                tagContents += c;
            }
        }
        
    return "";
}


/** Returns the number of help entries for a query */
size_t Help::getNumHelpEntries(void) {

    size_t numHelpEntries = 0;
    for (size_t i=0; i<helpRoot->getNumChildren(); i++)
        {
        HelpNode* h = helpRoot->getChildIndexed(i);
        if ( h->getTagName() == "help_entry" )
            numHelpEntries++;
        }
    return numHelpEntries;
}


/** Interprets special XML characters, such as "&gt;" */
char Help::getSpecialCharacter(std::istream& inStream) {

    // zip forward, looking for the closing semicolon and then interpret and return the character
    std::string specialCharName = "";
    int ch;
    while ( (ch = inStream.get()) != ';' )
        {
        char c = (char)ch;
        if (c != '&' && c != ';')
            specialCharName += c;
        }    
    StringUtilities::toLower(specialCharName);
    if (specialCharName == "amp")
        return '&';
    else if (specialCharName == "lt")
        return '<';
    else if (specialCharName == "gt")
        return '>';
    else if (specialCharName == "quot")
        return '\"';
    else if (specialCharName == "apos")
        return '\'';
    else if (specialCharName == "frasl")
        return '/';    
    return '?';
}


/** Initialize the help from an XML file */
void Help::initializeHelp(std::string f) {

#   if defined (DEBUG_HELP_SYSTEM)
    std::cout << "Initializing user help system " << f << std::endl;
#   endif

    // find the path to the directory containing the help files
    RbFileManager fMngr = RbFileManager();
    std::string pathToHelpDir = fMngr.getCurDirectory();
    pathToHelpDir += "/help";
    fMngr.setFilePath( pathToHelpDir );
    if ( fMngr.testDirectory() == false )
        {
        RBOUT("Warning: Cannot find directory containing help files. User help is unavailable.");
        return;
        }
        
    // initialize a vector of strings containing the files in the help directory
    std::vector<std::string> helpFiles;
    fMngr.setStringWithNamesOfFilesInDirectory( helpFiles );
    if ( helpFiles.size() == 0 )
        {
        RBOUT("Warning: The help directory contains no help files. User help is unavailable.");
        return;
        }
        
    // open each help file (which is in XML format) and parse its contents
    if (helpRoot == NULL)
        helpRoot = new HelpNode();
    helpRoot->setTagName("root");
    helpRoot->setHelpEntry("");
    for (std::vector<std::string>::iterator p = helpFiles.begin(); p != helpFiles.end(); p++)
        {
#       if defined (DEBUG_HELP_SYSTEM)
        std::cout << "Help file: " << (*p) << std::endl;
#       endif
        if ( parseHelpFile(*p) == false )
            {
            std::string problemFile = StringUtilities::getLastPathComponent(*p);
            RBOUT("Warning: Unable to read help file \"" + problemFile + "\"");
            }
        }
        
    // do some post-processing on the help tree, removing information from nodes that are not leaves
    for (std::vector<HelpNode*>::iterator p = helpNodes.begin(); p != helpNodes.end(); p++)
        {
        if ( (*p)->isLeaf() == false )
            (*p)->setHelpEntry("");
        }
    
    // check that the help tree has some entries
    if ( getNumHelpEntries() == 0 )
        {
        RBOUT("Warning: Found no user help entries. User help is unavailable.");
        return;
        }

#   if defined (DEBUG_HELP_SYSTEM)
    print();
#   endif

    isHelpInitialized = true;
}


/** Returns whether there is help available for a query */
bool Help::isHelpAvailableForQuery(const std::string& qs) {

    if ( getHelpNodeForQuery(qs) == NULL)
        return false;
    return true;
}


/** Parse an XML file */
bool Help::parseHelpFile(std::string& fn) {

    // open file
	std::ifstream fStrm;
    fStrm.open(fn.c_str(), std::ios::in);

    // read the file token-by-token looking for Fasta things
    getNextTag(helpRoot, fStrm);

    // close file
    fStrm.close();
    
    return true;
}

       
/** Print the help tree */
void Help::print(void) {

    helpRoot->showNode(helpRoot, 3);
    return;
    
    size_t i = 0;
    for (std::vector<HelpNode*>::iterator p = helpNodes.begin(); p != helpNodes.end(); p++)
        {
        std::cout << (*p)->getIndex() << " -- (";
        for (size_t j=0; j<(*p)->getNumChildren(); j++)
            {
            std::cout << (*p)->getChildIndexed(j)->getIndex();
            if ( j+1 != (*p)->getNumChildren() )
                std::cout << " ";
            }
        std::cout << ") " << (*p)->getTagName() << " -- " << (*p)->getHelpEntry() << std::endl;
        i++;
        }
}


/** Skip white space in a stream */
void Help::skipWhiteSpace(std::istream& inStream) {

    while (isspace(inStream.peek()))
        inStream.get();
}

