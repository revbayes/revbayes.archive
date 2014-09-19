/**
 * @file
 * This file contains the declaration of XmlElementTextNode, which implements an XmlElementAttributed.
 * XmlElementInstance is the the implementation of xml elements which have attributes and can contain text
 * but no children as its body.
 * Example:
 * <name>Sebastian</name>
 *
 * There is currently no general guideline when to use attributes and when to use child node. Sebastian recomments,
 * that plain elements should be attributes and complex elements should be child nodes. Plain elements are elements which can be represented by a simple string,
 * complex elements instead have some strucute which might be variable.
 *
 * @brief Declaration of the inferace XmlElementTextNode
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
#ifndef XmlElementTextNode_H
#define XmlElementTextNode_H

#include "XmlElement.h"
#include "XmlElementAttributed.h"

#include <string>
#include <map>

class XmlElementTextNode : public XmlElementAttributed {
    
public:
    XmlElementTextNode(const std::string& name, uintptr_t identifier);
    
    
    std::string&    getContent() { return content; }                                            //!< Get the content
    void            setTextNode(const std::string& value);                                      //!< Setting this node as a text node.
    void            writeToStream(std::ostream& outStream, int indentLevel=0) const;            //!< Print the xml element into the out stream
    
private:
    
    std::string                         content;                                                //!< the content of the text node, i.e. the plain text string
    
};

#endif