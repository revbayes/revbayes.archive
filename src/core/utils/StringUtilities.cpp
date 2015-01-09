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





/**
 * Fill this string with spaces so that it has the required length.
 * Either fill the spaces on the right if left aligned (true)
 * or on the left if right aligned.
 */
void StringUtilities::fillWithSpaces(std::string &s, int l, bool left)
{
    
    for (int i=int(s.length()); i<l ; ++i)
    {
        // either left algined
        if (left == true)
        {
            s.push_back(' ');
        }
        else
        {
            s.insert(s.begin(), ' ');
        }
        
    }
    
}


/**
 * Wraps text so that each line doesn't exceeds column width.
 * Prepends tabs to each line.
 *
 * @param s             string to format
 * @param tabs          number of tabs to prepend to each line
 * @param width         column width
 * @param removeFormat  strips newline, tab etc if set to true
 * @return              formatted text
 */
std::string StringUtilities::formatTabWrap(std::string s, size_t tabs, size_t width, bool removeFormat)
{
    
    std::string tabbing = "";
    for (int i = 0; i < tabs * 4; i++)
    {
        tabbing.append(" ");
    }
    
    std::string result = tabbing;
    size_t w = width - tabbing.size();
    int cc = 0; // character count
    char lastChar = '\0';
    
    for (unsigned int i = 0; i < s.size(); i++)
    {
        if (result.size() > 0)
        {
            lastChar = result[result.size() - 1];
        }
        
        // strip consecutive spaces
        if (!(lastChar == ' ' && s[i] == ' '))
        {
            if (!removeFormat)
            {
                result += s[i];
                lastChar = s[i];
                cc++;
            }
            else if (!StringUtilities::isFormattingChar(s[i]))
            {
                result += s[i];
                lastChar = s[i];
                cc++;
            }
        }
        
        if (lastChar == '\n')
        {
            cc = 0;
            result.append(tabbing);
        }
        
        if (lastChar == ' ')
        {
            // we now have a possible point where to wrap the line.
            // peek ahead and see where next possible wrap point is:
            size_t next = s.substr(i).find_first_of(" ", 1);
            
            // if next wrap point is beyond the width, then wrap line now
            if (cc + next >= w)
            {
                result.append("\n").append(tabbing);
                // reset char count for next line
                cc = 0;
            }
        }
    }
    return result;
}


/** Format string for printing to screen, with word wrapping, and various indents */
std::string StringUtilities::formatStringForScreen(const std::string &s, const std::string &firstLinePad, const std::string &hangingPad, size_t screenWidth) {

    std::string outputString;

    std::vector<std::string> lineList;
    StringUtilities::stringSplit( s, "\n", lineList );

    for ( size_t i=0; i<lineList.size(); i++ )
    {
    
        std::vector<std::string> stringList;
        StringUtilities::stringSplit(lineList[i], " ", stringList);

        if ( stringList.size() > 0 )
        {
            outputString += firstLinePad;

            size_t count = firstLinePad.size();
            for ( std::vector<std::string>::iterator it = stringList.begin(); it != stringList.end(); it++ )
            {

                if ( count + (*it).size() > screenWidth && count != 0 )
                {
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
        {
            outputString += "\n";
        }
        
    }

    return outputString;
}


std::string StringUtilities::getStringWithDeletedLastPathComponent(const std::string& s)
{
    
#	ifdef WIN32
    std::string pathSeparator = "\\";
#	else
    std::string pathSeparator = "/";
#   endif
    
    std::string tempS = s;
	size_t location = tempS.find_last_of( pathSeparator );
	if ( location == std::string::npos )
    {
		/* There is no path in this string. We
         must have only the file name. */
		return "";
    }
	else if ( location == tempS.length() - 1 )
    {
		/* It looks like the last character is "/", which
         means that no file name has been provided. */
		return tempS;
    }
	else
    {
		/* We can divide the path into the path and the file. */
		tempS.erase( location );
		return tempS;
    }
    
    return "";
}


/** Return file contents as string with '\n' line breaks */
std::string StringUtilities::getFileContentsAsString(const std::string& s)
{

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
    {
        lastComponent = sVec[sVec.size()-1];
    }
    
    return lastComponent;
}



/**
 * Indicates if a char is affecting text formatting
 * @param c
 * @return
 */
bool StringUtilities::isFormattingChar(char c)
{
    return c == '\n' || c == '\t' || c == '\r';
}


/** Determine if the string s represents a number */
bool StringUtilities::isNumber(const std::string& s) {

    for (size_t i=0; i<s.size(); i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
        
    }
    
    return true;
}


/**
 * Utility function for getting a one-line summary being max maxLen long.
 * We find the first non-empty line in the input. If it is longer than maxLen,
 * we truncate it at maxLen - 3 and add "..." at the end. If it is shorter, we
 * just return the complete line (without line break).
 */
std::string StringUtilities::oneLiner( const std::string& input, size_t maxLen )
{
    std::string oneLiner;
    
    std::ostringstream x;
    size_t begin = 0;
    size_t i = 0;
    for ( size_t i = 0; i < input.size(); ++i )
    {
        if ( input[ i ] == '\n' || input[ i ] == '\r' )
        {
            begin = i + 1;
        }
        
        if ( isgraph( input[ i ] ) )
        {
            break;
        }
        
    }
    size_t firstGraph = i;
    
    // Empty input; return empty oneliner
    if ( firstGraph >= input.size() )
    {
        return oneLiner;
    }

    // Construct the oneliner
    for ( i = begin; i < input.size() && i < maxLen; ++i )
    {
        if ( input[ i ] == '\n' || input[ i ] == '\r' )
        {
            break;
        }
        oneLiner.push_back( input[ i ] );
    }

    // If the oneliner was cut short by a newline, check whether there
    // are more printable graphs in the input. If so, we append dots.
    if ( input[i] == '\n' || input[i] == '\r' )
    {
        for ( ; i < input.size(); ++i )
        {
            if ( isgraph( input[i] ) )
                break;
        }
        
        if ( i < input.size() )
        {
            if ( maxLen - oneLiner.size() < 3 )
            {
                oneLiner[ maxLen - 1 ] = '.';
                oneLiner[ maxLen - 2 ] = '.';
                oneLiner[ maxLen - 3 ] = '.';
            }
            else
            {
                oneLiner += "...";
            }
        }
    }
    // If not, we check if the oneliner was cut short by the maximum length,
    // and, if so, insert dots into the last three character positions
    else if ( oneLiner.size() == maxLen )
    {
        oneLiner[ maxLen - 1 ] = '.';
        oneLiner[ maxLen - 2 ] = '.';
        oneLiner[ maxLen - 3 ] = '.';
    }

    return oneLiner;
}


/** Utility function for dividing string into pieces */
void StringUtilities::stringSplit(const std::string &s, const std::string &delim, std::vector<std::string>& results)
{
    
    // create our own copy of the string
    std::string str = s;

    size_t cutAt;
    while ( (cutAt = str.find_first_of(delim)) != str.npos )
    {
        if (cutAt > 0)
        {
            results.push_back(str.substr(0, cutAt));
        }
        else
        {
            results.push_back( "" );
        }
        str = str.substr(cutAt+1);
    }
    
    if (str.length() > 0)
    {
        results.push_back(str);
    }
    
}


/** Utility function for converting string to all lower case */
void StringUtilities::toLower(std::string& str)
{

    for (size_t i=0; i<str.size(); i++)
    {
        str[i] = char( tolower(str[i]) );
    }
    
}


/** Utility function for converting string to all lower case */
std::string& StringUtilities::firstCharToUpper(std::string& str)
{
    
    if ( str.size() > 0)
    {
        str[0] = char( toupper(str[0]) );
    }
    
    return str;
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




