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

#include "Frame.h"
#include "FunctionTable.h"
#include "VectorString.h"

#include <map>
#include <ostream>
#include <string>

class ConstantNode;
class DAGNode;
class Distribution;
class DistributionReal;
class MemberObject;
class RandomNumberGenerator;
class RbFunction;
class RbObject;

typedef std::map<std::string, RbObject*> TypeTable;

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
 * distribution. When addDistribution is called, the relevant distribution functions are added
 * to the function table.
 *
 */
class Workspace : public Frame {

    public:

        bool                        addDistribution(const std::string& name, Distribution* dist);                        //!< Add distribution
        bool                        addDistribution(const std::string& name, DistributionReal* dist);                    //!< Add real-valued distribution
        bool                        addFunction(const std::string& name, RbFunction* func);                              //!< Add function
        bool                        addType(RbObject* exampleObj);                                                       //!< Add type
        bool                        addTypeWithConstructor(const std::string& name, MemberObject* templ);                //!< Add type with constructor
        DAGNode*                    executeFunction(const std::string& name, const std::vector<Argument>& args) const;   //!< Execute function
        FunctionTable*              getFunctionTable(void) { return functionTable; }                                     //!< Get function table
        RbFunction*                 getFunction(const std::string& name, const std::vector<Argument>& args);             //!< Get function copy
        RandomNumberGenerator*      get_rng(void);                                                                       //!< Get default random number generator
        const std::string&          getTypeNameRef(const std::string* name) const;                                       //!< Get reference to type name
        void                        initializeGlobalWorkspace(void);                                                     //!< Initialize global workspace
        bool                        isXOfTypeY(const std::string& x, const std::string& y) const;                        //!< Type checking

        bool                        isXConvertibleToY(const std::string& xType, int xDim, const std::string& yType, int yDim) const;    //!< Type conversion checking
        void                        printValue(std::ostream& c) const;                                                   //!< Print workspace
        static Workspace&           globalWorkspace(void)                                                                //!< Get global workspace
                                    {
                                        static Workspace globalSpace = Workspace();
                                        return globalSpace;
                                    }
        static Workspace&           userWorkspace(void)                                                                  //!< Get user workspace
                                    {
                                         static Workspace userSpace = Workspace(&globalWorkspace());
                                         return userSpace;
                                    }
 
    private:
                                    Workspace(void);                                                                     //!< Workspace with NULL parent
                                    Workspace(Workspace* parentSpace);                                                   //!< Workspace with parent
                                    Workspace(const Workspace& w) {}                                                     //!< Prevent copy
                                   ~Workspace(void);                                                                     //!< Delete function table
        Workspace&                  operator=(const Workspace& w);                                                       //! Prevent assignment

        FunctionTable*              functionTable;                                                                       //!< Table holding functions
        TypeTable                   typeTable;                                                                           //!< Type table
};

#endif

