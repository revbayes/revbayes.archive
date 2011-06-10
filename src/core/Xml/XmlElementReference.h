//
//  XmlElementReferece.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef XmlElementReference_H
#define XmlElementReference_H

#include "XmlElement.h"

#include <iostream>
#include <string>

class XmlElementReference : public XmlElement {
    
public:
    XmlElementReference(const std::string& name, uintptr_t identifier);
    
    void            writeToStream(std::ostream& outStream, int indentLevel=0) const;                      //!< Print the xml element into the out stream
    
};

#endif