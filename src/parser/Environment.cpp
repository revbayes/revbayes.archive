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

/**
 * @brief Constructor from parent environment
 *
 * The constructor initializes parentEnvironment
 * to the supplied pointer.
 *
 * @param parentEnv     Pointer to the parent environment
 *
 */
Environment::Environment(Environment* parentEnv) :
    parentEnvironment(parentEnv), variableTable() {
}


/**
 * @brief Destructor
 *
 * All variables are managed by the environment so they
 * should be destroyed here.
 */
Environment::~Environment() {

    for (std::map<std::string, RbObject*>::iterator i=variableTable.begin(); i!=variableTable.end(); i++)
        delete i->second;
}


/**
 * @brief Add variable
 *
 * Add a variable to the variable table. The table manages any
 * objects stored in it so we just store a pointer to the
 * variable and then delete it in the destructor.
 *
 * @param name      Name of variable
 * @param variable  Pointer to the object
 * @return          Returns true on success, false on failure
 *
 */
bool Environment::addVariable(const std::string& name, RbObject* variable) {

    std::pair<std::map<std::string, RbObject*>::iterator, bool> retVal;

    retVal = variableTable.insert(std::pair<std::string, RbObject*>(name, variable));
    
    return retVal.second;
}


/**
 * @brief Get variable
 * 
 * Get a variable in the variable table.
 *
 * @param name  Name of variable
 * @return      Returns pointer to object on success, 
 *
 */
RbObject* Environment::getVariable(const std::string& name) {

    if (variableTable.find(name) == variableTable.end()) {
        if (parentEnvironment != NULL)
            return parentEnvironment->getVariable(name);
        else
            return NULL;
    }

    return variableTable[name];
}

