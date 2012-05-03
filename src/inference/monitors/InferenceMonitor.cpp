//
//  InferenceMonitor.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 5/2/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "InferenceDagNode.h"
#include "InferenceMonitor.h"
#include "RbLanguageObject.h"

InferenceMonitor::InferenceMonitor() {
    
}


InferenceMonitor::InferenceMonitor(const InferenceMonitor &i) : nodes( i.nodes ) {
    
    // set the print gen
    printgen = i.printgen;
    
    // clone the template objects
    for (std::vector<RbLanguageObject*>::const_iterator it = i.templateObject.begin(); it != i.templateObject.end(); ++it) {
        RbLanguageObject* tmp = (*it)->clone();
        templateObject.push_back( tmp );
    }
}


InferenceMonitor::~InferenceMonitor( void ) {
    
    // free the template objects
    for (std::vector<RbLanguageObject*>::iterator i = templateObject.begin(); i != templateObject.end(); ++i) {
        RbLanguageObject* tmp = *i;
        delete tmp;
    }
    templateObject.clear();
}


InferenceMonitor& InferenceMonitor::operator=(const InferenceMonitor &i) {
    
    // check for self-assignment
    if ( &i != this ) {
        
        // delete all my elements
        for (std::vector<RbLanguageObject*>::iterator i = templateObject.begin(); i != templateObject.end(); ++i) {
            RbLanguageObject* tmp = *i;
            delete tmp;
        }
        templateObject.clear();
        
        // set the nodes (we don't own them)
        nodes = i.nodes;
        
        // set the printgen
        printgen = i.printgen;
        
        // clone the template objects
        for (std::vector<RbLanguageObject*>::const_iterator it = i.templateObject.begin(); it != i.templateObject.end(); ++it) {
            RbLanguageObject* tmp = (*it)->clone();
            templateObject.push_back( tmp );
        }
    }
    
    return *this;
}



void InferenceMonitor::printValue(std::ostream &o) const {
    for (std::vector<InferenceDagNode *>::const_iterator i = nodes.begin(); i != nodes.end(); ++i) {
        if ( i != nodes.begin() ) {
            o << ", ";
        }
        o << long( (*i)->getValue().value );
    }
}


void InferenceMonitor::setArguments( const std::vector<InferenceDagNode *> &args) {
    nodes = args;
}


void InferenceMonitor::setTemplateObjects(const std::vector<RbLanguageObject *> t) {
    
    // free the template objects
    for (std::vector<RbLanguageObject*>::iterator i = templateObject.begin(); i != templateObject.end(); ++i) {
        RbLanguageObject* tmp = *i;
        delete tmp;
    }
    templateObject.clear();
    
    // reassign the template objects
    for (std::vector<RbLanguageObject*>::const_iterator i = t.begin(); i != t.end(); ++i) {
        RbLanguageObject* tmp = (*i);
        templateObject.push_back( tmp );
    }
}

