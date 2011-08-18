/**
 * @file
 * This file contains the declaration of OptionRule, which is
 * used to describe argument rules corresponding to the
 * selection of one of several string options.
 *
 * @brief Declaration of OptionRule
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef OptionRule_H
#define OptionRule_H

#include "ValueRule.h"
#include "VectorString.h"

#include <string>

class DAGNode;
class RbObject;
class RbString;

const std::string OptionRule_name = "OptionRule";

class OptionRule : public ValueRule {

    public:
                                    OptionRule(const std::string& argName, VectorString optVals);                       //!< Constructor of rule without default value
                                    OptionRule(const std::string& argName, RbString* defVal, VectorString optVals);     //!< Constructor of rule with default value

        // Basic utility functions
        OptionRule*                 clone(void) const { return new OptionRule(*this); }                                 //!< Clone object
        virtual const VectorString& getClass(void) const;                                                               //!< Get class vector
        void                        printValue(std::ostream& o) const;                                                  //!< Print value for user
        std::string                 richInfo(void) const;                                                               //!< General info on object

        // OptionRule functions
        virtual bool                isArgValid(DAGNode* var, bool& needsConversion, bool once) const;             //!< Is var valid argument?

    protected:
        bool                        areOptionsUnique(const VectorString& optVals) const;                                //!< Test if options are unique

        VectorString                options;                                                                            //!< Permissible values
};

#endif

