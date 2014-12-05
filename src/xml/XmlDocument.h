/**
 * @file
 * This file contains the declaration of XmlDocument, which represents a xml file
 * from the file system in the computer memory. The document holds the complete
 * xml trees and is responsible for operations to retieve and add xml elements
 * to a tree.
 * Using the XmlDocument to add xml elements enables the usage of reference, where the
 * xml element is created on first occurrence with a unique id, and for every later occurrence
 * only a reference is inserted in the tree.
 *
 * @brief Declaration of XmlDocument
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
#ifndef XmlDocument_H
#define XmlDocument_H

#include "XmlElement.h"
#include <map>
#include <string>
#include <vector>
#include <stdint.h>


class XmlDocument {
    
public:
    XmlDocument();
    
    void                                    addXmlElement(XmlElement* element);                                     //!< add a root xml element
    const XmlElement*                       getFirstXmlElement() const;                                             //!< get the first element of the document
    int                                     getNumberOfElements() const { return (int)elements.size(); }
    const XmlElement*                       getXmlElement(uintptr_t identifier) const;                              //!< get the element with identifier
    bool                                    hasXmlElementWithId(uintptr_t identifier) const;                        //!< is there already a
    std::string                             print();
    
protected:
    
private:
    std::vector<XmlElement*>                referencedElements;
    std::vector<XmlElement*>                elements;                                                               //!< the elements contained in this document
    
    std::map<uintptr_t, XmlElement*>        entries;                                                                //!< the map of all elements to their ids
    
    
};

#endif
