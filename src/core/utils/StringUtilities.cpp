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

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>


/** Format string for printing to screen, with word wrapping, and various indents */
std::string StringUtilities::formatStringForScreen(const std::string s, std::string firstLinePad, std::string hangingPad, size_t screenWidth) {

    std::string outputString;

    std::vector<std::string> lineList;
    StringUtilities::stringSplit( s, "\n", lineList );

    for ( size_t i=0; i<lineList.size(); i++ ) {
    
        std::vector<std::string> stringList;
        StringUtilities::stringSplit(lineList[i], " ", stringList);

        if ( stringList.size() > 0 ) {
            outputString += firstLinePad;

            size_t count = firstLinePad.size();
            for ( std::vector<std::string>::iterator it = stringList.begin(); it != stringList.end(); it++ ) {

                if ( count + (*it).size() > screenWidth && count != 0 ) {
                    outputString[outputString.size()-1] = '\n';
                    outputString += hangingPad;
                    count = hangingPad.size();
                }
                count += (*it).size() + 1;
                outputString += (*it) + " ";
            }
            outputString[outputString.size()-1] = '\n';
        }
        else
            outputString += "\n";
    }

    return outputString;
}


/** Return file contents as string with '\n' line breaks */
std::string StringUtilities::getFileContentsAsString(std::string& s) {

    // open file
	std::ifstream fStrm;
    fStrm.open(s.c_str(), std::ios::in);
    if ( !fStrm.is_open() )
        return "";
        
    // read the file
    int ch;
    std::string retStr = "";
    while ( (ch = fStrm.get()) != EOF)
        {
        char c = (char)ch;
        
        if (ch == '\n' || ch == '\r' || ch == EOF)
            retStr += '\n';
        else
            retStr += c;
        } 

    // close file
    fStrm.close();

    return retStr;
}


/** Find the last component of a file path */
std::string StringUtilities::getLastPathComponent(const std::string& s) {

    std::vector<std::string> sVec;
    stringSplit(s, "/", sVec);
    std::string lastComponent = "";
    if (sVec.size() > 0)
        lastComponent = sVec[sVec.size()-1];
    return lastComponent;
}


/** Determine if the string s represents a number */
bool StringUtilities::isNumber(std::string& s) {

    for (size_t i=0; i<s.size(); i++)
        {
        if (!isdigit(s[i]))
            return false;
        }
    return true;
}


/** Utility function for dividing string into pieces */
void StringUtilities::stringSplit(std::string str, std::string delim, std::vector<std::string>& results) {

    size_t cutAt;
    while ( (cutAt = str.find_first_of(delim)) != str.npos )
        {
        if (cutAt > 0)
            results.push_back(str.substr(0, cutAt));
        else
            results.push_back( "" );
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


/** Global operator for appending double to std::string */
std::string operator+( const std::string& A, double B ) {
    
    std::stringstream o;
    o << A << B;
    return o.str();
}


/** Global operator for appending double to std::string */
std::string operator+( const std::string& A, int B ) {
    
    std::stringstream o;
    o << A << B;
    return o.str();
}


/** Global operator for appending double to std::string */
std::string operator+( const std::string& A, size_t B ) {
    
    std::stringstream o;
    o << A << B;
    return o.str();
}


/** Global operator for appending double to std::string */
std::string operator+( double A, const std::string& B ) {
    
    std::stringstream o;
    o << A << B;
    return o.str();
}


/** Global operator for appending double to std::string */
std::string operator+( int A, const std::string& B ) {
    
    std::stringstream o;
    o << A << B;
    return o.str();
}


/** Global operator for appending double to std::string */
std::string operator+( size_t A, const std::string& B ) {
    
    std::stringstream o;
    o << A << B;
    return o.str();
}


/** Global operator for appending double to std::string */
std::string RevLanguage::operator+( const std::string& A, double B ) {
    
    std::stringstream o;
    o << A << B;
    return o.str();
}


/** Global operator for appending double to std::string */
std::string RevLanguage::operator+( const std::string& A, int B ) {
    
    std::stringstream o;
    o << A << B;
    return o.str();
}


/** Global operator for appending double to std::string */
std::string RevLanguage::operator+( const std::string& A, size_t B ) {
    
    std::stringstream o;
    o << A << B;
    return o.str();
}


/** Global operator for appending double to std::string */
std::string RevLanguage::operator+( double A, const std::string& B ) {
    
    std::stringstream o;
    o << A << B;
    return o.str();
}


/** Global operator for appending double to std::string */
std::string RevLanguage::operator+( int A, const std::string& B ) {
    
    std::stringstream o;
    o << A << B;
    return o.str();
}


/** Global operator for appending double to std::string */
std::string RevLanguage::operator+( size_t A, const std::string& B ) {
    
    std::stringstream o;
    o << A << B;
    return o.str();
}


/** Global operator for appending double to std::string */
std::string RevBayesCore::operator+( const std::string& A, double B ) {
    
    std::stringstream o;
    o << A << B;
    return o.str();
}


/** Global operator for appending double to std::string */
std::string RevBayesCore::operator+( const std::string& A, int B ) {
    
    std::stringstream o;
    o << A << B;
    return o.str();
}


/** Global operator for appending double to std::string */
std::string RevBayesCore::operator+( const std::string& A, size_t B ) {
    
    std::stringstream o;
    o << A << B;
    return o.str();
}


/** Global operator for appending double to std::string */
std::string RevBayesCore::operator+( double A, const std::string& B ) {
    
    std::stringstream o;
    o << A << B;
    return o.str();
}


/** Global operator for appending double to std::string */
std::string RevBayesCore::operator+( int A, const std::string& B ) {
    
    std::stringstream o;
    o << A << B;
    return o.str();
}


/** Global operator for appending double to std::string */
std::string RevBayesCore::operator+( size_t A, const std::string& B ) {
    
    std::stringstream o;
    o << A << B;
    return o.str();
}




