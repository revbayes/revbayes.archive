//
//  XmlDocument.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef XmlDocument_H
#define XmlDocument_H

//#include "XmlElement.h"

#include <map>
#include <string>
#include <vector>

class XmlElement;

class XmlDocument {
    
public:
    XmlDocument();
    
    void                                    addXmlElement(const XmlElement* element);
    const XmlElement*                       getFirstXmlElement();
    const XmlElement*                       getXmlElement(uintptr_t identifier);
    bool                                    hasXmlElementWithId(uintptr_t identifier);
    std::string                             print();
    
protected:
    
private:
    std::vector<const XmlElement*>          referencedElements;
    std::vector<const XmlElement*>          elements;
    
    std::map<uintptr_t,const XmlElement*>   entries;
    
    
};

#endif