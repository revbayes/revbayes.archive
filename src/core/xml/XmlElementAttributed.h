/**
 * @file
 * This file contains the declaration of XmlElementAttributed, which implements an XmlElement, but is still abstract.
 * XmlElementAttributed is the base class of all xml elements which have attributes.
 * <person name="Sebastian"> is an xml element with the key-value attribute (name,Sebastian)
 *
 * @brief Declaration of the inferace XmlElementAttributed
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
#ifndef XmlElementAttributed_H
#define XmlElementAttributed_H

#include "XmlElement.h"

#include <map>
#include <string>
#include <vector>

class XmlElementAttributed : public XmlElement {
    
public:
    XmlElementAttributed(const std::string& name, uintptr_t identifier);
    
    std::string             getAttribute(const std::string& key) const;                                             //!< get the attribute with key
    bool                    getAttributeBool(const std::string& key) const;                                         //!< get the attribute with key
    double                  getAttributeDouble(const std::string& key) const;                                       //!< get the attribute with key
    int                     getAttributeInt(const std::string& key) const;                                          //!< get the attribute with key
    std::vector<bool>       getAttributeVectorBool(const std::string& key) const;                                   //!< get the attribute with key
    std::vector<double>     getAttributeVectorDouble(const std::string& key) const;                                 //!< get the attribute with key
    std::vector<int>        getAttributeVectorInt(const std::string& key) const;                                    //!< get the attribute with key
    
    void                    setAttribute(const std::string& name, const std::string& value);                        //!< Set an attribute for this node
    void                    setAttribute(const std::string& name, bool value);                                      //!< Set an attribute for this node
    void                    setAttribute(const std::string& name, double value);                                    //!< Set an attribute for this node
    void                    setAttribute(const std::string& name, int value);                                       //!< Set an attribute for this node
    void                    setAttribute(const std::string& name, const std::vector<bool>& value);                  //!< Set an attribute for this node
    void                    setAttribute(const std::string& name, const std::vector<double>& value);                //!< Set an attribute for this node
    void                    setAttribute(const std::string& name, const std::vector<int>& value);                   //!< Set an attribute for this node
    
    virtual void            writeToStream(std::ostream& outStream, int indentLevel=0) const = 0;                    //!< Print the xml element into the out stream
    
protected:
    
    std::map<std::string,std::string>   attributes;
    
private:
    std::vector<std::string>&           split(const std::string &s, char delim, std::vector<std::string> &elems) const ;
    std::vector<std::string>            split(const std::string &s, char delim) const ;
    
    
    
};

#endif