/**
 * @file
 * This file contains the declaration of RbFunction, which is
 * the interface and abstract base class for RevBayes functions.
 *
 * @brief Declaration of RbFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#ifndef RbFunction_H
#define RbFunction_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "Argument.h"
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Environment.h"
#include "RbInternal.h"
#include "RbPtr.h"

class InferenceFunction;
class DAGNode;

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

class RbFunction :  public RbLanguageObject {

    public:
    virtual                                            ~RbFunction(void);                                                                  //!< Destructor
                                                        RbFunction(const RbFunction &x);                                                    //!< Copy constuctor

        // Basic utility functions you have to override
        virtual RbFunction*                             clone(void) const = 0;                                                              //!< Clone object
        static const std::string&                       getClassName(void);                                                                 //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                             //!< Get class type spec

        // Basic utility functions you may want to override
        virtual std::string                             debugInfo(void) const;                                                              //!< Brief info about object

        // Basic utility functions you should not have to override
    	void                                            printValue(std::ostream& o) const;                                                  //!< Print the general information on the function ('usage')
        void                                            setExecutionEnviroment(Environment *e);                                             //!< Set the environment from which the function was executed.

        // RbFunction functions you have to override
        virtual RlValue<RbLanguageObject>               execute(void);                                                                      //!< Execute function
        virtual const ArgumentRules&                    getArgumentRules(void) const = 0;                                                   //!< Get argument rules
        virtual const TypeSpec&                         getReturnType(void) const = 0;                                                      //!< Get type of return value

        // RbFunction function you may want to override
        virtual bool                                    checkArguments(const std::vector<RbPtr<Argument> >& passedArgs, std::vector<unsigned int>* matchScore); //!< Process args, return a match score if pointer is not null
        virtual InferenceFunction*                      getLeanFunction(void) const;                                                        //!< Get the lean version of this function
        virtual void                                    processArguments(const std::vector<RbPtr<Argument> >& passedArgs);                  //!< Process args, return a match score if pointer is not null
        virtual bool                                    throws(void) const { return false; }                                                //!< Does the function throw exceptions?
    

        // RbFunction functions you should not override
        void                                            clear(void);                                                                        //!< Clear argument Environment "args"
        const std::vector<RbPtr<Argument> >&            getArguments(void) const;                                                           //!< Get processed arguments in argument Environment "args"
        std::vector<RbPtr<Argument> >&                  getArguments(void);                                                                 //!< Get processed arguments in argument Environment "args"
        void                                            setArgument(const std::string& name, const RbPtr<Argument>& arg);                   //!< Set the argument for the label. We collect the argument and delegate to setArgumentVariable()
    
	protected:
                                                        RbFunction(void);                                                                   //!< Basic constructor

    
        // function you may want to override
        virtual void                                    clearArguments(void);                                                               //!< Clear argument Environment "args"
        virtual RbPtr<RbLanguageObject>                 executeFunction(const std::vector<const RbObject*>& args);                          //!< Execute the function. This is the function one has to overwrite for single return values.
        virtual void                                    setArgumentVariable(const std::string& name, const RbPtr<const Variable> &var) {}   //!< Set the private member variable here (for derived classes)!

        // helper functions
        RlValue<RbLanguageObject>                       execute(size_t level, const std::vector<size_t> &offsets, const std::vector<RlValue<const RbLanguageObject> >& args);                 //!< Execute the function. This is the function one has to overwrite for vector type execution.

        // Member variables
        bool                                            argsProcessed;                                                                      //!< Are arguments processed?
        std::vector<RbPtr<Argument> >                   args;
        RbPtr<Environment>                              env;

    private:   
        int                                             computeMatchScore(const DAGNode* arg, const ArgumentRule& rule);
    
};

#endif

