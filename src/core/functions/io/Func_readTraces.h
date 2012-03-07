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
    Func_readTraces*            clone(void) const;                                                      //!< Clone the object
    static const std::string&   getClassName(void);                                                     //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                                 //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                                                //!< Get language type of the object
    
    // Regular functions
    const ArgumentRules&        getArgumentRules(void) const;                                           //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                                              //!< Get type of return value

protected:
    const RbLanguageObject&     executeFunction(void);                                                  //!< Execute function
    void                        setArgumentVariable(const std::string& name, const Variable* var);
    
private:
    void                        formatError(RbFileManager& fm, std::string& errorStr);                  //!< Format the error string when (mis)reading files

    // Arguments
    RbConstVariablePtr          filename;
};

#endif

