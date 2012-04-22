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
#include "RbVector.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;

class Func__range :  public RbFunction {

    public:
                                    Func__range( void );
    
        // Basic utility functions
        Func__range*                clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value

    protected:
        const RbLanguageObject&     executeFunction(const std::vector<const RbObject*>& args);  //!< Execute function
 
    private:

        // memberfunction return values
        RbVector<Integer>           range;
};

#endif

