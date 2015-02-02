//
//  XmlElementInstance.cpp
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "XmlElementInstance.h"

#include <map>
#include <string>
#include <ostream>



XmlElementInstance::XmlElementInstance(const std::string& name, uintptr_t identifier) : XmlElementAttributed(name, identifier) {
    
}

void XmlElementInstance::addSubElement(XmlElement* element) {
    
    subElements.push_back(element);
}

void XmlElementInstance::writeToStream(std::ostream &outStream, int indentLevel) const {
    
    indentStream(outStream,indentLevel);
    outStream << "<" << name << " ID=\"" << identifier << "\"";
    
    // output the attributes
    for (std::map<std::string,std::string>::const_iterator it=attributes.begin(); it!=attributes.end(); ++it) {
        outStream << " " << it->first << "=\"" << it->second << "\"";
    }
    
    // check if we have some sub-elements
    if ((int)subElements.size() > 0) {
        // close the start tag
        outStream << ">" << std::endl;
        
        // call writeToStream with indent level += 1 for all sub-elements
        for (std::vector<XmlElement*>::const_iterator it=subElements.begin(); it!=subElements.end(); ++it) {
            (*it)->writeToStream(outStream,indentLevel+1);
        }
        
        // indent the stream again before setting the closing tag
        indentStream(outStream,indentLevel);
        // set closing tag
        outStream << "</" << name << ">" << std::endl;
    } else {
        // close the start tag
        outStream << "/>" << std::endl;
    }
}