//
//  XmlParser.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef XmlParser_H
#define XmlParser_H

#include "XmlDocument.h"
#include "XmlElementAttributed.h"
#include "XmlTag.h"

#include <iostream>
#include <string>

class XmlParser {
    
public:
    XmlParser();
    
    XmlDocument*            parse(const char* inStream);
    XmlDocument*            parse(std::string& inStream);
    XmlDocument*            parse(std::istream& inStream);
    
protected:
    
private:
    XmlTag                  readTag(std::istream& instream);
    XmlTag                  readObjectTag(std::istream& instream);
    XmlTag                  readTextTag(std::istream& instream);
    XmlElementAttributed*   readTagContents(std::istream& instream, const std::string& name, uintptr_t identifier);
    void                    skipWhiteSpace(std::istream& inStream);
    
    
    
};

#endif