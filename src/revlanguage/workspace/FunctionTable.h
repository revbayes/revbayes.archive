/**
 * @file
 * This file contains the declaration of FunctionTable, which is
 * used to hold global functions in the base environment (the
 * global workspace) and the user workspace.
 *
 * @brief Declaration of FunctionTable
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 * @extends RbObject
 *
 * $Id$
 */

#ifndef FunctionTable_H
#define FunctionTable_H

#include "RbLanguageObject.h"
#include "RbPtr.h"

#include <map>
#include <ostream>
#include <string>
#include <vector>

namespace RevLanguage {
    
class Argument;
class ArgumentRule;
class Function;


class FunctionTable {

    public:
        FunctionTable(FunctionTable* parent = NULL);                                                                                    //!< Empty table
        FunctionTable(const FunctionTable& x);                                                                                          //!< Copy constructor
        virtual                                 ~FunctionTable();                                                                       //!< Delete functions

        // Assignment operator
        FunctionTable&                          operator=(const FunctionTable& x);                                                      //!< Assignment operator 

        // Basic utility functions
        virtual FunctionTable*                  clone(void) const;                                                                      //!< Clone object
        void                                    printValue(std::ostream& o) const;                                                      //!< Print table for user
        void                                    printTableWithoutHeader(std::ostream& o) const;                                         //!< Print table table entries user without header

        // FunctionTable functions
        virtual void                            addFunction(const std::string name, Function *func);                                    //!< Add function
        void                                    clear(void);                                                                            //!< Clear table
        RbLanguageObject*                       executeFunction(const std::string&           name,
                                                                const std::vector<Argument>& args);                                     //!< Evaluate function (once)
        bool                                    existsFunction(const std::string &name) const;                                          //!< Does this table contain a function with given name?
        void                                    eraseFunction(const std::string& name);                                                 //!< Erase a function (all versions)
        std::vector<Function*>                  findFunctions(const std::string& name) const;                                           //!< Return functions matching name
        const Function&                         getFunction(const std::string& name);                                                   //!< Get function (a copy)
        const Function&                         getFunction(const std::string& name, const std::vector<Argument>& args);                //!< Get function (a copy)
        bool                                    isDistinctFormal(const ArgumentRules& x, const ArgumentRules& y) const;                 //!< Are formals unique?
        void                                    setParentTable(const FunctionTable* ft) { parentTable = ft; }                           //!< Set parent table

    protected:
        Function&                               findFunction(const std::string&           name,
                                                             const std::vector<Argument>& args);                                        //!< Find function, process args
        
        // Member variables
        std::multimap<std::string, Function*>   table;                                                                                  //!< Table of functions
        const FunctionTable*                    parentTable;                                                                            //!< Enclosing table

};
    
}

#endif

