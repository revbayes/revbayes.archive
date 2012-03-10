/**
 * @file
 * This file contains the declaration of RbAbstract, which is
 * used to describe abstract types in the type table.
 * We need the type table to declare argument types and return type of user defined functions.
 * Usually we would just add a dummy element into the type table
 *
 *
 * @brief Declaration of RbAbstract
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RbAbstract_H
#define RbAbstract_H

#include "RbLanguageObject.h"

#include <ostream>
#include <string>


class RbAbstract : public RbLanguageObject {

public:
                            RbAbstract(const TypeSpec& classType);                                      //!< Constructor

    // Basic utility functions
    RbAbstract*                 clone(void) const;                                                      //!< Clone object
//    static const std::string&   getClassName(void);                                                     //!< Get class name
//    static const TypeSpec&      getClassTypeSpec(void);                                                 //!< Get class type spec
    const TypeSpec&             getTypeSpec() const;                                                    //!< Get type spec
    void                        printValue(std::ostream& o) const;                                      //!< Print value (for user)

protected:                                                            //!< Element type, if any
    TypeSpec                    typeSpec;
};

#endif
