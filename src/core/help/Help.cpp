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
#include "StringUtilities.h"
#include "UserInterface.h"
#include <iostream>
#include <sstream>



/** Default constructor */
Help::Help(void) {

    helpRoot = NULL;
    isHelpInitialized = false;
}


/** Copy constructor */
Help::Help(const Help&) {

    helpRoot = NULL;
    isHelpInitialized = false;
}


/** Destructor */
Help::~Help(void) {

}


/** Initialize the help from an XML file */
void Help::initializeHelp(std::string f) {

    std::cout << "Initializing user help system " << f << std::endl;
    
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
    helpRoot->setTagType(ROOT);
    helpRoot->setHelpEntry("");
    for (std::vector<std::string>::iterator p = helpFiles.begin(); p != helpFiles.end(); p++)
        {
        std::cout << "Help file: " << (*p) << std::endl;
        if ( parseHelpFile(*p) == false )
            {
            std::string problemFile = StringUtilities::getLastPathComponent(*p);
            RBOUT("Warning: Unable to read help file \"" + problemFile + "\"");
            }
        }
    
    // check that the help tree has some entries

    
    isHelpInitialized = true;
}


/** Parse an XML file */
bool Help::parseHelpFile(std::string& fn) {

    // open file
	std::ifstream fStrm;
    fStrm.open(fn.c_str(), std::ios::in);

    // read the file token-by-token looking for Fasta things
    HelpNode* p = helpRoot;
    std::string str = "";
    enum ReadType { NADA, OPEN_TAG, CLOSE_TAG, TAG_INFO};
    ReadType readType = NADA;
    int ch;
    while ( (ch = fStrm.get()) != EOF)
        {
        char c = (char)ch;
        //std::cout << c;
        
        if (ch == '\n' || ch == '\r' || ch == EOF)
            {
            //std::cout << std::endl;
            }
        else if (c == '<')
            {
            if (str != "")
                std::cout << "tag entry: " << str << std::endl;
            str = "";
            readType = OPEN_TAG;
            }
        else if (c == '>')
            {
            if (str != "")
                std::cout << "tag type:  " << str << std::endl;
            if (readType == OPEN_TAG)
                {
                HelpNode* newNode = new HelpNode();
                newNode->setParent(p);
                p->addChild(newNode);
                p = newNode;
                }
            else if (readType == CLOSE_TAG)
                {
                if (p->getParent() != NULL)
                    p = p->getParent();
                readType = TAG_INFO;
                }
            str = "";
            }
        else if (c == '/')
            {
            str = "";
            readType = CLOSE_TAG;
            }
        else
            {
            if (readType == OPEN_TAG || readType == CLOSE_TAG)
                {
                if (c != ' ')
                    str += c;
                }
            else 
                {
                if (c == ' ')
                    {
                    if (str.size() > 1)
                        if ( str[str.size()-1] != ' ' )
                            str += c;
                    }
                else 
                    str += c;
                }
            }
                        
        } 

    // close file
    fStrm.close();

    return true;
}

       
void Help::skipWhiteSpace(std::istream& inStream) {

    while (isspace(inStream.peek()))
        inStream.get();
}

