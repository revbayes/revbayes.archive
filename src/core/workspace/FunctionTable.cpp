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
 * @author The RevBayes Development Core Team
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
            msg << name << " =  ";
            i->second->printValue(msg);
            msg << " cannot overload " << name << " = ";
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


/** Execute function and get its variable value (evaluate once) */
DAGNode* FunctionTable::executeFunction(const std::string& name, const std::vector<Argument>& args) const {

    RbFunction* theFunction = findFunction(name, args, true);
    DAGNode*    theValue    = theFunction->execute();

    theFunction->clearArgs();

    return theValue;
}


/**
 * Find functions matching name
 *
 * @todo Inherited functions are not returned if there
 *       are functions matching the name in the current
 *       workspace.
 */
std::vector<const RbFunction*> FunctionTable::findFunctions(const std::string& name) const {

    std::vector<const RbFunction*>  theFunctions;

    size_t count = table.count(name);
    if (count == 0) {
        if (parentTable != NULL)
            return parentTable->findFunctions( name );
        else
            return theFunctions;
    }

    std::pair<std::multimap<std::string, RbFunction*>::const_iterator,
              std::multimap<std::string, RbFunction*>::const_iterator> retVal;
    retVal = table.equal_range( name );

    std::multimap<std::string, RbFunction*>::const_iterator it;
    for ( it=retVal.first; it!=retVal.second; it++ )
        theFunctions.push_back( (*it).second );

    return theFunctions;
}


/** Find function (also processes arguments) */
RbFunction* FunctionTable::findFunction(const std::string& name, const std::vector<Argument>& args, bool evaluateOnce) const {

    std::pair<std::multimap<std::string, RbFunction*>::const_iterator,
              std::multimap<std::string, RbFunction*>::const_iterator> retVal;

    size_t count = table.count(name);
    if (count == 0) {
        if (parentTable != NULL)
            return parentTable->findFunction(name, args, evaluateOnce);
        else
            throw RbException("No function named '"+ name + "'");
    }
    retVal = table.equal_range(name);
    if (count == 1) {
        if (retVal.first->second->processArguments(args, evaluateOnce) == false) {
            
            std::ostringstream msg;
            msg << "Argument mismatch for call to function '" << name << "'. Correct usage is:" << std::endl;
            retVal.first->second->printValue( msg );
            msg << std::endl;
            throw RbException( msg );
        }
        return retVal.first->second;
    }
    else {
        VectorInteger matchScore, bestScore;
        RbFunction* bestMatch = NULL;

        bool ambiguous = false;
        std::multimap<std::string, RbFunction*>::const_iterator it;
        for (it=retVal.first; it!=retVal.second; it++) {
            if ( (*it).second->processArguments(args, evaluateOnce, &matchScore) == true ) {
                if ( bestMatch == NULL ) {
                    bestScore = matchScore;
                    bestMatch = it->second;
                    ambiguous = false;
                }
                else {
                    size_t j;
                    for (j=0; j<matchScore.size() && j<bestScore.size(); j++) {
                        if (matchScore[j] < bestScore[j]) {
                            bestScore = matchScore;
                            bestMatch = it->second;
                            ambiguous = false;
                            break;
                        }
                        else if (matchScore[j] > bestScore[j])
                            break;
                    }
                    if (j==matchScore.size() || j==bestScore.size()) {
                        ambiguous = true;   // Continue checking, there might be better matches ahead
                    }
                }
            }
        }
        /* Delete all processed arguments except those of the best matching function, if it is ambiguous */
        for ( it = retVal.first; it != retVal.second; it++ ) {
            if ( !( (*it).second == bestMatch && ambiguous == false ) )
                (*it).second->clearArgs();
        }
        if ( bestMatch == NULL || ambiguous == true ) {
            std::ostringstream msg;
            if ( bestMatch == NULL )
                msg << "No overloaded function '" << name << "' matches arguments" << std::endl;
            else
                msg << "Ambiguous call to function '" << name << "'" << std::endl;
            msg << "Potentially matching functions are:" << std::endl;
            for ( it = retVal.first; it != retVal.second; it++ ) {
                (*it).second->printValue( msg );
                msg << std::endl;
            }
            throw RbException( msg );
        }
        else {
            return bestMatch;
        }
    }
}


/** Get function copy (for repeated evaluation in a FunctionNode) */
RbFunction* FunctionTable::getFunction(const std::string& name, const std::vector<Argument>& args) const {

    RbFunction* theFunction = findFunction(name, args, false);
    RbFunction* copy        = theFunction->clone();

    theFunction->clearArgs();

    return copy;
}


/** Check if two formals are unique */
bool FunctionTable::isDistinctFormal(const ArgumentRules& x, const ArgumentRules& y) const  {

    /* Check that all labels are unique in both sets of argument rules */
    for (size_t i=0; i<x.size(); i++) {
        for (size_t j=i+1; j < y.size(); j++) {
            if (x[i]->getArgLabel().size() != 0 && x[j]->getArgLabel().size() != 0)
            if (x[i]->getArgLabel() == x[j]->getArgLabel())
                return false;
        }
    }
    for (size_t i=0; i<y.size(); i++) {
        for (size_t j=i+1; j<y.size(); j++) {
            if (y[i]->getArgLabel().size() != 0 && y[j]->getArgLabel().size() != 0)
            if (y[i]->getArgLabel() == y[j]->getArgLabel())
                return false;
        }
    }

    /* Check that the same labels are not used for different positions */
    for (size_t i=0; i<x.size(); i++) {

        const std::string& xLabel = x[i]->getArgLabel();
        if (xLabel.size() == 0)
            continue;

        for (size_t j=0; j<y.size(); j++) {

            const std::string& yLabel = y[i]->getArgLabel();
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
            (x[i]->getArgDim() != y[i]->getArgDim() || x[i]->getArgType() != y[i]->getArgType()))
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
std::string FunctionTable::richInfo(void) const {

    std::ostringstream o;
    if (table.size() == 0)
        o << "FunctionTable with no entries" << std::endl;
    else
        o << "FunctionTable with " << table.size() << " entries:" << std::endl;
    printValue(o);

    return o.str();
}
