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
class MemberObject;
class RbFunction;
class RbObject;


const std::string MethodTable_name = "Method table";

class MethodTable : public FunctionTable {

    public:
    MethodTable(RbPtr<MethodTable> parent=RbPtr<MethodTable>::getNullPtr());                                      //!< Empty table
        virtual                    ~MethodTable() {}                                                            //!< Destructor doesn't need to do anything

        // Assignment operator
        MethodTable&                operator=(const MethodTable& x);                                            //!< Assignment operator 

        // Basic utility functions
        std::string                 briefInfo(void) const;                                                      //!< Brief info to string
        MethodTable*                clone(void) const { return new MethodTable(*this); }                        //!< Clone object
        const VectorString&         getClass() const;                                                           //!< Get class vector
        const TypeSpec&             getTypeSpec(void) const;                                                    //!< Get language type of the object
        std::string                 richInfo(void) const;                                                       //!< Complete info to string

        // MethodTable function (we just set the name of MemberFunction objects here)
        void                        addFunction(const std::string name, RbPtr<RbFunction> func);                      //!< Add function; set name if appropriate
    
    private:
        static const TypeSpec       typeSpec;
};

#endif

