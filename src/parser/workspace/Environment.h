/**
 * @file
 * This file contains the declaration of Environment, which is
 * the abstract base class for classes that are used to hold
 * information about an evaluation or execution Environment.
 * Each Environment has a pointer to the enclosing (parent) Environment.
 * A Environment and its parents constitute an evaluation Environment.
 * The base Environment is the global workspace. It is a
 * special type of Environment, which is described in the class
 * Workspace, derived from Environment.
 *
 * A Environment essentially consists of a table (map) or vector of
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

#include "DAGNode.h"
#include "FunctionTable.h"
#include "RbInternal.h"
#include "RbLanguageObject.h"
#include "RbPtr.h"
#include "Variable.h"
#include "VariableSlot.h"

#include <map>
#include <string>
#include <vector>


typedef std::map<std::string, VariableSlot* > VariableTable;         //!< Typedef for convenience


/**
 * @brief Execution or evaluation Environment
 *
 * Environment is used to hold information about an evaluation or
 * execution Environment. Each Environment has a pointer to the enclosing
 * (parent) Environment. A Environment and its parents together constitute
 * an evaluation Environment. The base Environment is the global
 * workspace. It is a special type of Environment, which is described
 * in the class Workspace, derived from Environment. The global work-
 * space contains the user workspace, which is a workspace hold-
 * ing user-defined types and functions. The user workspace, in
 * turn, contains the nested regular Environments, which are created
 * during execution or evaluation of code.
 *
 * A Environment essentially consists of a variable table and function table. The workspace
 * also contains a class (type) table.
 *
 */

class Environment : public RbInternal {
    
    public:
                                                Environment(void);                                                                          //!< Constructor of Environment with NULL parent
                                                Environment(Environment* parentFr);                                                         //!< Constructor of Environment with parent
                                                Environment(const Environment& x);                                                          //!< Copy Constructor
        virtual                                ~Environment(void);                                                                          //!< Destrcutor
        
        // Operators
        VariableSlot&                           operator[](const std::string& name);                                                        //!< Get named variable slot reference
        const VariableSlot&                     operator[](const std::string& name) const;                                                  //!< Get named variable slot const reference
        VariableSlot&                           operator[](const size_t index);                                                             //!< Get named variable slot reference
        const VariableSlot&                     operator[](const size_t index) const;                                                       //!< Get named variable slot const reference
        Environment&                            operator=(const Environment& x);                                                            //!< Assignment
       
        // Basic utility functions
        virtual Environment*                    clone(void) const;                                                                          //!< Clone Environment
        static const std::string&               getClassName(void);                                                                         //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                                                     //!< Get class type spec
        virtual const TypeSpec&                 getTypeSpec(void) const;                                                                    //!< Get language type of the object
        virtual void                            printValue(std::ostream& o) const;                                                          //!< Print table for user
        
        // Regular functions
        bool                                    addFunction(const std::string& name, RbFunction* func);                                     //!< Add function
        void                                    addVariable(const std::string& name, VariableSlot* slot);                                   //!< Add variable
        void                                    addVariable(const std::string& name, const RbPtr<Variable>& var);                           //!< Add variable
        void                                    addVariable(const std::string& name, DAGNode* variable);                                    //!< Add variable
        void                                    addVariable(const std::string& name);                                                       //!< Add variable
        void                                    clear(void);                                                                                //!< clears the variable table
        void                                    eraseVariable(const std::string& name);                                                     //!< Erase a variable
        RlValue<RbLanguageObject>               executeFunction(    const std::string&              name,
                                                const std::vector<RbPtr<Argument> >&   args);                                                       //!< Execute function
        bool                                    existsFunction(const std::string &name) const;                                              //!< Does a function exists with given name?
        bool                                    existsVariable(const std::string& name) const;                                              //!< Does variable exist?
        std::string                             generateUniqueVariableName(void);                                                           //!< Automatically generate a unique variable name
        
        RbPtr<const DAGNode>                    getDagNode(const std::string& name) const;                                                  //!< Convenient alternative for [name]->getDagNode()
        const RbPtr<DAGNode>&                   getDagNode(const std::string& name);                                                        //!< Convenient alternative for [name]->getDagNode() (non-const to return non-const node)
        const RbFunction&                       getFunction(const std::string& name);                                                       //!< Get function copy
        const RbFunction&                       getFunction(const std::string& name, const std::vector<RbPtr<Argument> >& args);            //!< Get function copy
        const FunctionTable&                    getFunctionTable(void) const;                                                               //!< Get function table (const)
        FunctionTable&                          getFunctionTable(void);                                                                     //!< Get function table (non-const)
        const std::string&                      getName(size_t i) const;                                                                    //!< Get name at position i.
        RlValue<const RbLanguageObject>         getValue(const std::string& name) const;                                                    //!< Convenient alternative for [name]->getValue()
        const RlValue<RbLanguageObject>&        getValue(const std::string& name);                                                          //!< Convenient alternative for [name]->getValue() (non-const to return non-const value)
        virtual bool                            isSameOrParentOf(const Environment& otherEnvironment) const;                                //!< Is the Environment same or parent of other Environment?
        void                                    setName(size_t i, const std::string &n);                                                    //!< Replace the name of the i'th variable
        void                                    setParentEnvironment(Environment* newEnvironment);                                          //!< Set parent Environment
        size_t                                  size(void) const;                                                                           //!< Get size of variable table 
        
    protected:
        
        FunctionTable                           functionTable;                                                                              //!< Table holding functions
        Environment*                            parentEnvironment;                                                                          //!< Pointer to enclosing Environment
        VariableTable                           variableTable;                                                                              //!< Variable table
        std::vector<std::string>                varNames;
    
};

#endif
