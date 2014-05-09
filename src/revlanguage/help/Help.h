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
#include "pugixml.hpp"
#include "IHelp.h"

class Help : public IHelp {
public:
 
    void initializeHelp(std::string helpDir); //!< Initialize the help from an XML file
    //~Help(void); //!< Destructor
    std::string getHelp(std::string qs, int columnWidth); //!< Format the help information for printing to the terminal
    bool isHelpAvailableForQuery(std::string query);

//    static Help& getHelp(void) //!< Return a reference to the singleton help
//    {
//        static Help globalHelpInstance = Help();
//        return globalHelpInstance;
//    }
//    void initializeHelp(std::string helpDir); //!< Initialize the help from an XML file
//    bool isHelpAvailableForQuery(const std::string& qs); //!< Used by the parser to query the help tree for a specific command
//
//    bool isUserHelpAvailable(void) {
//        return isHelpInitialized;
//    } //!< Returns whether the help was successfully initialized
    
private:
//    Help(void); //!< Default constructor
//    Help(const Help&); //!< Copy constructor (hidden away as this is a singleton class)
//    Help& operator=(const Help&); //!< Assignment operator (hidden away as this is a singleton class)
    
    
    std::string wrapText(const std::string s, std::string padding, size_t w); //!< Used for formatting a string for printing to the screen
    pugi::xml_parse_result loadHelpFile(const std::string& qs); //!< loads the help xml file, name of file is expected to be <query>.xml
    std::string formatOutString(std::string s, size_t columnWidth, int indentLevel, int numLineBreaks);
    std::string formatOutString(std::string s, size_t columnWidth, int indentLevel, int numLineBreaks, bool stripLineBreaks);
    std::string replaceString(std::string subject, const std::string& search, const std::string& replace);
    template <typename T>std::string to_string(T value);
    std::string stripConsecutiveSpace(std::string subject);


    std::string helpDir;
    pugi::xml_document doc; //!< the (parsed) xml help file

};

#endif

