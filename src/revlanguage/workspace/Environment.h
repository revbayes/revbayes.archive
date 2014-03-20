/**
 * @file
 * This file contains the declaration of environment, which is
 * the abstract base class for classes that are used to hold
 * information about an evaluation or execution environment.
 * Each environment has a pointer to the enclosing (parent) environment.
 * A Environment and its parents constitute an evaluation environment.
 * The base environment is the global workspace. It is a
 * special type of Environment, which is described in the class
 * Workspace, derived from Environment.
 *
 * An environment essentially consists of a table (map) or vector of
 * variables, which may or may not be labeled. The workspace
 * also contains a function table, and a class (type) table.
 *
 * @brief Declaration of Environment
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-17, version 1.0
 *
 * $Id$
 */

#ifndef Environment_H 
#define Environment_H

#include "FunctionTable.h"
#include "RbLanguageObject.h"
#include "RbPtr.h"
#include "Variable.h"
#include "VariableSlot.h"

#include <map>
#include <string>
#include <vector>


namespace RevLanguage {

    typedef std::map<std::string, VariableSlot* > VariableTable; //!< Typedef for convenience

    class Environment {
    public:
        Environment(void); //!< Constructor of Environment with NULL parent
        Environment(Environment* parentFr); //!< Constructor of Environment with parent
        Environment(const Environment& x); //!< Copy Constructor
        virtual ~Environment(void); //!< Destrcutor

        // Operators
        VariableSlot& operator[](const std::string& name); //!< Get named variable slot reference
        const VariableSlot& operator[](const std::string& name) const; //!< Get named variable slot const reference
        VariableSlot& operator[](const size_t index); //!< Get named variable slot reference
        const VariableSlot& operator[](const size_t index) const; //!< Get named variable slot const reference
        Environment& operator=(const Environment& x); //!< Assignment

        // Basic utility functions
        virtual Environment* clone(void) const; //!< Clone Environment
        virtual void printValue(std::ostream& o) const; //!< Print table for user

        // Regular functions
        bool addFunction(const std::string& name, Function* func); //!< Add function
        void addVariable(const std::string& name, VariableSlot* slot); //!< Add variable
        void addVariable(const std::string& name, const RbPtr<Variable>& var); //!< Add variable
        void addVariable(const std::string& name, RbLanguageObject* variable); //!< Add variable
        void addVariable(const std::string& name); //!< Add variable
        void clear(void); //!< clears the variable table
        void eraseVariable(const std::string& name); //!< Erase a variable
        RbLanguageObject* executeFunction(const std::string& name, const std::vector<Argument>& args); //!< Execute function
        bool existsFunction(const std::string &name) const; //!< Does a function exists with given name?
        bool existsVariable(const std::string& name) const; //!< Does variable exist?
        std::string generateUniqueVariableName(void); //!< Automatically generate a unique variable name

        const Function& getFunction(const std::string& name); //!< Get function copy
        const Function& getFunction(const std::string& name, const std::vector<Argument>& args); //!< Get function copy
        const FunctionTable& getFunctionTable(void) const; //!< Get function table (const)
        FunctionTable& getFunctionTable(void); //!< Get function table (non-const)
        const std::string& getName(size_t i) const; //!< Get name at position i.
        const RbLanguageObject& getValue(const std::string& name) const; //!< Convenient alternative for [name]->getValue()
        RbLanguageObject& getValue(const std::string& name); //!< Convenient alternative for [name]->getValue() (non-const to return non-const value)
        virtual bool isSameOrParentOf(const Environment& otherEnvironment) const; //!< Is the Environment same or parent of other Environment?
        void setName(size_t i, const std::string &n); //!< Replace the name of the i'th variable
        void setParentEnvironment(Environment* newEnvironment); //!< Set parent Environment
        size_t size(void) const; //!< Get size of variable table 
        void remove(const RbPtr<Variable>& var); //!< Remove variable from workspace
        void remove(const std::string& var); //!< Remove variable from workspace
        
        const VariableTable& getVariableTable(void) const; 
        VariableTable& getVariableTable(void); 

    protected:

        FunctionTable functionTable; //!< Table holding functions
        Environment* parentEnvironment; //!< Pointer to enclosing Environment
        VariableTable variableTable; //!< Variable table
        std::vector<std::string> varNames;

    };

}

#endif
