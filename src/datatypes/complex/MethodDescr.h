/**
 * @file
 * This file contains the declaration of MethodDescr, which is
 * used to hold the description of a member function. The
 * description consists of a return type and a set of argument
 * rules for the function. The class is used in MethodTable.
 *
 * @brief Declaration of MethodDescr
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 * @extends RbInternal
 *
 * $Id: MethodDescr.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef MethodDescr_H
#define MethodDescr_H

#include "RbInternal.h"

#include <ostream>
#include <string>

class ArgumentRule;

class MethodDescr : RbInternal {

    public:
                                MethodDescr(const std::string& retType, const ArgumentRule** argRules);         //!< Constructor
    
        MethodDescr*            clone() const { return new MethodDescr(*this); }    //!< Clone object
        const ArgumentRule**    getArgRules() const { return argRules; }            //!< Get arg rules
        void                    printValue(std::ostream& o) const;                  //!< Print function description
        std::string             toString(void) const;                               //!< Print complete info

    private:
        const std::string&      returnType;                                         //!< The return type
        const ArgumentRule**    argRules;                                           //!< The argument rules
};

#endif


