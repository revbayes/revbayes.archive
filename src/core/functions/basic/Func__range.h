/**
 * @file
 * This file contains the declaration of Func__range, which is used
 * to look up a variable.
 *
 * @brief Declaration of Func__range
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__range_H
#define Func__range_H

#include "RbFunction.h"
#include "VectorInteger.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

const std::string Func__range_name = "Range function";

class Func__range :  public RbFunction {

    public:
        // Basic utility functions
        Func__range*                clone(void) const;                                          //!< Clone the object
        const VectorString&         getClass(void) const;                                       //!< Get class vector
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value

    protected:
        const RbLanguageObject&     executeFunction(void);                                      //!< Execute function
        void                        setArgumentVariable(const std::string& name, const RbVariablePtr& var);
 
    private:
        static const TypeSpec       typeSpec;
        static const TypeSpec       returnTypeSpec;

        // Arguments
        RbVariablePtr               first;
        RbVariablePtr               last;

        // memberfunction return values
        VectorInteger               range;
};

#endif

