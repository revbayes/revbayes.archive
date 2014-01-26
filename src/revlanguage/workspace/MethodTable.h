/**
 * @file
 * This file contains the declaration of MethodTable, which is
 * used to hold member functions of complex objects.
 *
 * @brief Declaration of MethodTable
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

#ifndef MethodTable_H
#define MethodTable_H

#include "FunctionTable.h"

#include <map>
#include <ostream>
#include <string>
#include <vector>

namespace RevLanguage {

class Function;

class MethodTable : public FunctionTable {

    public:
        MethodTable(MethodTable* parent = NULL);                                                                //!< Empty table
        virtual                    ~MethodTable() {}                                                            //!< Destructor doesn't need to do anything

        // Assignment operator
        MethodTable&                operator=(const MethodTable& x);                                            //!< Assignment operator 

        // Basic utility functions
        MethodTable*                clone(void) const { return new MethodTable(*this); }                        //!< Clone object

        // MethodTable function (we just set the name of MemberFunction objects here)
        void                        addFunction(const std::string name, Function *func);         //!< Add function; set name if appropriate
    
};
    
}

#endif

