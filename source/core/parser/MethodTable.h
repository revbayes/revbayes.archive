/**
 * @file
 * This file contains the declaration of MethodTable, which is
 * used to hold member functions of complex objects.
 *
 * @brief Declaration of MethodTable
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 * @extends RbObject
 *
 * $Id: MethodTable.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef MethodTable_H
#define MethodTable_H

#include "FunctionTable.h"

#include <map>
#include <ostream>
#include <string>
#include <vector>

class Argument;
class ArgumentRule;
class DAGNode;
class RbFunction;
class RbObject;

class MethodTable : public FunctionTable {

    public:
                                            MethodTable(MethodTable* parent=NULL);                                      //!< Empty table
                                            MethodTable(const MethodTable& x);                                          //!< Copy constructor
                                           ~MethodTable() {}                                                            //!< Destructor doesn't need to do anything

        // Assignment operator
        MethodTable&    operator=(const MethodTable& x);                                                                //!< Assignment operator 

        // Basic utility functions
        std::string                         briefInfo(void) const;                                                      //!< Brief info to string
        MethodTable*                        clone(void) const { return new MethodTable(*this); }                        //!< Clone object
        std::string                         richInfo(void) const;                                                       //!< Complete info to string

        // MethodTable functions
        void                                addFunction(const std::string name, RbFunction* func);                      //!< Add function, generate unique id
        void                                clearArgs(int funcId) const;                                                //!< Delete processed arguments
        DAGNode*                            executeFunction(int funcId) const;                                          //!< Evaluate function (for repeated calls)
        const ArgumentFrame&                getArgs(int funcId) const;                                                  //!< Get processed arguments
        int                                 processArguments(   const std::string&              name,
                                                                const std::vector<Argument>&    args) const;            //!< Process arguments
        void                                setParentTable(FunctionTable* table) { parentTable = table; }               //!< Set parent table

    protected:
        std::map<int, RbFunction*>          funcs;                                                                      //!< Map from function id to function
        std::map<RbFunction*, int>          ids;                                                                        //!< Map from function to function id
};

#endif

