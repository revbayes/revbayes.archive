//
//  XmlElementAttributed.cpp
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/14/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "XmlElementAttributed.h"

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdlib>
//#include <stdio.h>

XmlElementAttributed::XmlElementAttributed(const std::string& name, uintptr_t identifier) : XmlElement(name,identifier) {
    
}

std::string XmlElementAttributed::getAttribute(std::string const &key) const {
    return attributes.find(key)->second;
}

bool XmlElementAttributed::getAttributeBool(std::string const &key) const {
    std::string value = attributes.find(key)->second;
    
    return value.compare("1") == 0;
}

double XmlElementAttributed::getAttributeDouble(std::string const &key) const {
    std::string value = attributes.find(key)->second;
    
    return atof(value.c_str());
}

int XmlElementAttributed::getAttributeInt(std::string const &key) const {
    std::string value = attributes.find(key)->second;
    
    return atoi(value.c_str());
}

std::vector<bool> XmlElementAttributed::getAttributeVectorBool(std::string const &key) const {
    std::string value = attributes.find(key)->second;
    
    // remove the surounding pair of '{' and '}'
    value.erase(0,1);
    value.erase(value.size()-1,1);
    
    // split the values into a vector
    const std::vector<std::string>& tokens = split(value, ';');
    
    // loop over the values and transform them
    std::vector<bool> bools;
    for (std::vector<std::string>::const_iterator it=tokens.begin(); it!=tokens.end(); ++it) {
        bools.push_back(it->compare("1") == 0);
    }
    
    return bools;
}

std::vector<double> XmlElementAttributed::getAttributeVectorDouble(std::string const &key) const {
    std::string value = attributes.find(key)->second;
    
    // remove the surounding pair of '{' and '}'
    value.erase(0,1);
    value.erase(value.size()-1,1);
    
    // split the values into a vector
    const std::vector<std::string>& tokens = split(value, ';');
    
    // loop over the values and transform them
    std::vector<double> doubles;
    for (std::vector<std::string>::const_iterator it=tokens.begin(); it!=tokens.end(); ++it) {
        doubles.push_back(atof(it->c_str()));
    }
    
    return doubles;
}

std::vector<int> XmlElementAttributed::getAttributeVectorInt(std::string const &key) const {
    std::string value = attributes.find(key)->second;
    
    // remove the surounding pair of '{' and '}'
    value.erase(0,1);
    value.erase(value.size()-1,1);
    
    // split the values into a vector
    const std::vector<std::string>& tokens = split(value, ';');
    
    // loop over the values and transform them
    std::vector<int> ints;
    for (std::vector<std::string>::const_iterator it=tokens.begin(); it!=tokens.end(); ++it) {
        ints.push_back(atoi(it->c_str()));
    }
    
    return ints;
}

void XmlElementAttributed::setAttribute(std::string const &name, std::string const &value) {
    attributes[name]    = value;
}

void XmlElementAttributed::setAttribute(std::string const &key, bool value) {
    attributes[key]    = (value ? "1" : "0");
}

void XmlElementAttributed::setAttribute(std::string const &key, double value) {
    std::stringstream out;
    out << value;
    
    attributes[key]    = out.str();
}

void XmlElementAttributed::setAttribute(std::string const &key, int value) {
    std::stringstream out;
    out << value;
    
    attributes[key]    = out.str();
}

void XmlElementAttributed::setAttribute(std::string const &key, const std::vector<bool>& value) {
    std::stringstream out;
    out << "{";
    std::vector<bool>::const_iterator it = value.begin();
    while (it != value.end()) {
        char tmp = ((*it) ? '1' : '0');
        ++it;
        if (it != value.end()) {
            out << tmp << ";";
        }
        else {
            out << tmp << "}";
        }
    }
    
    attributes[key]    = out.str();
}

void XmlElementAttributed::setAttribute(std::string const &key, const std::vector<double>& value) {
    std::stringstream out;
    out << "{";
    std::vector<double>::const_iterator it = value.begin();
    while (it != value.end()) {
        double tmp = (*it);
        ++it;
        if (it != value.end()) {
            out << tmp << ";";
        }
        else {
            out << tmp << "}";
        }
    }
    
    attributes[key]    = out.str();
}

void XmlElementAttributed::setAttribute(std::string const &key, const std::vector<int>& value) {
    std::stringstream out;
    out << "{";
    std::vector<int>::const_iterator it = value.begin();
    while (it != value.end()) {
        int tmp = (*it);
        ++it;
        if (it != value.end()) {
            out << tmp << ";";
        }
        else {
            out << tmp << "}";
        }
    }
    
    attributes[key]    = out.str();
}


std::vector<std::string>& XmlElementAttributed::split(const std::string &s, char delim, std::vector<std::string> &elems) const {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> XmlElementAttributed::split(const std::string &s, char delim) const {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}
