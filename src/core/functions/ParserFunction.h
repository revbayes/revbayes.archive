/**
 * @file
 * This file contains the declaration of ParserFunction, which is
 * the interface and abstract base class for RevBayes functions.
 *
 * @brief Declaration of ParserFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-16 17:27:08 -0700 (Mon, 16 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface ParserFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id: ParserFunction.h 1378 2012-04-17 00:27:08Z hoehna $
 */

#ifndef ParserFunction_H
#define ParserFunction_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "Argument.h"
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Environment.h"
#include "RbFunction.h"

class DAGNode;
class InferenceFunction;

/**
 * This is the interface and abstract base class for functions in
 * RevBayes. Function instances are put in the function table in the
 * relevant Environment (user workspace or base environment) if they are
 * global. If they are member functions of user-defined type, they
 * are instead associated with the function table of the approprioate
 * class in the class table of the user workspace.
 * 
 * A function instance knows its argument rules and can process a
 * vector of labeled argument values according to these rules to pro-
 * duce a ready-to-use list of arguments. The processing involves label
 * matching as well as filling of missing values with default values.
 * The processing of labeled argument values is done in the function
 * processArguments, which will throw an error if the provided
 * arguments do not match.
 *
 */

class ParserFunction : public RbFunction {
    
public:
    ParserFunction(InferenceFunction *func, const std::string &funcName, const ArgumentRules &argRules, const TypeSpec &returnType, bool throws = false);                                                                   //!< Basic constructor
    ParserFunction(const ParserFunction &x);                                                    //!< Copy constuctor
    virtual                                            ~ParserFunction(void);                                                                  //!< Destructor
    
    // Basic utility functions
    virtual ParserFunction*                         clone(void) const;                                                              //!< Clone object
    static const std::string&                       getClassName(void);                                                                 //!< Get class name
    static const TypeSpec&                          getClassTypeSpec(void);                                                             //!< Get class type spec
    virtual std::string                             debugInfo(void) const;                                                              //!< Brief info about object
    const TypeSpec&                                 getTypeSpec(void) const;
    
    // Basic utility functions you should not have to override
    void                                            printValue(std::ostream& o) const;                                                  //!< Print the general information on the function ('usage')
    
    // ParserFunction functions you have to override
    virtual const RbLanguageObject&                 execute(void);                                                                      //!< Execute function
    virtual const ArgumentRules&                    getArgumentRules(void) const;                                                   //!< Get argument rules
    virtual const TypeSpec&                         getReturnType(void) const;                                                      //!< Get type of return value
    
    // ParserFunction function you may want to override
    virtual bool                                    checkArguments(const std::vector<Argument>& passedArgs, std::vector<unsigned int>* matchScore); //!< Process args, return a match score if pointer is not null
    virtual void                                    processArguments(const std::vector<Argument>& passedArgs);                          //!< Process args, return a match score if pointer is not null
    virtual bool                                    throws(void) const { return throwsError; }                                          //!< Does the function throw exceptions?
    
    
    // ParserFunction functions you should not override
    void                                            clear(void);                                                                        //!< Clear argument Environment "args"
    const std::vector<Argument>&                    getArguments(void) const;                                                           //!< Get processed arguments in argument Environment "args"
    std::vector<Argument>&                          getArguments(void);                                                                 //!< Get processed arguments in argument Environment "args"
    void                                            setArgument(const std::string& name, const Argument& arg);                          //!< Set the argument for the label. We collect the argument and delegate to setArgumentVariable()
    
protected:
    
    virtual void                                    clearArguments(void);                                                               //!< Clear argument Environment "args"
    virtual const RbLanguageObject&                 execute(const std::vector<const RbObject*>& args);                                  //!< Execute the function. This is the function one has to overwrite for vector type execution.
    virtual const RbLanguageObject&                 executeFunction();                                                                  //!< Execute the function. This is the function one has to overwrite for single return values.
    virtual const RbLanguageObject&                 executeFunction(const std::vector<const RbObject*>& args);                          //!< Execute the function. This is the function one has to overwrite for single return values.
    
    // Member variables
    bool                                            argsProcessed;                                                                      //!< Are arguments processed?
    std::vector<Argument>                           args;
    
private:   
    int                                             computeMatchScore(const DAGNode* arg, const ArgumentRule& rule);
    
    ArgumentRules                                   argRules;
    InferenceFunction*                              function;
    std::string                                     functionName;
    TypeSpec                                        returnType;
    bool                                            throwsError;
    
    //        RbLanguageObject*                               retVal;
};

#endif

