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


/**
 * WARNING: We assume that there is no element with this id in the document. 
 * If there is such an element, its pointer in out hash map will be overwritten!
 *
 */
void XmlDocument::addXmlElement(const RbPtr<XmlElement> element) {
    elements.push_back(element);
    entries.insert(std::pair<uintptr_t, const RbPtr<XmlElement> >(element->getId(),element) );
}

RbPtr<const XmlElement> XmlDocument::getFirstXmlElement() const {
    return RbPtr<const XmlElement>( *(elements.begin()) );
}

RbPtr<const XmlElement> XmlDocument::getXmlElement(uintptr_t identifier) const {
    return RbPtr<const XmlElement>( entries.find(identifier)->second );
}

bool XmlDocument::hasXmlElementWithId(uintptr_t identifier) const {
    return entries.find(identifier) != entries.end();
}

std::string XmlDocument::print() {
    // create a stream for the output
    std::stringstream out;
    // write every element into the stream
    for (std::vector<RbPtr<XmlElement> >::const_iterator it=elements.begin(); it!=elements.end(); it++) {
        (*it)->writeToStream(out);
        out << std::endl;
    }
    
    return out.str();
}