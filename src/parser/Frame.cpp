/**
 * @file
 * This file contains the implementation of Frame, which
 * is used to hold information about an evaluation or
 * execution frame.
 *
 * @brief Implementation of Frame
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

#include "Frame.h"

/**
 * @brief Constructor from parent frame
 *
 * The constructor initializes parentFrame
 * to the supplied pointer.
 *
 * @param parentEnv     Pointer to the parent frame
 *
 */
Frame::Frame(Frame* parentFr) :
    parentFrame(parentFr), variableTable() {
}


/**
 * @brief Destructor
 *
 * All variables are managed by the frame so they
 * should be destroyed here.
 */
Frame::~Frame() {

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
bool Frame::addVariable(const std::string& name, RbObject* variable) {

    std::pair<std::map<std::string, RbObject*>::iterator, bool> retVal;
    retVal = variableTable.insert(std::pair<std::string, RbObject*>(name, variable));
    
    return retVal.second;
}


/**
 * @brief Erase variable
 *
 * Erase a variable from the variable table. The table manages any
 * objects stored in it so we need to delete the value here.
 *
 * @param name      Name of variable
 * @return          Returns true on success, false on failure
 *
 */
bool Frame::eraseVariable(const std::string& name) {

    int numErased = variableTable.erase(name);

    return numErased != 0;
}


/**
 * @brief Exists variable
 *
 * Check if a variable exists.
 *
 * @param name      Name of variable
 * @return          Returns true if the variable exists
 *
 */
bool Frame::existsVariable(const std::string& name) const {

    if (variableTable.find(name) == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->existsVariable(name);
        else
            return false;
    }

    return true;
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
RbObject* Frame::getVariable(const std::string& name) {

    if (variableTable.find(name) == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getVariable(name);
        else
            return NULL;
    }

    return variableTable[name];
}

