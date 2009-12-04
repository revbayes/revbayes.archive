/**
 * @file
 * This file contains the declaration of FunctionTable, which is
 * used to hold global functions in the base environment (the
 * global workspace) and the user workspace.
 *
 * @brief Declaration of FunctionTable
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 * @extends RbObject
 *
 * $Id: FunctionTable.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef FunctionTable_H
#define FunctionTable_H

#include "ArgumentRule.h"

#include <string>
#include <ostream>
#include <map>


class FunctionTable {

    public:
            FunctionTable(const FunctionTable& x);      //!< Copy constructor
            ~FunctionTable();                           //!< Delete functions

        // Static functions for use by this and other objects
        static bool     isDistinctFormal(std::vector<ArgumentRule>& x, std::vector<ArgumentRule> y) const;
        static bool     isMatch(std::vector<ArgumentRule>& x, std::vector<Argument>& y) const;

        // Basic utility functions
        std::string     briefInfo() const = 0;                          //!< Brief info about table
        void            print(std::ostream& o) const = 0;               //!< Print table

        // Regular functions
        bool        addFunction(const std::string name, RbFunction* func);                      //!< Add function
        bool        existsFunction(const std::string& name, std::vector<Argument>& args) const; //!< Exists?
        bool        existsFunction(const std::string& name,
                        std::vector<ArgumentRule>& argRules) const;                             //!< Exists?
        RbFunction* getFunction(const std::string& name, std::vector<Argument>& args) const;    //!< Get function

    protected:
        std::multimap<std::string, RbFunction*> table;      //!< Table of functions
};

#endif

