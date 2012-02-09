/**
 * @file
 * This file contains the declaration of RbAbstract, which is
 * used to describe abstract types in the type table.
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
#include "VectorString.h"

#include <ostream>
#include <string>


class RbAbstract : public RbLanguageObject {

public:
                            RbAbstract(const VectorString classVec,
                                       TypeSpec* elemType = NULL);                           //!< Constructor

    // Basic utility functions
    RbAbstract*             clone(void) const;                                                      //!< Clone object
    const VectorString&     getClass(void) const { return classVector; }                            //!< Get class vector
    const TypeSpec&         getTypeSpec() const;                                                    //!< Get type spec
    void                    printValue(std::ostream& o) const;                                      //!< Print value (for user)

protected:
    VectorString            classVector;                                                            //!< Class vector describing type
    TypeSpec*               elementType;                                                            //!< Element type, if any
    TypeSpec                typeSpec;
};

#endif
