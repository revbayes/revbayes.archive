/**
 * @file
 * This file contains the declaration of Workspace, which is
 * used to hold the global workspace, the mother of all frames.
 * It is also used for the user workspace, which is the next
 * descendant frame, containing all variables, types and
 * functions defined by the user.
 *
 * @brief Declaration of Workspace
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

#ifndef Workspace_H
#define Workspace_H

#include "DAGNode.h"
#include "Frame.h"
#include "FunctionTable.h"
#include "RbFunction.h"
#include "RbObject.h"

#include <map>
#include <ostream>
#include <string>


/**
 * @brief Workspace
 *
 * The Workspace class is used for two singleton instances, the global workspace and the user
 * workspace.
 *
 * The global workspace is the base frame for all other frames. It contains all
 * builtin functions and types, as well as any builtin (system) variables. The user workspace
 * is enclosed within the global workspace, and contains all functions, types and global
 * variables defined by the user. Local variables defined by the user are kept in local
 * frames.
 *
 * The workspace has a variable table, which it inherits from Frame. In addition, it
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
class Workspace : Frame {

    public:

        void            addFunction(const std::string& name, RbFunction* func);     //!< Add function
        const RbObject* executeFunction(const std::string& name,
                            const std::vector<Argument>& args) const;               //!< Execute function
        void            eraseFunction(const std::string& name);                     //!< Erase function
        FunctionTable*  getFunctionTable() { return functionTable; }                //!< Get function table
        RbFunction*     getFunction(const std::string& name,
                            const std::vector<Argument>& args);                     //!< Get function
        void            printValue(std::ostream &c) const;                          //!< Print workspace

        /** Get global workspace */
        static Workspace& globalWorkspace() {
                static Workspace globalSpace = Workspace(NULL);
                return globalSpace;
        }

        /** Get user workspace */
        static Workspace& userWorkspace() {
                static Workspace userSpace = Workspace(&globalWorkspace());
                return userSpace;
        }
 
    private:
            Workspace(Workspace* parentSpace = NULL);                   //!< Prevent construction
            Workspace(const Workspace& w) {}                            //!< Prevent copy construction
            ~Workspace();                       //!< Destructor, delete objects here

        Workspace&          operator=(const Workspace& w);  //! Prevent assignment

        FunctionTable*      functionTable;      //!< Table holding functions
};

#endif

