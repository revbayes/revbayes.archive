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

#include "RbInternal.h"

#include <map>
#include <ostream>
#include <string>
#include <vector>

class Argument;
class ArgumentRule;
class RbFunction;
class RbObject;

class FunctionTable : RbInternal {

    public:
                        FunctionTable(FunctionTable* parent=NULL);  //!< Empty table
                        FunctionTable(const FunctionTable& x);      //!< Copy constructor
                        ~FunctionTable();                           //!< Delete functions

        // Assignment operator
        FunctionTable&  operator=(const FunctionTable& x);          //!< Assignment operator 

        // Static help function that can be used by other objects, like MethodTable
        static bool     isDistinctFormal(const ArgumentRule** x, const ArgumentRule** y); //!< Are formals unique?

        // Basic utility functions
        std::string     briefInfo(void) const;                                  //!< Brief info to string
        FunctionTable*  clone(void) const { return new FunctionTable(*this); }  //!< Clone object
        std::string     toString(void) const;                                   //!< Complete info to string
        void            printValue(std::ostream& o) const;                      //!< Print table for user

        // Regular functions
        void            addFunction(const std::string name, RbFunction* func);  //!< Add function
        void            eraseFunction(const std::string& name);                 //!< Erase function
        const RbObject* executeFunction(const std::string& name, const std::vector<Argument>& args) const;  //!< Execute function
        RbFunction*     getFunction(const std::string& name, const std::vector<Argument>& args) const;      //!< Get function (a copy)

    protected:
        std::multimap<std::string, RbFunction*>     table;          //!< Table of functions
        FunctionTable*                              parentTable;    //!< Enclosing table
};

#endif

