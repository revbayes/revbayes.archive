/**
 * @file
 * This file contains the declaration of Workspace, which is
 * used to hold the global workspace, a singleton class.
 *
 * @brief Declaration of Workspace
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @implements RbObject
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
 * @brief Global workspace
 *
 * There is only one global workspace, which is the base environment for all other environments.
 * It has a variable table, to which a user can add variables. It also has a function table,
 * Class SymbolTable is used to maintain a table of all the symbols that are defined.
 *  The symbol table actually keeps two separate maps between symbols and objects, one for
 *  variables (DAGNode objects) and the other for functions (RbFunction objects). Each entry
 *  in the table is a pair consisting of a string holding the name of the entry (the key)
 *  and a pointer to the object itself.
 *
 *  Unlike R, we do not allow functions and variables to be represented by the same symbol.
 *  This is ensured in the add functions by cross-checking the tables.
 *
 *  The class declaration contains code to create the global symbol table. Copy and assignment
 *  of symbol tables are prevented by making copy constructor and assignment operator private.
 *
 *  If we wish to support scoping in the future (for loops and user-defined functions), this is
 *  probably the right place to keep track of the scope.
 */
class SymbolTable {

    public:
            ~SymbolTable();    //!< Destructor, delete objects here

        bool        add(const string name, RbFunction *entry);  //!< Add function entry
        bool        add(const string name, DAGNode *entry);     //!< Add variable entry
        bool        add(const char* name, RbFunction *entry);   //!< Add function entry (C-style)
        bool        add(const char* name, DAGNode *entry);      //!< Add variable entry (C-style)
        void        eraseFunction(const string& name);          //!< Erase function
        void        eraseVariable(const string& name);          //!< Erase variable
        bool        existsFunction(const string &name) const;   //!< Check if function exists
        bool        existsVariable(const string &name) const;   //!< Check if variable exists
        RbFunction* getFunction(string &name);                  //!< Get function
        DAGNode*    getVariable(string &name);                  //!< Get variable
        void        print(ostream &c) const;                    //!< print table

        /** get global symbol table */
        static SymbolTable& globalTable() {
            return theSymbolTable;
        }
	static WorkSpace& workSpaceInstance(void)
		{
		static WorkSpace singleWorkSpace;
		return singleWorkSpace;
		}

    private:
            Workspace() {}   //!< Prevent construction

        SymbolTable(const SymbolTable &s) {}                //! Prevent copy construction
        SymbolTable&    operator=(const SymbolTable&);      //! Prevent assignment

        static SymbolTable theSymbolTable;                  //! the only instance
        map<const string, RbFunction*>  functionTable;      //!< table holding functions
        map<const string, DAGNode*>     varTable;           //!< table holding variables
};

#endif
