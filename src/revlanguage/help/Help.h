/**
 *  Help.h
 *
 * @file
 * This file contains the declaration of Help, which is
 * used to manage user help. The class is created on start
 * up and initialized from an XML file or files pointed to
 * by a string containing the path to the directory or file
 * containing the help.
 *
 * @brief Declaration of Help
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#ifndef Help_H
#define Help_H

#include <string>
#include <vector>
#include "../libs/pugixml_1.2/src/pugixml.hpp"


class Help {
public:
    ~Help(void); //!< Destructor
    std::string formatHelpString(const std::string& qs, size_t columnWidth); //!< Format the help information for printing to the terminal

    static Help& getHelp(void) //!< Return a reference to the singleton help
    {
        static Help globalHelpInstance = Help();
        return globalHelpInstance;
    }
    void initializeHelp(); //!< Initialize the help from an XML file
    bool isHelpAvailableForQuery(const std::string& qs); //!< Used by the parser to query the help tree for a specific command

    bool isUserHelpAvailable(void) {
        return isHelpInitialized;
    } //!< Returns whether the help was successfully initialized

private:
    Help(void); //!< Default constructor
    Help(const Help&); //!< Copy constructor (hidden away as this is a singleton class)
    Help& operator=(const Help&); //!< Assignment operator (hidden away as this is a singleton class)
    std::string formatStringWithBreaks(const std::string s, std::string padding, size_t w); //!< Used for formatting a string for printing to the screen
    pugi::xml_parse_result loadHelpFile(const std::string& qs); //!< loads the help xml file, name of file is expected to be <query>.xml
    std::string formatOutString(std::string s, size_t columnWidth, int indentLevel, int numLineBreaks);
    std::string replaceString(std::string subject, const std::string& search, const std::string& replace);
    
    bool isHelpInitialized; //!< Flag indicating whether the help has been initialized
    std::string pathToHelpDir; //!< Where the help xml files are located
    pugi::xml_document doc; //!< the (parsed) xml help file
};

#endif

