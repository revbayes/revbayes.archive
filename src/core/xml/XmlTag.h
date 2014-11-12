/**
 * @file
 * This file contains the declaration of XmlTag, which is the declaration of xml elements read from a text.
 * The XmlTag class is very important for the XmlParser and is the actual workhorse, since it breaks a tag like
 * <person id="1234567890" name="Sebastian"> into its informative elements and attributes.
 *
 * @brief Declaration of the inferace XmlTag
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
#ifndef XmlTag_H
#define XmlTag_H

#include <string>
#include <map>
#include <vector>
#include <stdint.h>

class XmlTag {
    
public:
    XmlTag(std::string str);
    
    std::map<std::string,std::string>&  getAttributes() { return attributes; }                                              //!< get all the attributes
    uintptr_t                           getId() { return identifier; }                                                      //!< get the id
    std::string&                        getName() { return name; }                                                          //!< get the name of this tag (first string after '<')
    std::string                         getText() { return text; }                                                          //!< get the text
    
    bool                                isClosed() { return closed; }
    bool                                isClosingTag() { return closingTag; }
    bool                                isReference() { return reference; }
    bool                                isTextTag() { return textTag; }
    
private:
    
    void                                createObjectTag(std::string& content);
    void                                createTextTag(std::string& content);
    std::vector<std::string>&           split(const std::string &s, char delim, std::vector<std::string> &elems);
    std::vector<std::string>            split(const std::string &s, char delim);
    
    std::map<std::string,std::string>   attributes;
    bool                                closed;
    bool                                closingTag;
    uintptr_t                           identifier;
    std::string                         name;
    bool                                reference;
    std::string                         text;
    bool                                textTag;
    
};

#endif