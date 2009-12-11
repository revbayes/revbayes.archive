/**
 * @file
 * This file contains the implementation of Workspace, which is
 * used to hold the global workspace and the user workspace.
 *
 * @brief Implementation of Workspace
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @extends Frame
 * @package parser
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#include "FunctionTable.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbObject.h"
#include "Workspace.h"

#include <sstream>

/** Basic constructor */
Workspace::Workspace(Workspace* parentSpace)
    : Frame(parentSpace), functionTable(parentSpace->getFunctionTable()) {
}


/** Destructor */
Workspace::~Workspace() {

    delete functionTable;
}


/** Assignment operator */
Workspace& Workspace::operator=(const Workspace& x) {

    if (this != &x) {

        delete functionTable;
        functionTable = new FunctionTable(x.functionTable);
    }

    return (*this);
}


/** Add a function to the workspace */
void Workspace::addFunction(const std::string& name, RbFunction* func) {

    if (existsVariable(name))
        throw RbException("There is already a variable named '" + name + "' in the workspace");

    functionTable->addFunction(name, func);
}


/** Erase a function */
void Workspace::eraseFunction(const std::string& name) {

    functionTable->eraseFunction(name);
}


/** Execute function */
const RbObject* Workspace::executeFunction(const std::string& name, const std::vector<Argument>& args) const {

    return functionTable->executeFunction(name, args);
}


/** Get function */
RbFunction* Workspace::getFunction(const std::string& name, const std::vector<Argument>& args) {

    return functionTable->getFunction(name, args);
}


/** Print workspace */
void Workspace::printValue(std::ostream& o) const {

    Frame::printValue(o);

}


