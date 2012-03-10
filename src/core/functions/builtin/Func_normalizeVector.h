/**
 * @file
 * This file contains the declaration of Func_normalize, which 
 * normalizes a vector of positive real numbers.
 *
 * @brief Declaration of Func_normalize
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_normalize_H
#define Func_normalize_H

#include "RbFunction.h"
#include "Simplex.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;


class Func_normalizeVector :  public RbFunction {

    public:
        // Basic utility functions
        Func_normalizeVector*       clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value

    protected:
        const RbLanguageObject&     executeFunction(void);                                      //!< Execute function
        void                        setArgumentVariable(const std::string& name, const Variable* var);

    private:

        // memberfunction return values
        RbVector<RealPos>           normalizedVector;

        // arguments
        RbConstVariablePtr          vector;
        RbConstVariablePtr          total;
};

#endif

