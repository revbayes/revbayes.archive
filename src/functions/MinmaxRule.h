/**
 * @file
 * This file contains the declaration of MinmaxRule, which is
 * used to describe argument rules with min and max values.
 *
 * @brief Declaration of MinmaxRule
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * @todo More complex rules are probably better implemented
 *       in argument rule classes derived from this class,
 *       for instance MinMaxRule, OptionRule, etc. -- Fredrik
 *
 * $Id$
 */

#ifndef MinmaxRule_H
#define MinmaxRule_H

#include "ArgumentRule.h"

#include <string>

class DAGNode;
class RbObject;
class VectorString;

class MinmaxRule : public ArgumentRule {

    public:
                                    MinmaxRule(const std::string& argName, const std::string& valType, RbObject* min, RbObject* max);    //!< Constructor of rule without default value
                                    MinmaxRule(const std::string& argName, RbObject* defVal, RbObject* min, RbObject* max);             //!< Constructor of rule with default value
		                            MinmaxRule(const MinmaxRule& x);                    //!< Copy constructor
        virtual                     ~MinmaxRule(void);                                  //!< Destructor

        // Assignment operator
        MinmaxRule&                 operator=(const MinmaxRule& x);                     //!< Assignment operator

        // Basic utility functions
        MinmaxRule*                 clone(void) const { return new MinmaxRule(*this); } //!< Clone object
        virtual const VectorString& getClass(void) const;                               //!< Get class vector
        void                        printValue(std::ostream& o) const;                  //!< Print value for user
        std::string                 toString(void) const;                               //!< General info on object

        // MinmaxRule functions
        virtual bool                isArgValid(DAGNode* var) const;                     //!< Is var valid argument?

    protected:
        RbObject*                   minVal;                                             //!< Min value
        RbObject*                   maxVal;                                             //!< Max value
};

#endif


