/**
 * @file
 * This file contains the declaration of DistributionFunction, which
 * is used for functions related to a statistical distribution.
 *
 * @brief Declaration of DistributionFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef DistributionFunction_H
#define DistributionFunction_H

#include "RbDouble.h"
#include "RbFunction.h"

#include <map>
#include <set>
#include <string>
#include <vector>

class ArgumentRule;
class DAGNode;
class Distribution;
class IntVector;
class StringVector;

class DistributionFunction :  public RbFunction {

    public:
        enum FuncType { DENSITY, RVALUE, PROB, QUANTILE };                  //!< Enum specifying function type

                                    DistributionFunction(Distribution* dist, FuncType funcType);//!< Constructor
                                    DistributionFunction(const DistributionFunction& x);        //!< Copy constructor
                                    ~DistributionFunction(void);                                //!< Destructor

        // Assignment operator
        DistributionFunction&       operator=(const DistributionFunction& x);   //!< Assignment operator

        // Basic utility functions
        DistributionFunction*       clone(void) const;                          //!< Clone object
    	bool                        equals(const RbObject* obj) const;          //!< Equals comparison
    	const StringVector&         getClass(void) const;                       //!< Get class vector

        // DistributionFunction functions
        const ArgumentRules&        getArgumentRules(void) const;           //!< Get argument rules
        const std::string&          getReturnType(void) const;              //!< Get type of return value
        bool                        processArguments(const std::vector<Argument>& args, IntVector* matchScore=NULL);    //!< Process args, set member variables of distribution

	protected:
        RbObject*                   executeOperation(const std::vector<DAGNode*>& args);    //!< Execute operation

        ArgumentRules               argumentRules;                          //!< Argument rules
        std::string                 returnType;                             //!< Return type
        Distribution*               distribution;                           //!< The distribution
        FuncType                    functionType;                           //!< Function type
};

#endif

