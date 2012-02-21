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
#include "RbUtil.h"
#include "VectorString.h"
#include "Ellipsis.h"

#include <sstream>


// Definition of the static type spec member
const TypeSpec FunctionTable::typeSpec(FunctionTable_name);

/** Basic constructor, empty table with or without parent */
FunctionTable::FunctionTable(FunctionTable* parent) : RbInternal(), table(), parentTable(parent) {

}


/** Copy constructor */
FunctionTable::FunctionTable(const FunctionTable& x) {
    
    for (std::multimap<std::string, RbFunction* >::const_iterator i=x.table.begin(); i!=x.table.end(); i++)
        {
        table.insert(std::pair<std::string, RbFunction* >( (*i).first, ( (*i).second->clone() )));
        }
    parentTable = x.parentTable;
}


/** Destructor */
FunctionTable::~FunctionTable(void) {

    clear();
    
}

/** Assignment operator */
FunctionTable& FunctionTable::operator=(const FunctionTable& x) {

    if (this != &x) {

        clear();
        for (std::multimap<std::string, RbFunction* >::const_iterator i=x.table.begin(); i!=x.table.end(); i++)
            {
            table.insert(std::pair<std::string, RbFunction* >((*i).first, ((*i).second->clone())));
            }
        parentTable = x.parentTable;
    }

    return (*this);
}


/** Add function to table */
void FunctionTable::addFunction(const std::string name, RbFunction* func) {
    std::pair<std::multimap<std::string, RbFunction* >::iterator,
              std::multimap<std::string, RbFunction* >::iterator> retVal;

    retVal = table.equal_range(name);
    for (std::multimap<std::string, RbFunction* >::iterator i=retVal.first; i!=retVal.second; i++) {
        if (!isDistinctFormal(i->second->getArgumentRules(), func->getArgumentRules())) {
            std::ostringstream msg;
            msg << name << " =  ";
            i->second->printValue(msg);
            msg << " cannot overload " << name << " = ";
            func->printValue(msg);
            
            // free the memory
            delete func;
            
            // throw the error message
            throw RbException(msg.str());
        }
    }
    table.insert(std::pair<std::string, RbFunction* >(name, func));
}


/** Clear table */
void FunctionTable::clear(void) {

    for (std::multimap<std::string, RbFunction*>::iterator it = table.begin(); it != table.end(); it++) {
        RbFunction* theFunction = it->second;
        delete theFunction;
    }
    
    table.clear();
}


/** Erase function */
void FunctionTable::eraseFunction(const std::string& name) {

    std::pair<std::multimap<std::string, RbFunction* >::iterator,
              std::multimap<std::string, RbFunction* >::iterator> retVal;

    retVal = table.equal_range(name);
    table.erase(retVal.first, retVal.second);
    
    for (std::multimap<std::string, RbFunction*>::iterator it=retVal.first; it!=retVal.second; it++) {
        RbFunction* theFunction = it->second;
        delete theFunction;
    }
}


/** Execute function and get its variable value (evaluate once) */
const RbLanguageObject& FunctionTable::executeFunction(const std::string& name, const std::vector<Argument>& args) {

    RbFunction&             theFunction = findFunction(name, args);
    const RbLanguageObject& theValue    = theFunction.execute();

    theFunction.clear();

    return theValue;
}


/**
 * Find functions matching name
 *
 * @todo Inherited functions are not returned if there
 *       are functions matching the name in the current
 *       workspace.
 */
std::vector<RbFunction* > FunctionTable::findFunctions(const std::string& name) const {

    std::vector<RbFunction* >  theFunctions;

    size_t count = table.count(name);
    if (count == 0) {
        if (parentTable != NULL)
            return parentTable->findFunctions( name );
        else
            return theFunctions;
    }

    std::pair<std::multimap<std::string, RbFunction* >::const_iterator,
              std::multimap<std::string, RbFunction* >::const_iterator> retVal;
    retVal = table.equal_range( name );

    std::multimap<std::string, RbFunction* >::const_iterator it;
    for ( it=retVal.first; it!=retVal.second; it++ )
        theFunctions.push_back( (*it).second->clone() );

    return theFunctions;
}


/** Find function (also processes arguments) */
RbFunction& FunctionTable::findFunction(const std::string& name, const std::vector<Argument>& args) {
    
    std::pair<std::multimap<std::string, RbFunction* >::iterator,
              std::multimap<std::string, RbFunction* >::iterator> retVal;
    
    size_t count = table.count(name);
    if (count == 0) {
        if (parentTable != NULL) {// TODO: We shouldn't allow const casts!!!
            FunctionTable* pt = const_cast<FunctionTable*>(parentTable);
            return pt->findFunction(name, args);
        }
        else
            throw RbException("No function named '"+ name + "'");
    }
    retVal = table.equal_range(name);
    if (count == 1) {
        if (retVal.first->second->checkArguments(args,NULL) == false) {
            
            std::ostringstream msg;
            msg << "Argument mismatch for call to function '" << name << "'. Correct usage is:" << std::endl;
            retVal.first->second->printValue( msg );
            msg << std::endl;
            throw RbException( msg );
        }
        retVal.first->second->processArguments(args);
        return *retVal.first->second;
    }
    else {
        VectorInteger* matchScore = new VectorInteger();
        VectorInteger bestScore;
        RbFunction* bestMatch = NULL;

        bool ambiguous = false;
        std::multimap<std::string, RbFunction* >::iterator it;
        for (it=retVal.first; it!=retVal.second; it++) {
            matchScore->clear();
            if ( (*it).second->checkArguments(args, matchScore) == true ) {
                if ( bestMatch == NULL ) {
                    bestScore = *matchScore;
                    bestMatch = it->second;
                    ambiguous = false;
                }
                else {
                    size_t j;
                    for (j=0; j<matchScore->size() && j<bestScore.size(); j++) {
                        if ((*matchScore)[j] < bestScore[j]) {
                            bestScore = *matchScore;
                            bestMatch = it->second;
                            ambiguous = false;
                            break;
                        }
                        else if ((*matchScore)[j] > bestScore[j])
                            break;
                    }
                    if (j==matchScore->size() || j==bestScore.size()) {
                        ambiguous = true;   // Continue checking, there might be better matches ahead
                    }
                }
            }
        }
        /* Delete all processed arguments except those of the best matching function, if it is ambiguous */
        for ( it = retVal.first; it != retVal.second; it++ ) {
            if ( !( (*it).second == bestMatch && ambiguous == false ) )
                (*it).second->clear();
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
            bestMatch->processArguments(args);
            return *bestMatch;
        }
    }
}


/** Get class vector describing type of object */
const VectorString& FunctionTable::getClass() const {
    
    static VectorString rbClass = VectorString(FunctionTable_name) + RbInternal::getClass();
    return rbClass;
}


/** Get function copy (for repeated evaluation in a DeterministicNode) */
RbFunction* FunctionTable::getFunction(const std::string& name, const std::vector<Argument>& args) {
    
    // find the template function
    RbFunction& theFunction = findFunction(name, args);

    // we need a copy because we got the template function
    RbFunction* copy = theFunction.clone();

    // clear the arguments of the template function
    theFunction.clear();

    return copy;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& FunctionTable::getTypeSpec(void) const {
    return typeSpec;
}


/** Check if two formals are unique */
bool FunctionTable::isDistinctFormal(const ArgumentRules& x, const ArgumentRules& y) const  {

    /* Check that all labels are unique in both sets of argument rules */
    for (size_t i=0; i<x.size(); i++) {
        for (size_t j=i+1; j < x.size(); j++) {
            if (x[i].getArgumentLabel().size() != 0 && x[j].getArgumentLabel().size() != 0)
            if (x[i].getArgumentLabel() == x[j].getArgumentLabel())
                return false;
        }
    }
    for (size_t i=0; i<y.size(); i++) {
        for (size_t j=i+1; j<y.size(); j++) {
            if (y[i].getArgumentLabel().size() != 0 && y[j].getArgumentLabel().size() != 0)
            if (y[i].getArgumentLabel() == y[j].getArgumentLabel())
                return false;
        }
    }

    /* Check that the same labels are not used for different positions */
    for (size_t i=0; i<x.size(); i++) {

        const std::string& xLabel = x[i].getArgumentLabel();
        if (xLabel.size() == 0)
            continue;

        for (size_t j=0; j<y.size(); j++) {

            const std::string& yLabel = y[j].getArgumentLabel();
            if (yLabel.size() == 0)
                continue;

            if (xLabel == yLabel && i != j)
                return false;
        }
    }

    /* Check that types are different for at least one argument without default values */
    size_t i;
    for (i=0; i<x.size() && i<y.size(); i++) {
        if (x[i].hasDefault() == false &&
            y[i].hasDefault() == false &&
            !x[i].isType(Ellipsis_name) &&
            !y[i].isType(Ellipsis_name) &&
            x[i].getArgumentType() != y[i].getArgumentType())
            return true;
    }
    for (size_t j=i; j<x.size(); j++) {
        if (x[j].hasDefault() == false &&
            !x[j].isType(Ellipsis_name))
            return true;
    }
    for (size_t j=i; j<y.size(); j++) {
        if (y[j].hasDefault() == false &&
            !y[j].isType(Ellipsis_name))
            return true;
    }

    return false;
}


/** Print function table for user */
void FunctionTable::printValue(std::ostream& o) const {

    o << "<name> = <returnType> function (<formal arguments>)" << std::endl;
    for (std::multimap<std::string, RbFunction* >::const_iterator i=table.begin(); i!=table.end(); i++) {
        o << i->first << " = ";
        i->second->printValue(o);
        o << std::endl;
    }
}


/** Complete info about object */
std::string FunctionTable::debugInfo(void) const {

    std::ostringstream o;
    if (table.size() == 0)
        o << "FunctionTable with no entries" << std::endl;
    else
        o << "FunctionTable with " << table.size() << " entries:" << std::endl;
    printValue(o);

    return o.str();
}
