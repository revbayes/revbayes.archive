/**
 * @file
 * This file contains the declaration of Func_readTrace.
 * nexus
 *
 * @brief Declaration of Func_readTrace
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_readTraces_H
#define Func_readTraces_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class RbFileManager;
class VectorString;

class Func_readTraces :  public RbFunction {
    
public:
    // Basic utility functions
    Func_readTraces*            clone(void) const;                                                       //!< Clone the object
    const VectorString&         getClass(void) const;                                                    //!< Get class vector
    
    // Regular functions
    RbLanguageObject*           execute(void);                                                           //!< Execute function
    const ArgumentRules&        getArgumentRules(void) const;                                            //!< Get argument rules
    const TypeSpec              getReturnType(void) const;                                               //!< Get type of return value
    
private:
    void                        formatError(RbFileManager& fm, std::string& errorStr);                   //!< Format the error string when (mis)reading files
};

#endif

