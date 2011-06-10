//
//  XmlTag.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/14/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef XmlTag_H
#define XmlTag_H

#include <string>
#include <map>
#include <vector>

class XmlTag {
    
public:
    XmlTag(std::string str);
    
    std::map<std::string,std::string>&  getAttributes() { return attributes; }
    uintptr_t                           getId() { return identifier; }
    std::string&                        getName() { return name; }
    std::string                         getText() { return text; }
    
    bool                                isClosed() { return closed; }
    bool                                isClosingTag() { return closingTag; }
    bool                                isReference() { return reference; }
    bool                                isTextTag() { return textTag; }
    
protected:
    
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