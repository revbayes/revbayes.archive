/**
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

#include "HelpNode.h"

#include <string>
#include <vector>


class Help {

	public:
                                       ~Help(void);                                                                                         //!< Destructor
        std::string                     formatHelpString(const std::string& qs, size_t columnWidth);                                        //!< Format the help information for printing to the terminal
		static Help&                    getHelp(void)                                                                                       //!< Return a reference to the singleton help
                                           {
                                           static Help globalHelpInstance = Help();
                                           return globalHelpInstance;
                                           }
        HelpNode*                       getHelpNodeForQuery(const std::string& qs);                                                         //!< Returns the help node for a query
		void                            initializeHelp(std::string f);                                                                      //!< Initialize the help from an XML file
        bool                            isHelpAvailableForQuery(const std::string& qs);                                                     //!< Used by the parser to query the help tree for a specific command
        bool                            isUserHelpAvailable(void) { return isHelpInitialized; }                                             //!< Returns whether the help was successfully initialized
        void                            print(void);                                                                                        //!< Print the help tree

	private:
                                        Help(void);                                                                                         //!< Default constructor
                                        Help(const Help&);                                                                                  //!< Copy constructor (hidden away as this is a singleton class)
        Help&                           operator=(const Help&);                                                                             //!< Assignment operator (hidden away as this is a singleton class)
        size_t                          getNumHelpEntries(void);                                                                            //!< Returns the number of help entries for a command
        std::string                     formatStringWithBreaks(const std::string s, std::string padding, size_t w);                         //!< Used for formatting a string for printing to the screen
        std::string                     formatStringWithBreaks(const std::string s, std::string padding, size_t col, size_t w);             //!< Used for formatting a string for printing to the screen
        std::string                     formatStringWithBreaks(const std::string s, std::string padding1, std::string padding2, size_t w);  //!< Used for formatting a string for printing to the screen
        bool                            parseHelpFile(std::string& fn);                                                                     //!< Parse the XML file containing user help information
        std::string                     getNextTag(HelpNode* p, std::istream& inStream);                                                    //!< Get the next XML tag in the help file
        std::string                     getNextTag(HelpNode* p, std::string& s);                                                            //!< Get the next XML tag in the help file
        char                            getSpecialCharacter(std::istream& inStream);                                                        //!< Parse special characters, such as "&apos;"
        void                            skipWhiteSpace(std::istream& inStream);                                                             //!< Skip white space
        
        bool                            isHelpInitialized;                                                                                  //!< Flag indicating whether the help has been initialized
        HelpNode*                       helpRoot;                                                                                           //!< The root of the help tree
        std::vector<HelpNode*>          helpNodes;                                                                                          //!< A vector holding the help nodes that were allocated
        int                             numHelpNodes;                                                                                       //!< The number of help nodes
};

#endif


