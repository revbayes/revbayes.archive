/**
 * @file
 * This file helper functions for manipulating strings in RevBayes.
 *
 * @brief Namespace containing helper functions for strings
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */
#ifndef StringUtilities_H
#define StringUtilities_H

#include <string>
#include <vector>

namespace StringUtilities {
    
    std::string                 formatStringForScreen(  const std::string   s,
                                                        std::string         firstLinePad,
                                                        std::string         hangingPad,
                                                        size_t              screenWidth);                           //!< Format string for output to screen
    std::string                 getFileContentsAsString(std::string& s);                                            //!< Convert the file contents to a string
    std::string                 getLastPathComponent(const std::string& s);                                         //!< Find the last component of a file path
    void                        stringSplit(std::string str, std::string delim, std::vector<std::string>& results); //!< Split a string into pieces
    void                        toLower(std::string& str);                                                          //!< Convert string's characters to lower case
    bool                        isNumber(std::string& s);                                                           //!< Checks if a string is a number
}

#endif