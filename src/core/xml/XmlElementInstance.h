/**
 * @file
 * This file contains the declaration of XmlElementInstance, which implements an XmlElementAttributed.
 * XmlElementInstance is the implementation of xml elements which have attributes and can contain children
 * but no plain text as its body.
 * Example which could be respresented as attributes too instead of child nodes:
 * <person><name>Sebastian</name></person>
 *
 * There is currently no general guideline when to use attributes and when to use child node. Sebastian recomments,
 * that plain elements should be attributes and complex elements should be child nodes. Plain elements are elements which can be represented by a simple string,
 * complex elements instead have some strucute which might be variable.
 *
 * @brief Declaration of the inferace XmlElementInstance
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author Sebastian Hoehna
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id$
 */
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
    
    void            addSubElement(XmlElement* element);                                         //!< Add a sub-element
    void            writeToStream(std::ostream& outStream, int indentLevel=0) const;            //!< Print the xml element into the out stream
    
protected:
    
private:
    
    std::vector<XmlElement*>      subElements;
    
    
    
};

#endif