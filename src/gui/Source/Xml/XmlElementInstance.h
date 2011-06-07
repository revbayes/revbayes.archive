//
//  XmlElementInstance.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef XmlElementInstance_H
#define XmlElementInstance_H

#include "XmlElement.h"
#include "XmlElementAttributed.h"

#include <string>
#include <map>
#include <vector>

class XmlElementInstance : public XmlElementAttributed {
    
public:
    XmlElementInstance(const std::string& name, uintptr_t identifier);
    
    void            addSubElement(const XmlElement* element);                                   //!< Add a sub-element
    void            writeToStream(std::ostream& outStream, int indentLevel=0) const;            //!< Print the xml element into the out stream
    
protected:
    
private:
    
    std::vector<const XmlElement*>      subElements;
    
    
    
};

#endif