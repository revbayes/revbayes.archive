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
#include "VectorInteger.h"
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
        if (!isDistinctFormal(i->second->getArgumentRules(), func->getArgumentRules())) {
            std::ostringstream msg;
            msg << "'";
            i->second->printValue(msg);
            msg << "' cannot overload '";
            func->printValue(msg);
            msg << "'";
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


/** Clear table */
void FunctionTable::clear(void) {

    for (std::multimap<std::string, RbFunction*>::iterator i=table.begin(); i!=table.end(); i++)
        delete (*i).second;

    table.clear();
}


/** Erase function */
void FunctionTable::eraseFunction(const std::string& name) {

    std::pair<std::multimap<std::string, RbFunction*>::iterator,
              std::multimap<std::string, RbFunction*>::iterator> retVal;

    retVal = table.equal_range(name);
    table.erase(retVal.first, retVal.second);
}


/** Execute function */
DAGNode* FunctionTable::executeFunction(const std::string& name, const std::vector<Argument>& args) const {

    RbFunction* theFunction = findFunction(name, args);

    return theFunction->execute();
}


/** Find function (also processes arguments) */
RbFunction* FunctionTable::findFunction(const std::string& name, const std::vector<Argument>& args) const {

    std::pair<std::multimap<std::string, RbFunction*>::const_iterator,
              std::multimap<std::string, RbFunction*>::const_iterator> retVal;

    size_t count = table.count(name);
    if (count == 0) {
        if (parentTable != NULL)
            return parentTable->findFunction(name, args);
        else
            throw RbException("No function named '"+ name + "'");
    }
    retVal = table.equal_range(name);
    if (count == 1) {
        if (retVal.first->second->processArguments(args) == false)
            throw RbException("Argument mismatch for call to '" + name + "'");
        return retVal.first->second;
    }
    else {
        VectorInteger matchScore, bestScore;
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
            return bestMatch;
    }
}


/** Get function */
RbFunction* FunctionTable::getFunction(const std::string& name, const std::vector<Argument>& args) const {

    RbFunction* theFunction = (RbFunction*)(findFunction(name, args)->clone());

    theFunction->processArguments(args);

    return theFunction;
}


/** Get function value */
RbObject* FunctionTable::getFunctionValue(const std::string& name, const std::vector<Argument>& args) const {

    RbFunction* theFunction = findFunction(name, args);

    return theFunction->execute()->getValue();
}


/** Check if two formals are unique */
bool FunctionTable::isDistinctFormal(const ArgumentRules& x, const ArgumentRules& y) const  {

    /* Check that all labels are unique in both sets of argument rules */
    for (size_t i=0; i<x.size(); i++) {
        for (size_t j=i+1; j < y.size(); j++) {
            if (x[i]->getLabel().size() != 0 && x[j]->getLabel().size() != 0)
            if (x[i]->getLabel() == x[j]->getLabel())
                return false;
        }
    }
    for (size_t i=0; i<y.size(); i++) {
        for (size_t j=i+1; j<y.size(); j++) {
            if (y[i]->getLabel().size() != 0 && y[j]->getLabel().size() != 0)
            if (y[i]->getLabel() == y[j]->getLabel())
                return false;
        }
    }

    /* Check that the same labels are not used for different positions */
    for (size_t i=0; i<x.size(); i++) {

        const std::string& xLabel = x[i]->getLabel();
        if (xLabel.size() == 0)
            continue;

        for (size_t j=0; j<y.size(); j++) {

            const std::string& yLabel = y[i]->getLabel();
            if (yLabel.size() == 0)
                continue;

            if (xLabel == yLabel && i != j)
                return false;
        }
    }

    /* Check that types are different for at least one argument without default values */
    size_t i;
    for (i=0; i<x.size() && i<y.size(); i++) {
        if (x[i]->hasDefault() == false &&
            y[i]->hasDefault() == false &&
            !x[i]->isType(Ellipsis_name) &&
            !y[i]->isType(Ellipsis_name) &&
            (x[i]->getDim() != y[i]->getDim() || x[i]->getValueType() != y[i]->getValueType()))
            return true;
    }
    for (size_t j=i; j<x.size(); j++) {
        if (x[j]->hasDefault() == false &&
            !x[j]->isType(Ellipsis_name))
            return true;
    }
    for (size_t j=i; j<y.size(); j++) {
        if (y[j]->hasDefault() == false &&
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
