/**
 * @file
 * This file contains the declaration of XmlElement, which is the base class of all types of xml elements.
 * All xml elements have an identifier and are able to printe themselves to a stream.
 
 * @brief Declaration of the inferace XmlElement
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
#ifndef XmlElement_H
#define XmlElement_H

#include <string>
#include <map>
#include <stdint.h>

class XmlElement {
    
public:
    
    uintptr_t           getId() const { return identifier; }                                                                        //!< get the unique id of the element
    virtual void        writeToStream(std::ostream& outStream, int indentLevel = 0) const = 0;                                      //!< print this element, and all its children recursively, to the stream
    
protected:
    XmlElement(const std::string& name, uintptr_t identifier);                                                                      //!< constructer, which denies instantiation of this interface
    
    void                indentStream(std::ostream& outStream, int indentLevel) const;                                               //!< helper function, which indents the stream to a given level
    
    uintptr_t           identifier;                                                                                                 //!< the unique identifier, could be the memeory address
    std::string         name;                                                                                                       //!< the name (or tag) of the elemen, e.g. <person "name=Sebastian"> has name 'person'
    
};

#endif
