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


/** Do the arguments match the argument rules? */
bool FunctionTable::isMatch(const std::vector<ArgumentRule>& theRules, const std::vector<Argument>& theArgs) const {

    // TODO: Accomodate for ellipsis

    // Initialize some variables
    std::vector<bool> found = std::vector<bool>(x.size(),false);
    std::vector<bool> taken = std::vector<bool>(y.size(),false);
    std::vector<ArgumentRule>::iterator i;
    std::vector<Argument>::iterator     j;

    // Check that size is appropriate
    if (y.size() > x.size())
        return false;

    // Do exact matching of labels
    for(j=y.begin(); j!=y.end(); j++) {
        if (j->getLabel().size() == 0)
            continue;
        for (i=x.begin(); x!=y.end(); x++) {
            if (j->getLabel() == i->getLabel()) {
                if (j->getValue()->isType(i->getType())) {
                    found[int(i-x.begin())] = true;
                    taken[int(j-y.begin())] = true;
                }
                else
                    return false;
            }
        }
    }

    // Do partial matching of labels
    for(size_t j=0; j<y.size(); j++) {
        if (taken[j] || y[j].getLabel().size() == 0)
            continue;
        int nMatches = 0;
        for (size_t i=0; i<x.size(); i++) {
            if (found[i])
                continue;
            if (x[i].getLabel().compare(0, strlen(y[j].getLabel()), y[j].getLabel() == 0) {
                if (nMatches != 0)
                    return false;
                if (j->getValue()->isType(i->getType())) {
                    found[i] = true;
                    taken[j] = true;
                }
                else
                    return false;
            }
        }
    }

    // Match remaining ones in order
    size_t k = 0;
    for (size_t i=0; i<x.size(); i++) {
        if (found[i])
            continue;
        for (; k<y.size(); k++) {
            if (taken[k])
                continue;
            if (y[k].getValue()->isType(x[i].getType())) {
                found[i] = true;
                taken[k] = true;
                break;    
             }
             else
                 return false;
        }
        if (k==y.size())
            return false;
    }

    return true;
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


