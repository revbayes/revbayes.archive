//
//  XmlElementReferece.cpp
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "XmlElementReference.h"

XmlElementReference::XmlElementReference(const std::string& name, uintptr_t indentifier) : XmlElement(name,identifier) {
    
}

void XmlElementReference::writeToStream(std::ostream &outStream, int indentLevel) const {
    indentStream(outStream,indentLevel);
    
    outStream << "<" << name << " IDREF=\"" << identifier << "\" />";
}