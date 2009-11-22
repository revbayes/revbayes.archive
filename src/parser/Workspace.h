/**
 * @file
 * This file contains the declaration of Workspace, which is
 * used to hold the global workspace, which is the mother of
 * all environments, the ultimate base environment. It is also
 * used for the user workspace, which is the next descendant
 * environment, containing all variables, types and functions
 * defined by the user.
 *
 * @brief Declaration of Workspace
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @extends Environment
 * @package parser
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#ifndef Workspace_H
#define Workspace_H

#include "DAGNode.h"
#include "Environment.h"
#include "RbFunction.h"
#include "RbObject.h"
#include <map>
#include <string>


/**
 * @brief Workspace
 *
 * The Workspace class is used for two singleton instances, the global workspace and the user
 * workspace.
 *
 * The global workspace is the base environment for all other environments. It contains all
 * builtin functions and types, as well as any builtin (system) variables. The user workspace
 * is enclosed within the global workspace, and contains all functions, types and global
 * variables defined by the user. Local variables defined by the user are kept in local
 * environments.
 *
 * The workspace has a variable table, which it inherits from Environment. In addition, it
 * keeps a function table and type table. It provides various types of functionality for
 * storing and retrieving functions, types and member variables and their initializers.
 *
 * The workspace ensures that symbol names are unique for functions and variables by cross-
 * checking the tables. Class names need to be unique but can overlap with function or variable
 * names. Names are not allowed to clash with reserved words like 'for' or 'while'. As far as
 * user-defined functions are concerned, this is guaranteed by the parser.
 *
 * The class declaration contains code to create the global workspace and the user workspace.
 * Copy, assignment and construction of workspaces are prevented by making constructors and
 * assignment operator private.
 *
 * A distribution is a special complex of functions related to a particular probability
 * distribution. When addDistribution is called, the relevant distributions are added to
 * the function table.
 *
 */
class Workspace : Environment {

    public:

        bool        addFunction(const std::string& name, RbFunction* entry);        //!< Add function
        bool        addDistribution(const std::string& name, Distribution* entry);  //!< Add distribution
        bool        addType(const std::string& name, const std::string& base,
                            ArgRule[] memb, FunctionDef[] meth);                    //!< Add type entry
        void        eraseDistribution(const std::string& name);                     //!< Erase distribution
        void        eraseFunction(const std::string& name);                         //!< Erase function
        void        eraseType(const std::string& name);                             //!< Erase type
        RbFunction* getFunction(const std::string& name);                           //!< Get function
        std::map<std::string, ObjectSlot*>  getMembers(const std::string& type);    //!< Get class members
        std::map<std::string, RbFunction*>  getMethods(const std::string& type);    //!< Get class methods
        std::map<std::string, RbFunction*>  getMethod(const std::string& type, const std::string& );    //!< Get class methods
        RbFunction* getFunction(const std::string& name);           //!< Get function
        void        print(ostream &c) const;                        //!< Print table

        static Workspace&   globalWorkSpace() { return globalWorkSpace; }   //!< Get global workspace
        static Workspace&   userWorkSpace() { return userWorkSpace; }       //!< Get user workspace

    private:
            Workspace() {}                      //!< Prevent construction
            Workspace(const Workspace& w) {}    //!< Prevent copy construction
            ~Workspace();                       //!< Destructor, delete objects here

        static Workspace                    globalWorkspace;    //!< The global workspace
        static Workspace                    userWorkspace;      //!< The user workspace

        Workspace&                          operator=(const Workspace& w);  //! Prevent assignment

        std::multimap<const std::string, RbFunction*>   functionTable;      //!< Table holding functions
        std::map<const std::string, ClassDef*>          typeTable;          //!< Table holding variables
};

#endif
