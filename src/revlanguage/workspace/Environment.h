#ifndef Environment_H
#define Environment_H

#include "FunctionTable.h"
#include "RevObject.h"
#include "RevPtr.h"
#include "Variable.h"

#include <map>
#include <string>
#include <vector>


namespace RevLanguage {

    typedef std::map<std::string, RevPtr<Variable> > VariableTable;                                                             //!< Typedef for convenience

    /**
     * @brief Environment: Base class for frames
     *
     * Instances of the Environment class are used to hold information about an
     * evaluation or execution environment, such as the stack frame of a function call.
     * Each environment has a pointer to the enclosing (parent) environment.
     * A frame ('Environment') and its parents constitute an evaluation environment.
     * The base environment is the global workspace. It is a
     * special type of environment, which is described in the class
     * Workspace, derived from Environment.
     *
     * An environment essentially consists of a table (map) or vector of
     * variables, which may or may not be labeled. The workspace
     * also contains a function table, and a class (type) table.
     *
     */

    class Environment {
        
    public:
        Environment(void);                                                                                                      //!< Constructor of Environment with NULL parent
        Environment(Environment* parentFr);                                                                                     //!< Constructor of Environment with parent
        Environment(const Environment& x);                                                                                      //!< Copy Constructor
        virtual ~Environment(void);                                                                                             //!< Destrcutor

        // Operators
        Environment&                        operator=(const Environment& x);                                                    //!< Assignment

        // Basic utility functions
        virtual Environment*                clone(void) const;                                                                  //!< Clone Environment
        virtual void                        printValue(std::ostream& o) const;                                                  //!< Print table for user

        // Regular functions
        void                                addAlias(const std::string& name, const RevPtr<Variable>& var);                             //!< Add alias of variable
        bool                                addFunction(const std::string& name, Function* func);                                       //!< Add function
        void                                addNullVariable(const std::string& name);                                                   //!< Add variable with null object
        void                                addReference(const std::string& name, const RevPtr<Variable>& var);                         //!< Add reference to variable
        void                                addVariable(const std::string& name, const RevPtr<Variable>& var);                          //!< Add variable
        void                                addVariable(const std::string& name, RevObject* object);                                    //!< Add variable from object
        void                                clear(void);                                                                                //!< Clear the environment (variables and functions)
        void                                eraseVariable(const std::string& name);                                                     //!< Erase a variable by name
        void                                eraseVariable(const RevPtr<Variable>& var);                                                 //!< Erase a variable by variable address
//        RevPtr<Variable>                    executeFunction(const std::string& name, const std::vector<Argument>& args);                //!< Execute function
        bool                                existsFunction(const std::string &name) const;                                              //!< Does a function exists with given name?
        bool                                existsVariable(const std::string& name) const;                                              //!< Does variable exist?
        bool                                existsVariableInFrame(const std::string& name) const;                                       //!< Does variable exist in this frame?
        std::string                         generateUniqueVariableName(void);                                                           //!< Automatically generate a unique variable name
        const Function&                     getFunction(const std::string& name);                                                       //!< Get function reference
        const Function&                     getFunction(const std::string& name, const std::vector<Argument>& args, bool once) const;   //!< Get function reference
        const FunctionTable&                getFunctionTable(void) const;                                                               //!< Get function table (const)
        FunctionTable&                      getFunctionTable(void);                                                                     //!< Get function table (non-const)
        const RevObject&                    getRevObject(const std::string& name) const;                                                //!< Convenient alternative for [name]->getValue()
        RevObject&                          getRevObject(const std::string& name);                                                      //!< Convenient alternative for [name]->getValue() (non-const to return non-const value)
        RevPtr<Variable>&                   getVariable(const std::string& name);                                                       //!< Get variable
        const RevPtr<Variable>&             getVariable(const std::string& name) const;                                                 //!< Get variable (const)
        const VariableTable&                getVariableTable(void) const;                                                               //!< Get the table with the variables (const)
        VariableTable&                      getVariableTable(void);                                                                     //!< Get the table with the variables (non-const)
        bool                                isProcedure(const std::string& fxnName) const;                                              //!< Is 'fxnName' a procedure?
        virtual bool                        isSameOrParentOf(const Environment& otherEnvironment) const;                                //!< Is the Environment same or parent of other Environment?
        void                                setParentEnvironment(Environment* newEnvironment);                                          //!< Set parent Environment
        size_t                              size(void) const;                                                                           //!< Get size of variable table
        

    protected:

        FunctionTable                       functionTable;                                                                              //!< Table holding functions
        int                                 numUnnamedVariables;                                                                        //!< Current number of unnamed variables
        Environment*                        parentEnvironment;                                                                          //!< Pointer to enclosing Environment
        VariableTable                       variableTable;                                                                              //!< Variable table
    };

}

#endif
