/**
 * @file
 * This file contains the implementation of SymbolTable, which is
 * used to hold symbol tables.
 *
 * @brief Implementation of SymbolTable
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @package parser
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#include "SymbolTable.h"


/** Destructor deletes functions and variables */
SymbolTable::~SymbolTable() {

    for(map<const string, DAGNode *>::iterator i = varTable.begin(); i!=varTable.end(); i++) {
        if (i->second->getChildrenNodes().empty())
            delete i->second;
    }
    for(map<const string, RbFunction *>::iterator i = functionTable.begin(); i!=functionTable.end(); i++) {
        delete i->second;
    }
}

/** Add a function */
bool SymbolTable::add(const string name, RbFunction *entry) {
    
    if (existsFunction(name)) {
        cerr << "Function '" << name << "' already exist";
        return false;
    }
    if (existsVariable(name)) {
        cerr << "The symbol '" << name << "' refers to a variable";
        return false;
    }

    functionTable.insert(pair<const string, RbFunction *>(name,entry));
}

/** Add a variable */
bool SymbolTable::add(const string name, DAGNode *entry) {
    
    if (existsVariable(name)) {
        cerr << "Variable '" << name << "' already exist";
        return false;
    }
    if (existsFunction(name)) {
        cerr << "The symbol '" << name << "' refers to a function";
        return false;
    }

    varTable.insert(pair<const string, DAGNode *>(name,entry));
}

/** Add a function (C-style) */
bool SymbolTable::add(const char* name, RbFunction *entry) {
    
    return add(std::string(name), entry);
}

/** Add a variable (C-style) */
bool SymbolTable::add(const char* name, DAGNode *entry) {
    
    return add(std::string(name), entry);
}

/** Erase a function */
void SymbolTable::eraseFunction(const string& name) {

    map<const string, RbFunction *>::iterator i;
    i = functionTable.find(name);
    if(i != functionTable.end())
        functionTable.erase(i);
}

/** Erase a variable */
void SymbolTable::eraseVariable(const string& name) {

    map<const string, DAGNode *>::iterator i;
    i = varTable.find(name);
    if(i != varTable.end())
        varTable.erase(i);
}

/** Check if function exists
bool SymbolTable::existsFunction(const string &name) const {

    map<const string, RbFunction *>::const_iterator i;
    i = functionTable.find(name);
    if(i == functionTable.end())
        return false;
    else
        return true;
}

/** Check if variable exists */
bool SymbolTable::existsVariable(const string &name) const {

    map<const string, DAGNode *>::const_iterator i;
    i = varTable.find(name);
    if(i == varTable.end())
        return false;
    else
        return true;
}

/** Get function */
RbFunction *SymbolTable::getFunction(string &name) {

    if (existsFunction(name))
        return functionTable.find(name)->second;
    else
        return NULL;
}

/** Get variable */
DAGNode *SymbolTable::getVariable(string &name) {

    if (existsVariable(name))
        return varTable.find(name)->second;
    else
        return NULL;
}

/** Print symbol table */
void SymbolTable::print(std::ostream &c) const {

    int count;

    c << "Function table:" << std::endl;
    count=1;
    for(map<const string, RbFunction *>::const_iterator i=functionTable.begin(); i!=functionTable.end(); i++) {
        c << count++ << ". Key=" << (*i).first << " Value=";
        (*i).second->print(c);
        c << "\n";
    }

    c << "Variable table:" << std::endl;
    count=1;
    for(map<const string, DAGNode *>::const_iterator i=varTable.begin(); i!=varTable.end(); i++) {
        c << count++ << ". Key=" << (*i).first << " Value=";
        (*i).second->print(c);
        c << "\n";
    }
}
