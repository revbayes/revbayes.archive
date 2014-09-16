/**
 * @file
 * This file contains the declaration of XmlElementReference, which implements an XmlElement.
 * XmlElementReference only contain the reference ids to their true elements, so that a reconstructed element from the xml document
 * knows when an xml element is only a copy or if it is an actual reference to the same object.
 *
 *
 * @brief Declaration of the inferace XmlElementReference
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