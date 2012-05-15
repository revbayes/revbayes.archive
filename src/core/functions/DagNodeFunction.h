/**
 * @file
 * This file contains the declaration of DagNodeFunction, which is used
 * to map DagNode function calls (DagNode method calls) of complex objects
 * to internal functions instead of providing regular RbFunction objects
 * implementing the DagNode functions. Note that the first argument passed
 * in a DagNode function call is a pointer to the DagNode of the calling
 * object (like a this pointer).
 *
 * @brief Declaration of DagNodeFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-10 12:55:11 +0100 (Sat, 10 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: DagNodeFunction.h 1327 2012-03-10 11:55:11Z hoehna $
 */

#ifndef DagNodeFunction_H
#define DagNodeFunction_H

#include "RbFunction.h"

#include <map>
#include <set>
#include <string>
#include <vector>

class ArgumentRule;
class DAGNode;

class DagNodeFunction :  public RbFunction {
    
public:
    DagNodeFunction(const TypeSpec retType, ArgumentRules* argRules);                                       //!< Constructor
    virtual ~DagNodeFunction(void);                                                                         //!< Destructor
    
    // Basic utility functions
    DagNodeFunction*             clone(void) const;                                                         //!< Clone the object
    static const std::string&   getClassName(void);                                                         //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                                     //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                                                    //!< Get language type of the object
    
    // Regular functions   
    const ArgumentRules&        getArgumentRules(void) const;                                               //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                                                  //!< Get type of return value
    void                        setArgumentVariable(const std::string& name, const Variable* var);     
    void                        setDagNode(DAGNode& obj);                                                   //!< Set the DagNode object to which this function belongs
    void                        setMethodName(const std::string& name) { funcName = name; }                 //!< Set name of DagNode method

    protected:
    RbPtr<RbLanguageObject>     executeFunction(const std::vector<const RbObject*>& args);                  //!< Execute the function. This is the function one has to overwrite for single return values.

    
private:
    const ArgumentRules*        argumentRules;                                                              //!< Argument rules (different for different DagNode functions)
    std::string                 funcName;                                                                   //!< Name of DagNode method
    DAGNode*                    node;                                                                       //!< The DagNode object to which this function belongs (we do not own the DagNode object because of cyclic ownership)
    const TypeSpec              returnType;                                                                 //!< Return type (different for different DagNode functions)
    
};

#endif

