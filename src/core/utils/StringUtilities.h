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
    
    std::string                 formatStringForScreen(  const std::string   s,
                                                        std::string         firstLinePad,
                                                        std::string         hangingPad,
                                                        size_t              screenWidth);                           //!< Format RlString for output to screen
    std::string                 getFileContentsAsString(std::string& s);                                            //!< Convert the file contents to a RlString
    std::string                 getLastPathComponent(const std::string& s);                                         //!< Find the last component of a file path
    void                        stringSplit(std::string str, std::string delim, std::vector<std::string>& results); //!< Split a RlString into pieces
    void                        toLower(std::string& str);                                                          //!< Convert RlString's characters to lower case
    bool                        isNumber(std::string& s);                                                           //!< Checks if a RlString is a number
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
