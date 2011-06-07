//
//  XmlParser.cpp
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "XmlParser.h"

#include "XmlElement.h"
#include "XmlElementAttributed.h"
#include "XmlElementInstance.h"
#include "XmlElementReference.h"
#include "XmlElementTextNode.h"
#include "XmlTag.h"

#include <iostream>
#include <sstream>
#include <string>


XmlParser::XmlParser() {
    
}


XmlDocument* XmlParser::parse(const char* inStream) {
    // create a string-stream and throw the string into it
    std::istringstream ss (inStream);
     
    // parse with string-stream
    return parse(ss);
}

XmlDocument* XmlParser::parse(std::string& inStream) {
    // create a string-stream and throw the string into it
    std::stringstream ss (std::stringstream::in | std::stringstream::out);
    ss << inStream;
    
    // parse with string-stream
    return parse(ss);
}

XmlDocument* XmlParser::parse(std::istream &instream) {
    // create the XmlDocument
    XmlDocument* doc = new XmlDocument();
    
    // skip the initial white space
    skipWhiteSpace(instream);
    
    // iterate over all tag in the 
    while (instream.good() && !instream.eof()) {
        // read the first opening tag
        XmlTag      tag = readTag(instream);
        // test whether this tag is already closed, i.e. has "/>" at the end.
        if (tag.isClosed()) {
            // test whether this is a reference node
            if (tag.isReference()) {
                // this is a reference element -> create the object
                XmlElementReference* refNode = new XmlElementReference(tag.getName(),tag.getId());
                // add this element to the document on the top level
                doc->addXmlElement(refNode);
            }
            else {
                // this is a instance element -> create the object
                XmlElementInstance* instNode = new XmlElementInstance(tag.getName(),tag.getId());
                // set the attributes for this xml element
                for (std::map<std::string,std::string>::const_iterator it=tag.getAttributes().begin(); it!=tag.getAttributes().end(); ++it) {
                    instNode->setAttribute(it->first,it->second);
                }
                // add this element to the document on the top level
                doc->addXmlElement(instNode);
            }
        }
        else if (tag.isClosingTag()) {
            // this tag is a closing tag, i.e. "</name>"
            // so far we do nothing here
            // maybe we should check if the closing tag matches the last, not closed, opening tag
        }
        else {
            // the tag is neither closed nor a closing tag. this means there will follow some content, either further tags inside or simple text
            // read the contents of this tag
            XmlElementAttributed* element = readTagContents(instream,tag.getName(),tag.getId());
            // set the attributes for this xml element
            for (std::map<std::string,std::string>::const_iterator it=tag.getAttributes().begin(); it!=tag.getAttributes().end(); ++it) {
                element->setAttribute(it->first,it->second);
            }
            // add this element to the document on the top level
            doc->addXmlElement(element);
        }
        
        skipWhiteSpace(instream);
    }
    
    return doc;
}

XmlTag XmlParser::readTag(std::istream &instream) {
    // if the first conference is '<', then this function we read the instream until we found the first ">" which means the tag is closed
    // otherwise we assume it is a text/data tag
    
    // first we skip the white space
    skipWhiteSpace(instream);
    
    if (instream.peek() == '<') {
        return readObjectTag(instream);
    }
    else {
        return readTextTag(instream);
    }
}

XmlTag XmlParser::readObjectTag(std::istream &instream) {
    // we assume this is an object tag, hence this function we read the instream until we found the first ">" which means the tag is closed
    
    // create a string stream and add characters until ">" is found
    std::stringstream out;
    char c;
    do {
        c = instream.get();
        out << c;
    } while (c != '>');
    
    // create the tag object from the string
    XmlTag tag = XmlTag(out.str());
    
    return tag;
}

XmlTag XmlParser::readTextTag(std::istream &instream) {
    // we assume this is a text tag, hence this function we read the instream until we found the first "<" which means the tag is closed
    
    // create a string stream and add characters until ">" is found
    std::stringstream out;
    char c;
    // note: we do not want to remove the '<'
    while (instream.peek() != '<' && !instream.eof()) {
        c = instream.get();
        out << c;
    }
    
    // create the tag object from the string
    XmlTag tag = XmlTag(out.str());
    
    return tag;
}


XmlElementAttributed* XmlParser::readTagContents(std::istream &instream, const std::string& name, uintptr_t identifier) {
    // read the content tag
    XmlTag      tag = readTag(instream);
    // test whether this is a text tag
    if (tag.isTextTag()) {
        XmlElementTextNode* element = new XmlElementTextNode(name,identifier);
        element->setTextNode(tag.getText());
        return element;
    } else {
        // this tag is the content of an instance node
        XmlElementInstance* element = new XmlElementInstance(name,identifier);
        // iterate over all sub-elements
        while (!tag.isClosingTag()) {
            // test whether this tag is already closed, i.e. has "/>" at the end.
            if (tag.isClosed()) {
                // test whether this is a reference node
                if (tag.isReference()) {
                    // this is a reference element -> create the object
                    XmlElementReference* refNode = new XmlElementReference(tag.getName(),tag.getId());
                    // add this element to the document on the top level
                    element->addSubElement(refNode);
                }
                else {
                    // this is a instance element -> create the object
                    XmlElementInstance* instNode = new XmlElementInstance(tag.getName(),tag.getId());
                    // set the attributes for this xml element
                    for (std::map<std::string,std::string>::const_iterator it=tag.getAttributes().begin(); it!=tag.getAttributes().end(); ++it) {
                        instNode->setAttribute(it->first,it->second);
                    }
                    // add this element to the document on the top level
                    element->addSubElement(instNode);
                }
            }
            else {
                // the tag is neither closed nor a closing tag. this means there will follow some content, either further tags inside or simple text
                // read the contents of this tag
                XmlElementAttributed* subElement = readTagContents(instream,tag.getName(),tag.getId());
                // set the attributes for this xml element
                for (std::map<std::string,std::string>::const_iterator it=tag.getAttributes().begin(); it!=tag.getAttributes().end(); ++it) {
                    subElement->setAttribute(it->first,it->second);
                }
                // add this element to the document on the top level
                element->addSubElement(subElement);
            }
        }
        
        return element;
    }

}

void XmlParser::skipWhiteSpace(std::istream &inStream) {
    //
    while (isspace(inStream.peek())) {
        inStream.get();
    }
}