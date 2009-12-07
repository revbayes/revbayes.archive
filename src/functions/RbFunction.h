/**
 * @file
 * This file contains the declaration of RbFunction, which is
 * the interface and abstract base class for RevBayes functions.
 *
 * @brief Declaration of RbFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes development core team
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
#include "RbObject.h"

class ArgumentRule;
class DAGNode;
class StringVector;

/**
 * This is the interface and abstract base class for functions in
 * RevBayes. Function instances are put in the function table in the
 * relevant frame (user workspace or base environment) if they are
 * global. If they are member functions, they are instead associated
 * with the function table of the approprioate class in the class
 * table of the user workspace.
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
class RbFunction :  public RbObject {

    public:
		virtual                     ~RbFunction(void) {}                                    //!< Virtual destructor because of virtual functions

		// static variables
		static const ArgumentRule** argRules;                                               //!< Static argument rules
		static const std::string    returnType;                                             //!< Return type

		const StringVector&         getClass(void) const;                                   //!< Get class

        // Regular functions
        const ArgumentRule**        getArgumentRules(void) const { return argRules; }       //!< Get argument rules
        std::string                 getReturnType(void) const { return returnType; }        //!< Get return type
        RbObject*                   execute(void);                                          //!< Execute using processed args
        RbObject*                   execute(const std::vector<Argument>& args);             //!< Execute function
        void                        setArguments(const std::vector<Argument>& args);        //!< Process and set args

    protected:
                                    RbFunction(void);                                       //!< Basic constructor
                                    RbFunction(const RbFunction& fn);                       //!< Copy constructor
        std::vector<DAGNode*>       processArguments(const std::vector<Argument>& args);    //!< Process args
		virtual RbObject*           executeOperation(const std::vector<DAGNode*>& args)=0;  //!< Execute operation
        std::vector<DAGNode*>       processedArguments;                                     //!< Processed arguments
        bool                        argumentsProcessed;                                     //!< Are arguments processed?
};

#endif

