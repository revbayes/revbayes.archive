//
//  XmlElementTextNode.cpp
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "XmlElementTextNode.h"



XmlElementTextNode::XmlElementTextNode(const std::string& name, uintptr_t identifier) : XmlElementAttributed(name,identifier) {
    
}

void XmlElementTextNode::setTextNode(std::string const &value) {
    content = value;
    
}

void XmlElementTextNode::writeToStream(std::ostream &outStream, int indentLevel) const {
    
    indentStream(outStream,indentLevel);
    outStream << "<" << name << " ID=\"" << identifier << "\"";
    
    // output the attributes
    for (std::map<std::string,std::string>::const_iterator it=attributes.begin(); it!=attributes.end(); ++it) {
        outStream << " " << it->first << "=\"" << it->second << "\"";
    }
    
    // close the start tag
    outStream << ">";
    
    // write the content
    outStream << content;
    
    // set closing tag
    outStream << "</" << name << ">" << std::endl;
}