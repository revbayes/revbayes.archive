/**
 * @file
 * This file contains the declaration of Func_msimplex, which is used to construct
 * a move for simplices
 * 
 * @brief Declaration of Func_msimplex
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_msimplex_H
#define Func_msimplex_H

#include "RbFunction.h"

class DAGNode;
class VectorString;



class Func_msimplex :  public RbFunction {
    
    public:
        // Basic utility functions
        Func_msimplex*              clone(void) const;                                                       //!< Clone the object
        const VectorString&         getClass(void) const;                                                    //!< Get class vector
        
        // Regular functions
        DAGNode*                    execute(void);                                                           //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;                                            //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                                               //!< Get type of return value
    
    private:

};

#endif

