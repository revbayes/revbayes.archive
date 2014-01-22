/*!
 * @file
 * This file contains global constants of the RevLanguage.
 *
 * @brief Names of variable types
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-02-25 10:17:07 +0100 (Sat, 25 Feb 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-09-09, version 1.0
 *
 * $Id: RbUtil.h 1247 2012-02-25 09:17:07Z hoehna $
 */

#ifndef RlUtil_H
#define RlUtil_H

#include "RbLanguageObject.h"

#include <string>

namespace RevLanguage {
    
    namespace RlUtils {

        // Empty return type
        const std::string Void                                   = "void";

        class RlTypeConverter {
            public:
            static RbLanguageObject*                toReal(double x);
            static RbLanguageObject*                toString(const std::string &x);
        };
    }

}

#endif
