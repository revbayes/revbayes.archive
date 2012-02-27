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
 * @author The RevBayes Development Core Team
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

#include "Environment.h"
#include "FunctionTable.h"
#include "VectorString.h"

#include <map>
#include <ostream>
#include <string>

class ConstantNode;
class DAGNode;
class Distribution;
class DistributionContinuous;
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

class Workspace : public Environment {

    public:
        virtual                    ~Workspace(void);                                                                    //!< Delete function table
    
        // Frame functions you have to override
        Workspace*                  clone(void) const;                                                                  //!< Clone frame
        static const std::string&   getClassName(void);                                                                 //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                             //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                                            //!< Get language type of the object
        void                        printValue(std::ostream& o) const;                                                  //!< Print table for user

        bool                        addDistribution(const std::string& name, Distribution* dist);                       //!< Add distribution
        bool                        addDistribution(const std::string& name, DistributionContinuous* dist);             //!< Add distribution on continuous variable
        bool                        addFunction(const std::string& name, RbFunction* func);                             //!< Add function
        bool                        addType(RbObject* exampleObj);                                                      //!< Add type
        bool                        addType(const std::string& name, RbObject* exampleObj);                             //!< Add special abstract type (synonym)
        bool                        addTypeWithConstructor(const std::string& name, MemberObject* templ);               //!< Add type with constructor
        bool                        addTypeWithConstructor(const std::string& name, RbLanguageObject* templ);           //!< Add type with constructor
        bool                        areTypesInitialized(void) const { return typesInitialized; }                        //!< Is type table initialized?
        const RbLanguageObject&     executeFunction(    const std::string&              name,
                                                        const std::vector<Argument>&   args);                           //!< Execute function
        bool                        existsType(const TypeSpec& name) const;                                             //!< Does the type exist in the type table?
//        RbObject*                   findType(const TypeSpec& name) const;                                             //!< Does the type exist in the type table?
        const TypeSpec&             getClassTypeSpecOfType(const std::string& type) const;                              //!< Get reference to class vector of type
        const FunctionTable&        getFunctionTable(void) const { return *functionTable; }                             //!< Get function table (const)
        FunctionTable&              getFunctionTable(void) { return *functionTable; }                                   //!< Get function table (non-const)
        RbFunction*                 getFunction(const std::string& name, const std::vector<Argument>& args);            //!< Get function copy
        void                        initializeGlobalWorkspace(void);                                                    //!< Initialize global workspace
        static Workspace&           globalWorkspace(void)                                                               //!< Get global workspace
                                        {
                                        static Workspace globalSpace = Workspace();
                                        return globalSpace;
                                        }
        static Workspace&           userWorkspace(void)                                                                 //!< Get user workspace
                                        {
                                        static Workspace userSpace = Workspace(&globalWorkspace());
                                        return userSpace;
                                        }
 
    private:
                                    Workspace(void);                                                                    //!< Workspace with NULL parent
                                    Workspace(Workspace* parentSpace);                                                  //!< Workspace with parent
                                    Workspace(Environment* parentSpace);                                                //!< Workspace with parent
                                    Workspace(const Workspace& w);                                                    //!< Prevent copy
        Workspace&                  operator=(const Workspace& w);                                                      //! Prevent assignment

        FunctionTable*              functionTable;                                                                      //!< Table holding functions
        TypeTable                   typeTable;                                                                          //!< Type table

        bool                        typesInitialized;                                                                   //!< Is type table initialized? Before then, we can't perform type checking.

        static const TypeSpec       typeSpec;
};

#endif

