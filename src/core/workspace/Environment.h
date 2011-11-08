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
#include "RbInternal.h"
#include "RbLanguageObject.h"
#include "RbPtr.h"
#include "Variable.h"
#include "VariableSlot.h"

#include <map>
#include <string>
#include <vector>


typedef std::map<std::string, RbPtr<VariableSlot> > VariableTable;         //!< Typedef for convenience


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
 * A Environment essentially consists of a variable table. The workspace
 * also contains a function table, and a class (type) table.
 *
 */
const std::string Environment_name = "Environment";

class Environment : public RbInternal {
    
public:
    Environment(void);                                                                                                      //!< Constructor of Environment with NULL parent
    Environment(RbPtr<Environment> parentFr);                                                                                     //!< Constructor of Environment with parent
    Environment(const Environment &x);                                                                                      //!< Copy Constructor
    
    virtual                        ~Environment();                                                                          //!< Destrcutor
    
    // Operators
    RbPtr<VariableSlot>             operator[](const std::string& name);                                                    //!< Get named variable slot reference
    RbPtr<const VariableSlot>       operator[](const std::string& name) const;                                              //!< Get named variable slot const reference
    RbPtr<VariableSlot>             operator[](const size_t index);                                                         //!< Get named variable slot reference
    RbPtr<const VariableSlot>       operator[](const size_t index) const;                                                   //!< Get named variable slot const reference
    
    // Basic utility functions
    virtual Environment*            clone(void) const;                                                                      //!< Clone Environment
    virtual const VectorString&     getClass() const;                                                                       //!< Get class vector
    virtual const TypeSpec&         getTypeSpec(void) const;                                                                //!< Get language type of the object
    virtual void                    printValue(std::ostream& o) const;                                                      //!< Print table for user
    virtual std::string             richInfo(void) const;                                                                   //!< Complete info to string
    
    // Regular functions
    void                            addVariable(const std::string& name, RbPtr<VariableSlot> slot);                         //!< Add variable
    void                            addVariable(const std::string& name, RbPtr<Variable>& var);                             //!< Add variable
    void                            addVariable(const std::string& name, Variable* var);                                    //!< Add variable
    void                            addVariable(const std::string& name, const TypeSpec &ts, RbPtr<Variable> var);          //!< Add variable
    void                            addVariable(const std::string& name, const TypeSpec& typeSp, RbPtr<DAGNode> variable);  //!< Add variable
    void                            addVariable(const std::string& name, const TypeSpec& typeSp);                           //!< Add variable
    void                            clear(void);                                                                            //!< clears the variable table
    void                            eraseVariable(const std::string& name);                                                 //!< Erase a variable
    bool                            existsVariable(const std::string& name) const;                                          //!< Does variable exist?
    
    RbPtr<const DAGNode>            getDagNode(const std::string& name) const;                                              //!< Convenient alternative for [name]->getDagNode()
    RbPtr<DAGNode>                  getDagNode(const std::string& name);                                                    //!< Convenient alternative for [name]->getDagNode() (non-const to return non-const node)
    const std::string&              getName(size_t i) const { return varNames[i]; }                                         //!< Get name at position i.
    RbPtr<const RbObject>           getValue(const std::string& name) const;                                                //!< Convenient alternative for [name]->getValue()
    RbPtr<RbObject>                 getValue(const std::string& name);                                                      //!< Convenient alternative for [name]->getValue() (non-const to return non-const value)
    virtual bool                    isSameOrParentOf(RbPtr<Environment> otherEnvironment) const;                            //!< Is the Environment same or parent of other Environment?
    void                            setName(size_t i, const std::string &n);                                                //!< Replace the name of the i'th variable
    void                            setParentEnvironment(RbPtr<Environment> newEnvironment) { parentEnvironment = newEnvironment; }//!< Set parent Environment
    size_t                          size(void) const { return varNames.size(); }                                            //!< Get size of variable table 
    
protected:
    
    VariableTable                   variableTable;                                                                          //!< Variable table
    RbPtr<Environment>              parentEnvironment;                                                                      //!< Pointer to enclosing Environment
    std::vector<std::string>        varNames;
    
private:
    static const TypeSpec           typeSpec;
};

#endif
