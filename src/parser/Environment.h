/**
 * @file
 * This file contains the declaration of Environment, which
 * is used to hold information about an evaluation or exe-
 * cution environment. Each environment has a pointer to the
 * enclosing (parent) environment. The base environment,
 * which encloses all other environments, is the global
 * workspace. It is a special type of environment and is
 * kept in a singleton class derived from Environment,
 * called WorkSpace.
 *
 * An environment contains a variable table. The workspace
 * also contains a function table, and a class (type) table.
 *
 * @brief Declaration of Environment
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

#ifndef Environment_H 
#define Environment_H

#include <map>
#include <string>

#include "DAGNode.h"
#include "RbObject.h"

/** Environment: class used to hold local variable table */
class Environment : public RbObject {

    public:
            Environment(Environment* parentEnv);    //!< Constructor from parent environment
	        ~Environment();                         //!< Destructor deletes local objects

        static const StringVector   rbClass;                //!< Static class attribute

        // Basic utility functions
        virtual Environment*        clone() const { return new Environment(*this); }    //!< Clone object
        virtual bool                equals(const RbObject* o) const;        //!< Equals comparison
        virtual const StringVector& getClass() const { return rbClass; }    //!< Get class
        virtual void                print(std::ostream& o) const;           //!< Print complete object info
        virtual void                printValue(std::ostream& o) const;      //!< Print value (for user)

        // Regular functions
	    bool        addVariable(const std::string& name, DAGNode* variable);   //!< Add a variable
        bool        eraseVariable(const std::string& name);                     //!< Erase a variable
        bool        existsVariable(const std::string& name);                    //!< Does variable exist?
        DAGNode*    getVariable(const std::string& name);                       //!< Get a variable
        
	
    protected:
        Environment*                            parentEnvironment;  //!< Pointer to enclosing environment
	    std::map<const std::string, DAGNode*>   variableTable;      //!< Local variable table
};

#endif

