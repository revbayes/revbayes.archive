/**
 * @file
 * This file contains the declaration of ArgumentRule, which is
 * used to describe rules for arguments passed to functions.
 *
 * @brief Declaration of ArgumentRule
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes core development team
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

#ifndef ArgumentRule_H
#define ArgumentRule_H

#include "RbInternal.h"

#include "StringVector.h"

#include <string>


class ArgumentRule : public RbInternal {

    public:
                                ArgumentRule(const std::string& argName, const std::string& type, int dim=0);  //!< Constructor of rule without default value
                                ArgumentRule(const std::string& argName, RbObject* defVal);                    //!< Constructor of rule with default value
                                ArgumentRule(const std::string& argName, const std::string& t, RbObject* dv);  //!< Constructor of rule with required type and default value
                                ArgumentRule(const std::string& argName, const std::string& t, RbObject* dv, RbObject* mnv, RbObject* mxv);    //!< Constructor of rule with required type, default value, min and max
		                        ArgumentRule(const ArgumentRule& a);                    //!< Copy constructor
        virtual                 ~ArgumentRule(void);                                    //!< Destructor

        // Assignment operator
        ArgumentRule&           operator=(const ArgumentRule& x);                       //!< Assignment operator

        // Basic utility functions
        ArgumentRule*           clone(void) const { return new ArgumentRule(*this); }   //!< Clone object
        void                    printValue(std::ostream& o) const;                      //!< Print value (for user)
        std::string             toString(void) const;                                   //!< General info on object

        // Regular functions
        const RbObject&         getDefaultValue(void) const { return *defaultValue; }
        std::string             getLabel(void) const { return label; }              //!< Get label of argument
        const RbObject&         getMinValue(void) const { return *minValue; }       //!< Get min
        const RbObject&         getMaxValue(void) const { return *maxValue; }       //!< Get max
        std::string             getType(void) const { return requiredType; }        //!< Get type of argument
        bool                    isArgValid(const RbObject* val) const;              //!< Is val a valid argument?

    protected:
        std::string             label;                                              //!< Label of argument
        std::string             requiredType;                                       //!< Type of argument
        int                     numDim;                                             //!< Dimensions of argument
        RbObject*               defaultValue;                                       //!< Default value
        RbObject*               minValue;                                           //!< Min value
        RbObject*               maxValue;                                           //!< Max value
};

#endif

