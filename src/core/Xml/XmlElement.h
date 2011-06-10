//
//  XmlElement.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef XmlElement_H
#define XmlElement_H

#include <iostream>
#include <string>
#include <map>

class XmlElement {
    
public:
    
    uintptr_t           getId() const { return identifier; }
    
    virtual void        writeToStream(std::ostream& outStream, int indentLevel = 0) const = 0;
    
protected:
    XmlElement(const std::string& name, uintptr_t identifier);
    
    void                indentStream(std::ostream& outStream, int indentLevel) const;
    
    uintptr_t           identifier;
    std::string         name;

};

#endif