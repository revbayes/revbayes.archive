//
//  RbObject.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef RbObject_H
#define RbObject_H

#include "XmlDocument.h"
#include "XmlElement.h"

#include <string>

class RbObject {
    
public:
    RbObject();
    
    virtual const XmlElement*     encode(XmlDocument* doc, const std::string& name) = 0;                                        //!< function to encode this object into an XML string
    
protected:
    
private:
    
    
    
};

#endif