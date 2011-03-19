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
#include "RbObject.h"

class ArgumentRule;
class DAGNode;
class VectorInteger;
class VectorString;

typedef std::vector<ArgumentRule*> ArgumentRules;

/**
 * This is the interface and abstract base class for functions in
 * RevBayes. Function instances are put in the function table in the
 * relevant frame (user workspace or base environment) if they are
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
class RbFunction :  public RbObject {

    public:
		virtual                                ~RbFunction(void);                                                                   //!< Destructor

        // Basic utility functions
        virtual std::string                     briefInfo(void) const;                                                              //!< Brief info about object
        virtual RbObject*                       clone(void) const = 0;                                                              //!< Clone object
    	virtual bool                            equals(const RbObject* obj) const;                                                  //!< Check that the functions are the same
    	virtual const VectorString&             getClass(void) const;                                                               //!< Get class vector
    	void                                    printValue(std::ostream& o) const;                                                  //!< Print the general information on the function ('usage')
        virtual std::string                     toString(void) const;                                                               //!< Complete info about object

        // RbFunction functions
        virtual const ArgumentRules&            getArgumentRules(void) const = 0;                                                   //!< Get argument rules
        virtual const TypeSpec                  getReturnType(void) const = 0;                                                      //!< Get type of return value
        DAGNode*                                execute(void);                                                                      //!< Execute using processed args
        DAGNode*                                execute(const std::vector<Argument>& args);                                         //!< Execute function
        std::vector<DAGNode*> const&            getProcessedArguments(void) const { return processedArguments; }                    //!< Get processed arguments
        virtual bool                            processArguments(const std::vector<Argument>& args, VectorInteger* matchScore=NULL);    //!< Process args, return a match score if pointer is not null

	protected:
                                                RbFunction(void);                                                                   //!< Basic constructor
                                                RbFunction(const RbFunction& x);                                                    //!< Copy constructor

        // Assignment operator
        RbFunction&                             operator=(const RbFunction& x);                                                     //!< Assignment operator

        // Regular utility functions
        void                                    deleteProcessedArguments(void);                                                     //!< Delete processed arguments
    	virtual DAGNode*                        executeOperation(std::vector<DAGNode*> const& args) = 0;                            //!< Execute operation

        // Member variables
        std::vector<DAGNode*>                   processedArguments;                                                                 //!< Processed arguments
        std::vector<bool>                       referenceArgument;                                                                  //!< Is processed argument reference?
        bool                                    argumentsProcessed;                                                                 //!< Are arguments processed?
};

#endif

