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
#include "Environment.h"
#include "RbInternal.h"
#include "RbPtr.h"

class ArgumentRule;
class DAGNode;
class VectorInteger;
class VectorString;

typedef std::vector<ArgumentRule*> ArgumentRules;

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
const std::string RbFunction_name = "function";

class RbFunction :  public RbInternal {

    public:
        virtual                                        ~RbFunction(void) {}                                                                 //!< Destructor
                                                        RbFunction(const RbFunction &x);                                                    //!< Copy constuctor

        // Basic utility functions you have to override
        virtual RbFunction*                             clone(void) const = 0;                                                              //!< Clone object
    	virtual const VectorString&                     getClass(void) const;                                                               //!< Get class vector

        // Basic utility functions you may want to override
        virtual std::string                             briefInfo(void) const;                                                              //!< Brief info about object
        virtual std::string                             richInfo(void) const;                                                               //!< Complete info about object

        // Basic utility functions you should not have to override
    	void                                            printValue(std::ostream& o) const;                                                  //!< Print the general information on the function ('usage')

        // RbFunction functions you have to override
        virtual RbPtr<RbLanguageObject>                 execute(void) = 0;                                                                  //!< Execute function
        virtual const ArgumentRules&                    getArgumentRules(void) const = 0;                                                   //!< Get argument rules
        virtual const TypeSpec&                         getReturnType(void) const = 0;                                                      //!< Get type of return value

        // RbFunction function you may want to override
        virtual bool                                    processArguments(const std::vector<RbPtr<Argument> >&   passedArgs,
                                                                         RbPtr<VectorInteger>                  matchScore = RbPtr<VectorInteger>::getNullPtr());                  //!< Process args, return a match score if pointer is not null
        virtual bool                                    addAsChildOfArguments(void) { return true; }                                        //!< Should we add the node containing this function as a child of its parameters (arguments)? False in cases such as constructor functions and true in cases like math functions
        virtual bool                                    throws(void) const { return false; }                                                      //!< Does the function throw exceptions?
    

        // RbFunction functions you should not override
        void                                            clearArguments(void);                                                               //!< Clear argument Environment "args"
        const Environment&                              getArguments(void) const { return args; }                                           //!< Get processed arguments in argument Environment "args"

	protected:
                                                        RbFunction(void);                                                                   //!< Basic constructor

        // Member variables
        Environment                                     args;                                                                               //!< Environment for passed arguments
        bool                                            argsProcessed;                                                                      //!< Are arguments processed?
};

#endif

