/**
 * @file
 * This file contains the declaration of ArgumentRules, which is
 * is the container for all argument rules. We need a special object
 * for proper memory allocation and copying of the objects.
 *
 * @brief Declaration of ArgumentRules
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-02-13, version 1.0
 *
 * $Id$
 */

#include "ArgumentRules.h"
#include "ArgumentRule.h"
#include "VectorString.h"


// Definition of the static type spec member
const TypeSpec ArgumentRules::typeSpec(ArgumentRules_name);

ArgumentRules::ArgumentRules(void) {
    
}

ArgumentRules::ArgumentRules(const ArgumentRules& ar) {
    
    // get a copy of the argument rules
    for (std::vector<ArgumentRule*>::const_iterator it = ar.rules.begin(); it != ar.rules.end(); it++) {
        rules.push_back( (*it)->clone() );
    }
}


ArgumentRules::~ArgumentRules(void) {
    clear();
}


std::vector<ArgumentRule*>::iterator ArgumentRules::begin(void) {
    return rules.begin();
}


std::vector<ArgumentRule*>::const_iterator ArgumentRules::begin(void) const {
    return rules.begin();
}


ArgumentRules& ArgumentRules::operator=(const ArgumentRules &a) {
    
    if (this != &a) {
        // first we clear ourselves
        clear();
        
        // get a copy of the argument rules
        for (std::vector<ArgumentRule*>::const_iterator it = a.rules.begin(); it != a.rules.end(); it++) {
            rules.push_back( (*it)->clone() );
        }
    }
    
    return (*this);
}


const ArgumentRule& ArgumentRules::operator[](size_t index) const {
    return *rules[index];
}


ArgumentRule& ArgumentRules::operator[](size_t index) {
    return *rules[index];
}


void ArgumentRules::clear(void) {
    // delete the argument rules
    for (std::vector<ArgumentRule*>::iterator it = rules.begin(); it != rules.end(); it++) {
        ArgumentRule* theRule = (*it);
        delete theRule;
    }
    rules.clear();
}

std::vector<ArgumentRule*>::iterator ArgumentRules::end(void) {
    return rules.end();
}


std::vector<ArgumentRule*>::const_iterator ArgumentRules::end(void) const {
    return rules.end();
}


/** Get class vector describing type of object */
const VectorString& ArgumentRules::getClass(void) const { 
    
    static VectorString rbClass = VectorString(ArgumentRules_name) + RbInternal::getClass();
	return rbClass; 
}



/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& ArgumentRules::getTypeSpec(void) const {
    return typeSpec;
}


void ArgumentRules::insert(std::vector<ArgumentRule *>::iterator it, ArgumentRule *rule) {
    rules.insert(it, rule);
}


void ArgumentRules::insert(std::vector<ArgumentRule *>::iterator it, std::vector<ArgumentRule *>::const_iterator first, std::vector<ArgumentRule *>::const_iterator last) {
    
    for (std::vector<ArgumentRule *>::const_iterator org = first; org != last; org++) {
        rules.insert(it, (*org)->clone());
    }
}


/** Push back a new rule. We just delegate it to our member variable. */
void ArgumentRules::push_back(ArgumentRule *rule) {
    rules.push_back(rule);
}


/** Print value for user (in descriptions of functions, for instance) */
void ArgumentRules::printValue(std::ostream &o) const {
    
    // print the argument rules
    for (std::vector<ArgumentRule*>::const_iterator it = rules.begin(); it != rules.end(); it++) {
        ArgumentRule* theRule = (*it);
        theRule->printValue(o);
        o << std::endl;
    }
}


/** Get the size of the vector. We delegate to the member. */
size_t ArgumentRules::size(void) const {
    return rules.size();
}


