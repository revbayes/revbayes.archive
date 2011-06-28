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

#include <string>

class HelpNode;

class Help {

	public:
		static Help&                     getHelp(void)                             //!< Return a reference to the singleton help
                                            {
                                            static Help globalHelpInstance;
                                            return globalHelpInstance;
                                            }
		void                             initializeHelp(std::string f);            //!< Initialize the help from an XML file

	private:
                                         Help(void);                               //!< Default constructor
                                         Help(const Help&);                        //!< Copy constructor
        Help&                            operator=(const Help&);                   //!< Assignment operator
								        ~Help(void);                               //!< Destructor
        void                             skipWhiteSpace(std::istream& inStream);   //!< Skip white space
        bool                             isHelpInitialized;                        //!< Flag indicating whether the help has been initialized
        bool                             parseHelpFile(std::string& fn);           //!< Parse the XML file containing user help information
        HelpNode*                        helpRoot;                                 //!< The root of the help tree
};

#endif


