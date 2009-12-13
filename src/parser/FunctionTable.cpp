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

#include "ArgumentRule.h"
#include "FunctionTable.h"
#include "IntVector.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbNames.h"

#include <sstream>


/** Basic constructor, empty table with or without parent */
FunctionTable::FunctionTable(FunctionTable* parent)
    : RbInternal(), table(), parentTable(parent) {
}


/** Copy constructor */
FunctionTable::FunctionTable(const FunctionTable& x) {

    for (std::multimap<std::string, RbFunction*>::const_iterator i=x.table.begin(); i!=x.table.end(); i++)
        table.insert(std::pair<std::string, RbFunction*>((*i).first, (RbFunction*)((*i).second->clone())));

    parentTable = x.parentTable;
}


/** Destructor */
FunctionTable::~FunctionTable(void) {

    for (std::multimap<std::string, RbFunction*>::iterator i=table.begin(); i!=table.end(); i++)
        delete (*i).second;
}

/** Assignment operator */
FunctionTable& FunctionTable::operator=(const FunctionTable& x) {

    if (this != &x) {

        table.clear();
        for (std::multimap<std::string, RbFunction*>::const_iterator i=x.table.begin(); i!=x.table.end(); i++)
            table.insert(std::pair<std::string, RbFunction*>((*i).first, (RbFunction*)((*i).second->clone())));

        parentTable = x.parentTable;
    }

    return (*this);
}


/** Add function to table */
void FunctionTable::addFunction(const std::string name, RbFunction* func) {

    std::pair<std::multimap<std::string, RbFunction*>::iterator,
              std::multimap<std::string, RbFunction*>::iterator> retVal;

    retVal = table.equal_range(name);
    for (std::multimap<std::string, RbFunction*>::iterator i=retVal.first; i!=retVal.second; i++) {
        if (!FunctionTable::isDistinctFormal(i->second->getArgumentRules(), func->getArgumentRules())) {
            std::ostringstream msg;
            i->second->printValue(msg);
            msg << " cannot overload ";
            func->printValue(msg);
            throw RbException(msg.str());
        }
    }
    table.insert(std::pair<std::string, RbFunction*>(name, func));
}


/** Return brief info about object */
std::string FunctionTable::briefInfo () const {

    std::ostringstream   o;
    o << "FunctionTable with " << table.size() << " functions";

    return o.str();
}


/** Erase function but not in parent frame */
void FunctionTable::eraseFunction(const std::string& name) {

    std::pair<std::multimap<std::string, RbFunction*>::iterator,
              std::multimap<std::string, RbFunction*>::iterator> retVal;

    retVal = table.equal_range(name);
    if (retVal.first == table.end()) {
        if (parentTable != NULL)
            parentTable->eraseFunction(name);
        else
            throw RbException("No function named '"+ name + "'");
    }

    if (parentTable != NULL)
        table.erase(retVal.first, retVal.second);
    else
        throw RbException("Functions in base environment cannot be erased");
}


/** Execute function */
const RbObject* FunctionTable::executeFunction(const std::string& name, const std::vector<Argument>& args) const {

    std::pair<std::multimap<std::string, RbFunction*>::const_iterator,
              std::multimap<std::string, RbFunction*>::const_iterator> retVal;

    retVal = table.equal_range(name);
    if (retVal.first == table.end()) {
        if (parentTable != NULL)
            return parentTable->executeFunction(name, args);
        else
            throw RbException("No function named '"+ name + "'");
    }
    else if (retVal.first == retVal.second) {
        return ((RbFunction*)(retVal.first->second))->execute(args);
    }
    else {
        IntVector matchScore, bestScore;
        RbFunction* bestMatch = NULL;

        for (std::multimap<std::string, RbFunction*>::const_iterator i=retVal.first; i!=retVal.second; i++) {
            if ( (*i).second->processArguments(args, &matchScore) == true ) {
                if ( bestMatch == NULL ) {
                    bestScore = matchScore;
                    bestMatch = i->second;
                }
                else {
                    size_t j;
                    for (j=0; j<matchScore.size() && j<bestScore.size(); j++) {
                        if (matchScore[j] < bestScore[j]) {
                            bestScore = matchScore;
                            bestMatch = i->second;
                        }
                        else if (matchScore[j] > bestScore[j])
                            break;
                    }
                    if (j==matchScore.size() || j==bestScore.size())
                        throw RbException("Ambiguous call to function '" + name + "'");
                }
            }
        }
        if ( bestMatch == NULL )
            throw RbException("No overloaded function '" + name + "' matches arguments");
        else
            return bestMatch->execute();    // Arguments are already processed!
    }
}


/** Get function */
RbFunction* FunctionTable::getFunction(const std::string& name, const std::vector<Argument>& args) const {

    std::pair<std::multimap<std::string, RbFunction*>::const_iterator,
              std::multimap<std::string, RbFunction*>::const_iterator> retVal;

    retVal = table.equal_range(name);
    if (retVal.first == table.end()) {
        if (parentTable != NULL)
            return parentTable->getFunction(name, args);
        else
            throw RbException("No function named '"+ name + "'");
    }
    else if (retVal.first == retVal.second) {
        RbFunction* func = (RbFunction*)(retVal.first->second->clone());
        func->processArguments(args);
        return func;
    }
    else {
        IntVector matchScore, bestScore;
        const RbFunction* bestMatch = NULL;
        bool foundFirst = false;

        for (std::multimap<std::string, RbFunction*>::const_iterator i=retVal.first; i!=retVal.second; i++) {
            if ( i->second->processArguments(args, &matchScore) ) {
                if ( foundFirst == false ) {
                    foundFirst = true;
                    bestScore = matchScore;
                    bestMatch = i->second;
                }
                else {
                    size_t j;
                    for (j=0; j<matchScore.size() && j<bestScore.size(); j++) {
                        if (matchScore[j] < bestScore[j]) {
                            bestScore = matchScore;
                            bestMatch = i->second;
                        }
                        else if (matchScore[j] > bestScore[j])
                            break;
                    }
                    if (j==matchScore.size() || j==bestScore.size())
                        throw RbException("Ambiguous call to function '" + name + "'");
                }
            }
        }
        if ( bestMatch == NULL )
            throw RbException("No overloaded function '" + name + "' matches arguments");

        RbFunction* func = (RbFunction*)(bestMatch->clone());
        func->processArguments(args);       // Function clone does not have processed arguments
        return func;
    }
}


/** Check if two formals are unique */
bool FunctionTable::isDistinctFormal(const ArgumentRule** x, const ArgumentRule** y) {

    /* Check that all labels are unique in both sets of argument rules */
    for (int i=0; x[i]!=NULL; i++) {
        for (int j=i+1; x[j]!=NULL; j++) {
            if (x[i]->getLabel().size() != 0 && x[j]->getLabel().size() != 0)
            if (x[i]->getLabel() == x[j]->getLabel())
                return false;
        }
    }
    for (int i=0; y[i]!=NULL; i++) {
        for (int j=i+1; y[j]!=NULL; j++) {
            if (y[i]->getLabel().size() != 0 && y[j]->getLabel().size() != 0)
            if (y[i]->getLabel() == y[j]->getLabel())
                return false;
        }
    }

    /* Check that the same labels are not used for different positions */
    for (int i=0; x[i]!=NULL; i++) {

        const std::string& xLabel = x[i]->getLabel();
        if (xLabel.size() == 0)
            continue;

        for (int j=0; y[j]!=NULL; j++) {

            const std::string& yLabel = y[i]->getLabel();
            if (yLabel.size() == 0)
                continue;

            if (xLabel == yLabel && i != j)
                return false;
        }
    }

    /* Check that types are different for at least one argument without default values */
    int i;
    for (i=0; x[i]!=NULL && y[i]!=NULL; i++) {
        if (x[i]->getDefaultValue().isType(RbUndefined_name) &&
            y[i]->getDefaultValue().isType(RbUndefined_name) &&
            !x[i]->isType(Ellipsis_name) &&
            !y[i]->isType(Ellipsis_name) && 
            (x[i]->getNumDim() != y[i]->getNumDim() || x[i]->getType() != y[i]->getType()))
            return true;
    }
    for (int j=i; x[j]!=NULL; j++) {
        if (x[j]->getDefaultValue().isType(RbUndefined_name) &&
            !x[j]->isType(Ellipsis_name))
            return true;
    }
    for (int j=i; y[j]!=NULL; j++) {
        if (y[j]->getDefaultValue().isType(RbUndefined_name) &&
            !y[j]->isType(Ellipsis_name))
            return true;
    }

    return false;
}


/** Print function table for user */
void FunctionTable::printValue(std::ostream& o) const {

    o << "<name> = <returnType> function (<formal arguments>)" << std::endl;
    for (std::multimap<std::string, RbFunction*>::const_iterator i=table.begin(); i!=table.end(); i++) {
        o << i->first << " = ";
        i->second->printValue(o);
        o << std::endl;
    }
}


/** Complete info about object */
std::string FunctionTable::toString(void) const {

    std::ostringstream o;
    if (table.size() == 0)
        o << "FunctionTable with no entries" << std::endl;
    else
        o << "FunctionTable with " << table.size() << " entries:" << std::endl;
    printValue(o);

    return o.str();
}
