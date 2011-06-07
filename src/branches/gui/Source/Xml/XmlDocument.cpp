//
//  XmlDocument.cpp
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "XmlDocument.h"

#include "XmlElementInstance.h"

#include <sstream>

XmlDocument::XmlDocument() {
    
}

void XmlDocument::addXmlElement(const XmlElement *element) {
    elements.push_back(element);
    entries[element->getId()] = element;
}

const XmlElement* XmlDocument::getFirstXmlElement() {
    return *(elements.begin());
}

const XmlElement* XmlDocument::getXmlElement(uintptr_t identifier){
    return entries.find(identifier)->second;
}

bool XmlDocument::hasXmlElementWithId(uintptr_t identifier) {
    return entries.find(identifier) != entries.end();
}

std::string XmlDocument::print() {
    // create a stream for the output
    std::stringstream out;
    // write every element into the stream
    for (std::vector<const XmlElement*>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
        (*it)->writeToStream(out);
        out << std::endl;
    }
    
    return out.str();
}