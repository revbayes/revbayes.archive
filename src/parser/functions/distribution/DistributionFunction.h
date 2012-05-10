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

#include "RbFunction.h"

#include <string>
#include <vector>

class ParserDistribution;


class DistributionFunction :  public RbFunction {

    public:

                                    DistributionFunction(const RbPtr<ParserDistribution> &dist);            //!< Constructor
                                    DistributionFunction(const DistributionFunction& x);                    //!< Copy constructor

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
        RbPtr<RbLanguageObject>     executeFunction(const std::vector<const RbObject*>& args);              //!< Execute function

        RbPtr<ArgumentRules>        argumentRules;                                                          //!< Argument rules
        TypeSpec                    returnType;                                                             //!< Return type
        RbPtr<ParserDistribution>   distribution;                                                           //!< The distribution
    
};

#endif

