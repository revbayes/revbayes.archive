/**
 * @file
 * This file helper functions for manipulating RlStrings in RevBayes.
 *
 * @brief Namespace containing helper functions for RlStrings
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */
#ifndef RlStringUtilities_H
#define RlStringUtilities_H

#include <string>
#include <vector>

namespace StringUtilities {
    
    std::string                 formatStringForScreen(  const std::string  &s,
                                                        const std::string  &firstLinePad,
                                                        const std::string  &hangingPad,
                                                        size_t              screenWidth);                           //!< Format string for output to screen
    std::string                 getFileContentsAsString(const std::string& s);                                      //!< Convert the file contents to a string
    std::string                 getStringWithDeletedLastPathComponent(const std::string& s);                        //!< Convert the file contents to a string
    std::string                 getLastPathComponent(const std::string& s);                                         //!< Find the last component of a file path
    void                        stringSplit(const std::string &str, const std::string &delim, std::vector<std::string>& results); //!< Split a string into pieces
    void                        toLower(std::string& str);                                                          //!< Convert string's characters to lower case
    bool                        isNumber(const std::string& s);                                                     //!< Checks if a RlString is a number
    std::string                 oneLiner(const std::string& input, size_t maxLen);                                  //!< Get a one-liner of specified length
    void                        fillWithSpaces(std::string &s, size_t l, bool left);                                //!< Fill up the string with spaces either left aligned or right aligned
}

// TODO: Is there a better way for these global operators than duplicating the code for each namespace? (Sebastian)
namespace RevLanguage {

    std::string                     operator+(const std::string& A, double B);
    std::string                     operator+(const std::string& A, int B);
    std::string                     operator+(const std::string& A, size_t B);
    std::string                     operator+(double A, const std::string& B);
    std::string                     operator+(int A, const std::string& B);
    std::string                     operator+(size_t A, const std::string& B);

}

namespace RevBayesCore {
    
    std::string                     operator+(const std::string& A, double B);
    std::string                     operator+(const std::string& A, int B);
    std::string                     operator+(const std::string& A, size_t B);
    std::string                     operator+(double A, const std::string& B);
    std::string                     operator+(int A, const std::string& B);
    std::string                     operator+(size_t A, const std::string& B);
    
}

std::string                     operator+(const std::string& A, double B);
std::string                     operator+(const std::string& A, int B);
std::string                     operator+(const std::string& A, size_t B);
std::string                     operator+(double A, const std::string& B);
std::string                     operator+(int A, const std::string& B);
std::string                     operator+(size_t A, const std::string& B);


#endif
