/**
 * @file
 * This file contains the declaration of RlString, which is
 * a RevBayes wrapper around a regular RlString.
 *
 * @brief Declaration of RlString
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RlString_H
#define RlString_H

#include "RlBoolean.h"
#include "RlModelVariableWrapper.h"
#include "TypedDagNode.h"

#include <fstream>
#include <string>

namespace RevLanguage {

    class RlString : public RlModelVariableWrapper<std::string> {

        public:
            RlString(void);                                                                         //!< Default: empty RlString
            RlString(int i);                                                                        //!< Constructor from int
            RlString(double i);                                                                     //!< Constructor from double
            RlString(const std::string& v);                                                         //!< Constructor from RlString

            // Basic utility functions
            RlString*                       clone(void) const;                                      //!< Copy
            static const std::string&       getClassName(void);                                     //!< Get class name
            static const TypeSpec&          getClassTypeSpec(void);                                 //!< Get class type spec
            const TypeSpec&                 getTypeSpec(void) const;                                //!< Get language type of the object
            void                            printValue(std::ostream& o) const;                      //!< Print value (for user)
    
    };
    
}

#endif

