/**
 * @file
 * This file contains the declaration of MethodTable, which is
 * used to hold member functions of complex objects.
 *
 * @brief Implementation of MethodTable
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "MethodTable.h"
#include "RbFunction.h"
#include "RbNames.h"

#include <sstream>


/** Basic constructor, empty table with or without parent */
MethodTable::MethodTable(MethodTable* parent)
    : FunctionTable(parent), funcs(), ids() {
}


/** Copy constructor */
MethodTable::MethodTable(const MethodTable& x)
    : FunctionTable(x) {

    for (std::multimap<std::string, RbFunction*>::const_iterator i=table.begin(); i!=table.end(); i++) {
        int index = int(funcs.size());
        funcs[index]     = (*i).second;
        ids[(*i).second] = index;
    }
}


/** Assignment operator */
MethodTable& MethodTable::operator=(const MethodTable& x) {

    if (this != &x) {

        FunctionTable::operator=(x);

        funcs.clear();
        ids.clear();
        for (std::multimap<std::string, RbFunction*>::const_iterator i=table.begin(); i!=table.end(); i++) {
            int index = int(funcs.size());
            funcs[index] = (*i).second;
            ids[(*i).second]    = index;
        }
    }

    return (*this);
}


/** Add function to table */
void MethodTable::addFunction(const std::string name, RbFunction* func) {

    FunctionTable::addFunction(name, func);

    int index = int(funcs.size());
    funcs[index] = func;
    ids[func]    = index;
}


/** Return brief info about object */
std::string MethodTable::briefInfo () const {

    std::ostringstream   o;
    o << "MethodTable with " << table.size() << " functions";

    return o.str();
}


/** Find and execute operation based on function id; only safe version if there are overloaded member functions,
    we cannot rely on the name without having access to some arguments to use for argument matching. */
DAGNode* MethodTable::executeFunction(int funcId) const {

    std::map<int, RbFunction*>::const_iterator it = funcs.find(funcId);
    return (*it).second->executeOperation(getProcessedArguments(funcId));    
}


/** Get processed arguments */
std::vector<VariableSlot> const& MethodTable::getProcessedArguments(int funcId) const {

    std::map<int, RbFunction*>::const_iterator it = funcs.find(funcId);
    return (*it).second->getProcessedArguments();
}


/** Process arguments (for repeated execution) */
int MethodTable::processArguments(const std::string& name, const std::vector<Argument>& args) const {

    RbFunction* theFunction = findFunction(name, args, false);

    return ids.find(theFunction)->second;
}


/** Complete info about object */
std::string MethodTable::richInfo(void) const {

    std::ostringstream o;
    if (table.size() == 0)
        o << "MethodTable with no entries" << std::endl;
    else
        o << "MethodTable with " << table.size() << " entries:" << std::endl;
    printValue(o);

    return o.str();
}


