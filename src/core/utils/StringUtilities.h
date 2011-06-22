/**
 * @file
 * This file helper functions for manipulating strings in RevBayes.
 *
 * @brief Namespace containing helper functions for strings
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-11 16:58:43 -0800 (Fre, 11 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: StringUtilities.h 203 2009-12-23 14:33:11Z Hoehna $
 */
#ifndef StringUtilities_H
#define StringUtilities_H

#include <string>
#include <vector>

namespace StringUtilities {
    
    void                        stringSplit(std::string str, std::string delim, std::vector<std::string>& results); //!< Split a string into pieces
    void                        toLower(std::string& str);                                                          //!< Convert string's characters to lower case
}

#endif