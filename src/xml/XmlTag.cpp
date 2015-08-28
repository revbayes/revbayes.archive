//
//  XmlTag.cpp
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/14/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "XmlTag.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>


XmlTag::XmlTag(std::string content) {
    identifier = 0;
    
    // we assume that an object tag starts with '<' and ends with '>'
    char firstChar = content[0];
    if (firstChar == '<')
        createObjectTag(content);
    else
        createTextTag(content);
}


void XmlTag::createObjectTag(std::string &content) {
    
    textTag = false;
    closingTag = false;
    closed = false;
    reference = false;
    
    // remove first and last element, i.e. '<' and '>'
    content.erase(0,1);
    content.erase(content.size()-1,1);
    
    // test whether the second char is '/', i.e. it is a closing tag
    if (content[0] == '/')
    {
        closingTag = true;
        // we need to do nothing otherwise because the closing tags have no information so far
    }
    else
    {
        // test whether the last char is '/'
        if (content[content.size()-1] == '/')
        {
            // set the flag for this tag that it is closed
            closed = true;
        }
        
        // split the content into tokens which are separated by ' '
        // WARNING: I assume that no blanks are used in names or values
        const std::vector<std::string>& tokens = split(content, ' ');
        
        // get an iterator and iterate over the attributes
        std::vector<std::string>::const_iterator it = tokens.begin();
        // the first attribute is always the name
        name = (*it);
        ++it;
        for (;it!=tokens.end();++it) {
            // every token should be a key-value pair
            const std::vector<std::string>& key_value = split((*it), '=');
            // remove the '"' around the values
            std::string value = key_value.at(1);
            value.erase(0,1);
            value.erase(value.size()-1,1);
            if (key_value.at(0).compare("ID") == 0) {
                int tmp = atoi(value.c_str());
                identifier = (uintptr_t)tmp;
                reference = false;
                continue;
            }
            
            if (key_value.at(0).compare("IDREF") == 0) {
                identifier = (uintptr_t)atoi(value.c_str());
                reference = true;
                continue;
            }
            
            // set this attribute
            attributes[key_value.at(0)]    = value;
        }
    }
}

void XmlTag::createTextTag(std::string &content) {
    
    text = content;
    textTag = true;
    closed  = true;
    closingTag = false;
    reference = false;
    name = "";
    identifier = 0;
}

std::vector<std::string>& XmlTag::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> XmlTag::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}