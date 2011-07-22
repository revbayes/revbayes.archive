/**
 * @file
 * This file contains the declaration of StochasticReferenceRule,
 * which is used to describe reference rules that require the
 * variable to be a stochastic node.
 *
 * @brief Declaration of StochasticReferenceRule
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef StochasticReferenceRule_H
#define StochasticReferenceRule_H

#include "ReferenceRule.h"

#include <string>

class DAGNode;
class RbObject;
class VectorString;

class StochasticReferenceRule : public ReferenceRule {

    public:
                                    StochasticReferenceRule(const std::string& argName, const TypeSpec& typeSp);                         //!< Constructor from name and typespec

        // Basic utility functions
        StochasticReferenceRule*    clone(void) const;                                                                      //!< Clone object
        const VectorString&         getClass(void) const;                                                                   //!< Get class vector
        void                        printValue(std::ostream& o) const;                                                      //!< Print value for user
        std::string                 richInfo(void) const;                                                                   //!< General info on object

        // StochasticReferenceRule functions
        virtual bool                isArgValid(DAGNode* var, bool& needsConversion, bool once) const;                       //!< Is var valid argument?
};

#endif

