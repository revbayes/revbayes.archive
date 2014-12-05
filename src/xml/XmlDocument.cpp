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

XmlDocument::XmlDocument()
{
    
}


/**
 * WARNING: We assume that there is no element with this id in the document.
 * If there is such an element, its pointer in out hash map will be overwritten!
 *
 */
void XmlDocument::addXmlElement(XmlElement* element)
{
    elements.push_back(element);
    entries.insert(std::pair<uintptr_t, XmlElement*>(element->getId(),element) );
}

const XmlElement* XmlDocument::getFirstXmlElement() const
{

    return *(elements.begin());
}

const XmlElement* XmlDocument::getXmlElement(uintptr_t identifier) const
{
    return entries.find(identifier)->second;
}

bool XmlDocument::hasXmlElementWithId(uintptr_t identifier) const
{
    return entries.find(identifier) != entries.end();
}

std::string XmlDocument::print()
{
    // create a stream for the output
    std::stringstream out;
    // write every element into the stream
    for (std::vector<XmlElement*>::const_iterator it=elements.begin(); it!=elements.end(); it++)
    {
        (*it)->writeToStream(out);
        out << std::endl;
    }
    
    return out.str();
}