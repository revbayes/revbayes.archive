/**
 * @file
 * This file contains the declaration of OptionRule, which is
 * used to describe argument rules corresponding to the
 * selection of one of several RlString options.
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

#include "ArgumentRule.h"
#include "Vector.h"
#include "RlString.h"

#include <string>

namespace RevLanguage {

class OptionRule : public ArgumentRule {

    public:
                                    OptionRule(const std::string& argName, const Vector<RlString> &optVals);                            //!< Constructor of rule without default value
                                    OptionRule(const std::string& argName, RlString *defVal, const Vector<RlString>& optVals);          //!< Constructor of rule with default value

        // Basic utility functions
        OptionRule*                 clone(void) const { return new OptionRule(*this); }                                                 //!< Clone object
        std::string                 debugInfo(void) const;                                                                              //!< General info on object
        void                        printValue(std::ostream& o) const;                                                                  //!< Print value for user

    protected:
        bool                        areOptionsUnique(const Vector<RlString>& optVals) const;                                            //!< Test if options are unique

        Vector<RlString>            options;                                                                                            //!< Permissible values
    
};

}

#endif

