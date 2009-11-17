/**
 * @file
 * This file contains the implementation of Environment, which
 * is used to hold information about an evaluation or exe-
 * cution environment.
 *
 * @brief Implementation of Environment
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-17, version 1.0
 *
 * $Id$
 */

#include "Environment.h"

/** Constructor initializes parentEnvironment */
Environment::Environment(Environment* parentEnv) :
    parentEnvironment(parentEnv), variableTable() {
}


/**
 * Destructor. All variables are managed by the environment so
 * they should be destroyed here.
 */
Environment::~Environment() {

    for (std::map::iterator i=variableTable.begin(); i!=variableTable.end(); i++)
        delete (*i->second());
}


/**
 * Add a variable to the variable table.
 *
 * @param  name     Name of variable
 * @param  variable Pointer to DAGNode wrapping the object
 * @return Returns true on success, false on failure
 */
bool Environment::addVariable(std::string name, DAGNode* variable) {

    pair<iterator, bool> retVal;

    retVal = variableTable.insert(std::pair<name, variable>);
    
    return retVal.second;
}


/**
 * Get a variable in the variable table.
 *
 * @param  name Name of variable
 * @return Returns pointer to object on success, 
 */
DAGNode* Environment::getVariable(std::string &name) {

    if (variableTable.find(name) == variableTable.end()) {
        if (parentEnvironment != NULL)
            return parentEnvironment->getVariable(name);
        else
            return NULL;
    }

    return variableTable[name];
}

