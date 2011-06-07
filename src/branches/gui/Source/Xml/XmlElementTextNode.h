//
//  XmlElementTextNode.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef XmlElementTextNode_H
#define XmlElementTextNode_H

#include "XmlElement.h"
#include "XmlElementAttributed.h"

#include <string>
#include <map>

class XmlElementTextNode : public XmlElementAttributed {
    
public:
    XmlElementTextNode(const std::string& name, uintptr_t identifier);
    
    void            setTextNode(const std::string& value);                                      //!< Setting this node as a text node. This will remove all sub-elements
    void            writeToStream(std::ostream& outStream, int indentLevel=0) const;            //!< Print the xml element into the out stream
    
protected:
    
private:
    
    std::string                         content;
    
};

#endif