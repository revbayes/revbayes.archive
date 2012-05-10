/**
 * @file
 * This file contains the declaration of DistributionFunction, which
 * is used for functions related to a statistical distribution.
 *
 * @brief Declaration of DistributionFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef DistributionFunction_H
#define DistributionFunction_H

#include "Real.h"
#include "RbFunction.h"

#include <map>
#include <set>
#include <string>
#include <vector>

class ArgumentRule;
class DAGNode;
class ParserDistribution;


class DistributionFunction :  public RbFunction {

    public:
        enum FuncType { DENSITY, RVALUE, PROB, QUANTILE };                                                  //!< Enum specifying function type

                                    DistributionFunction(ParserDistribution* dist, FuncType funcType);            //!< Constructor
                                    DistributionFunction(const DistributionFunction& x);                    //!< Copy constructor
        virtual                    ~DistributionFunction(void);                                             //!< Destructor

        // Assignment operator
        DistributionFunction&       operator=(const DistributionFunction& x);                               //!< Assignment operator

        // Basic utility functions
        DistributionFunction*       clone(void) const;                                                      //!< Clone object
        static const std::string&   getClassName(void);                                                     //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                                //!< Get language type of the object

        // DistributionFunction functions
        const ArgumentRules&        getArgumentRules(void) const;                                           //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                              //!< Get type spec of return value
        void                        processArguments(const std::vector<Argument>& passedArgs);              //!< Process args, set member variables of distribution

	protected:
        const RbLanguageObject&     executeFunction(const std::vector<const RbObject*>& args);              //!< Execute function

        ArgumentRules*              argumentRules;                                                          //!< Argument rules
        TypeSpec                    returnType;                                                             //!< Return type
        ParserDistribution*         distribution;                                                           //!< The distribution
        FuncType                    functionType;                                                           //!< Function type
    
    private:
    
        // memberfunction return values;
        RealPos                     cd;
        Real                        density;
        Real                        quant;
};

#endif

