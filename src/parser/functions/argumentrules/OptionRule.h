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

#include "ConstArgumentRule.h"
#include "RbVector.h"

#include <string>

class DAGNode;
class RbObject;
class RbString;


class OptionRule : public ConstArgumentRule {

    public:
                                    OptionRule(const std::string& argName, RbVector optVals);                           //!< Constructor of rule without default value
                                    OptionRule(const std::string& argName, RbString* defVal, RbVector optVals);         //!< Constructor of rule with default value

        // Basic utility functions
        OptionRule*                 clone(void) const { return new OptionRule(*this); }                                 //!< Clone object
        std::string                 debugInfo(void) const;                                                              //!< General info on object
        static const std::string&   getClassName(void);                                                                 //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                             //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                                            //!< Get language type of the object
        void                        printValue(std::ostream& o) const;                                                  //!< Print value for user

    protected:
        bool                        areOptionsUnique(const RbVector& optVals) const;                                    //!< Test if options are unique

        RbVector                    options;                                                                            //!< Permissible values
    
};

#endif

