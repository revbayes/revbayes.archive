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
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 * @extends RbObject
 *
 * $Id: FunctionTable.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef FunctionTable_H
#define FunctionTable_H

#include "RbFunction.h"     // For typedef of ArgumentRules
#include "RbInternal.h"

#include <map>
#include <ostream>
#include <string>
#include <vector>

class Argument;
class ArgumentRule;
class RbObject;

const std::string FunctionTable_name = "Function Table";

class FunctionTable : public RbInternal {

    public:
        FunctionTable(FunctionTable* parent = NULL);                                                                                    //!< Empty table
        FunctionTable(const FunctionTable& x);                                                                                          //!< Copy constructor
        virtual                                 ~FunctionTable();                                                                       //!< Delete functions

        // Assignment operator
        FunctionTable&                          operator=(const FunctionTable& x);                                                      //!< Assignment operator 

        // Basic utility functions
        virtual std::string                     debugInfo(void) const;                                                                  //!< Brief info to string
        virtual FunctionTable*                  clone(void) const { return new FunctionTable(*this); }                                  //!< Clone object
        virtual const TypeSpec&                 getTypeSpec(void) const;                                                                //!< Get language type of the object
        virtual const VectorString&             getClass() const;                                                                       //!< Get class vector
        void                                    printValue(std::ostream& o) const;                                                      //!< Print table for user

        // FunctionTable functions
        virtual void                            addFunction(const std::string name, RbFunction* func);                                  //!< Add function
        void                                    clear(void);                                                                            //!< Clear table
        RbLanguageObject*                       executeFunction(const std::string&           name,
                                                                const std::vector<Argument>& args);                                     //!< Evaluate function (once)
        void                                    eraseFunction(const std::string& name);                                                 //!< Erase a function (all versions)
        std::vector<RbFunction* >               findFunctions(const std::string& name) const;                                           //!< Return functions matching name
        RbFunction*                             getFunction(const std::string& name, const std::vector<Argument>& args);                //!< Get function (a copy)
        bool                                    isDistinctFormal(const ArgumentRules& x, const ArgumentRules& y) const;                 //!< Are formals unique?
        void                                    setParentTable(const FunctionTable* table) { parentTable = table; }                     //!< Set parent table

    protected:
        RbFunction&                             findFunction(const std::string&           name,
                                                             const std::vector<Argument>& args);                                        //!< Find function, process args
        
        // Member variables
        std::multimap<std::string, RbFunction* >  table;                                                                                //!< Table of functions
        const FunctionTable*                    parentTable;                                                                            //!< Enclosing table
    
    private:
        static const TypeSpec       typeSpec;

};

#endif

