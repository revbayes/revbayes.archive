/**
 * @file
 * This file contains the implementation of StringUtilities, which 
 * helper functions for manipulating strings.
 *
 * @brief Declaration of StringUtilities
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#include "StringUtilities.h"



/** Find the last component of a file path */
std::string StringUtilities::getLastPathComponent(std::string& s) {

    std::vector<std::string> sVec;
    stringSplit(s, "/", sVec);
    std::string lastComponent = "";
    if (sVec.size() > 0)
        lastComponent = sVec[sVec.size()-1];
    return lastComponent;
}


/** Utility function for dividing string into pieces */
void StringUtilities::stringSplit(std::string str, std::string delim, std::vector<std::string>& results) {

    size_t cutAt;
    while ( (cutAt = str.find_first_of(delim)) != str.npos )
        {
        if (cutAt > 0)
            results.push_back(str.substr(0, cutAt));
        str = str.substr(cutAt+1);
        }
    if (str.length() > 0)
        results.push_back(str);
}


/** Utility function for converting string to all lower case */
void StringUtilities::toLower(std::string& str) {

    for (size_t i=0; i<str.size(); i++)
        str[i] = tolower(str[i]);
}
