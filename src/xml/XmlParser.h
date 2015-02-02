/**
 * @file
 * This file contains the declaration of XmlParser. The XmlParser reads the content of an xml file from a stream
 * and creates from it the xml document.
 *
 * @brief Declaration of the inferace XmlParser
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author Sebastian Hoehna
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id$
 */
#ifndef XmlParser_H
#define XmlParser_H

#include "XmlDocument.h"
#include "XmlElementAttributed.h"
#include "XmlTag.h"

#include <iostream>
#include <string>

class XmlParser {
    
public:
    XmlParser();                                                                                                                        //!< default constructor
    
    XmlDocument*                    parse(const char* inStream);                                                                        //!< parser with c-type input string
    XmlDocument*                    parse(std::string& inStream);                                                                       //!< parser reading from string
    XmlDocument*                    parse(std::istream& inStream);                                                                      //!< parser reading from input stream
    
    
private:
    XmlTag                          readTag(std::istream& instream);
    XmlTag                          readObjectTag(std::istream& instream);
    XmlTag                          readTextTag(std::istream& instream);
    XmlElementAttributed*           readTagContents(std::istream& instream, const std::string& name, uintptr_t identifier);
    void                            skipWhiteSpace(std::istream& inStream);
    
    
    
};

#endif