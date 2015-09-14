//
//  XmlElement.cpp
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "XmlElement.h"



XmlElement::XmlElement(const std::string& name, uintptr_t identifier) {
    this->name          = name;
    this->identifier    = identifier;
}

void XmlElement::indentStream(std::ostream &outStream, int indentLevel) const {
    for (int i=0; i<indentLevel; i++) {
        outStream << "\t";
    }
}