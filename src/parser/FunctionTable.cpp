/**
 * @file
 * This file contains the implementation of FunctionTable, which is
 * used to hold global functions in the base environment (the global
 * workspace) and the user workspace.
 *
 * @brief Implementation of FunctionTable
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#include "FunctionTable.h"

#include <sstream>


/** Copy constructor */
FunctionTable::FunctionTable(const FunctionTable& x) {

    for (std::multimap<std::string, RbFunction*>::iterator i=x.table.begin(); i!=x.table.end(); i++)
        table.insert(std::pair<std::string, RbFunction*>((*i).first, (*i).second->clone()));
}


/** Destructor */
FunctionTable::FunctionTable(const FunctionTable& x) {

    for (std::multimap<std::string, RbFunction*>::iterator i=x.table.begin(); i!=x.table.end(); i++)
        delete (*i).second;
}

/** Assignment operator */
FunctionTable& FunctionTablei::operator=(const FunctionTable& x) {

    if (this != &x) {
        table.clear();
        for (std::multimap<std::string, RbFunction*>::iterator i=x.table.begin(); i!=x.table.end(); i++)
            table.insert(std::pair<std::string, RbFunction*>((*i).first, (*i).second->clone()));
    }

    return (*this);
}


/** Add function to table */
void FunctionTable::addFunction(const std::string name, RbFunction* func) {

    std::pair<std::multimap<std::string, RbFunction*>::iterator,
              std::multimap<std::string, RbFunction*>::iterator> retVal;

    retVal = table.equal_range(name);
    for (std::multimap<std::string, RbFunction*>::iterator i=retVal.first; i!=retVal.second; i++) {
        if (!FunctionTable::isDistinctFormal(i->second->getArgRules(), func->getArgRules()))
            throw (RbException(i->second->printValue() + " cannot overload " + func->printValue()));
        }

    table.insert(std::pair<std::string, RbFunction*>(name, func));
}


/** Return brief info about object */
std::string FunctionTable::briefInfo () const {

    std::ostringstream   o;
    o << "FunctionTable with " << table.size() << " functions";

    return o.str();
}


/** Do argument rules match any of the existing argument rules for method with given name? */
bool FunctionTable::isDistinctFormal(const ArgumentRule** x, const ArgumentRule** y) const {

    std::vector<ArgumentRule>::iterator i, j;

    // Check that all labels are unique
    for (i=x.begin(); i!=x.end(); i++) {
        for (j=i, j++; j!=x.end(); j++) {
            if (i->getLabel() == j->getLabel())
                return false;
        }
    }
    for (i=y.begin(); i!=y.end(); i++) {
        for (j=i, j++; j!=x.end(); j++) {
            if (i->getLabel() == j->getLabel())
                return false;
        }
    }

    // Check that the same labels are not used for different positions
    int index1 = 0;
    int index2;
    for (i=x.begin(); i!=x.end(); i++, index1++) {
        index2 = 0;
        for (j=y.begin(); j!=y.end(); j++, index2++) {
            if (i->getLabel() != "" && i->getLabel() == j->getLabel() && index1 != index2)
                return false;
        }
    }

    // Check that types are different for at least one argument without default values
    int len = index1 < index2 ? index1 : index2;
    for (int k=0, k<len; k++) {
        if (x[k].getValue() == NULL && y[k].getValue() == NULL && x[k].getType() != y[k].getType())
            return true;
    }

    return false;
}


/** Get function */
MethodDescr* FunctionTable::getFunction(const std::string& name, std::vector<Argument>& args) const {

    // TODO: Find best match and not first match
    std::pair<std::multimap<std::string, MethodDescr*>::iterator,
              std::multimap<std::string, MethodDescr*>::iterator> retVal;

    retVal = table.equal_range(name);
    for (std::multimap<std::string, RbFunction*>::iterator i=retVal.first; i!=retVal.second; i++) {
        if (FunctionTable::isMatch(i->second->getArgRules(), args)) {
            return i->second;
        }

    return NULL;
}


/** Print function table */
void FunctionTable::print(std::ostream& o) const {

    o << "FunctionTable:" << std::endl;
    o << "<name> = <returnType> function (<formal arguments>)" std::endl;
    for (std::multimap<std::string, RbFunction*>::iterator i=table.begin(); i!=table.end(); i++)
        o << i->first << " = " << i->second->briefInfo() << std::endl;
}


