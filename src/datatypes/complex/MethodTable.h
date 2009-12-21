/**
 * @file
 * This file contains the declaration of MethodTable, which is
 * used to hold descriptions of member functions (methods) of
 * complex language objects. It is not a true function table,
 * it just holds the function descriptions for the parser to use
 * in printing descriptions of complex language objects and in
 * providing info about member methods.
 *
 * @brief Declaration of MethodTable
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 * @extends RbObject
 *
 * $Id: MethodTable.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef MethodTable_H
#define MethodTable_H

#include "MethodDescr.h"
#include "RbInternal.h"

#include <map>
#include <ostream>
#include <string>

class ArgumentRule;

class MethodTable : RbInternal {

    public:
        void            addMethodDescr(const std::string name, const std::string& retType, const ArgumentRule** argRules);     //!< Add description 
        MethodTable*    clone() const { return new MethodTable(*this); }                //!< Clone table
        virtual const StringVector& getClass(void) const;                               //!< Get class vector
        void            printValue(std::ostream& o) const;                              //!< Print description of member functions
        void            printValue(std::ostream& o, const std::string& name) const;     //!< Print description of named member function(s)
        std::string     toString(void) const;                                           //!< Print complete info

    private:
        std::multimap<const std::string, const MethodDescr>  methodTable;               //!< Table with descriptions of member functions
};

#endif

