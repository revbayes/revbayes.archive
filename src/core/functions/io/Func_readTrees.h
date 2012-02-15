/**
 * @file
 * This file contains the declaration of Func_readTrees, which uses the NCL to read in trees in any of the supported formats:
 * newick
 *
 * @brief Declaration of Func_readTrees
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_readTrees_H
#define Func_readTrees_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class RbFileManager;
class VectorString;

const std::string Func_readTrees_name = "Read trees function";

class Func_readTrees :  public RbFunction {
    
public:
    // Basic utility functions
    Func_readTrees*             clone(void) const;                                                      //!< Clone the object
    const VectorString&         getClass(void) const;                                                   //!< Get class vector
    const TypeSpec&             getTypeSpec(void) const;                                                //!< Get language type of the object
    
    // Regular functions
    const ArgumentRules&        getArgumentRules(void) const;                                           //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                                              //!< Get type of return value
    
protected:
    const RbLanguageObject&     executeFunction(void);                                                  //!< Execute function
    
private:
    void                        formatError(RbFileManager& fm, std::string& errorStr);                  //!< Format the error string when (mis)reading files
    
    static const TypeSpec       typeSpec;
    static const TypeSpec       returnTypeSpec;
};

#endif

