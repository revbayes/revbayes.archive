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
 * $Id:$
 */

#ifndef RbAbstract_H
#define RbAbstract_H

#include "RbObject.h"
#include "VectorString.h"

#include <ostream>
#include <string>


class RbAbstract : public RbObject {

    public:
                                RbAbstract(const VectorString classVec,
                                           size_t             numDims = 0,
                                           const std::string& elemType = "");                           //!< Constructor

        // Basic utility functions
        RbAbstract*             clone(void) const;                                                      //!< Clone object
        RbObject*               convertTo(const std::string& type, size_t dim) const;                   //!< Convert to type and dim
        const VectorString&     getClass(void) const { return classVector; }                            //!< Get class vector
        size_t                  getDim() const { return dim; }                                          //!< Get dim of class
        const TypeSpec          getTypeSpec() const;                                                    //!< Get type spec
        bool                    isConvertibleTo(const std::string& type, size_t dim, bool once) const;  //!< Is convertible to type and dim?
        void                    printValue(std::ostream& o) const;                                      //!< Print value (for user)
        std::string             richInfo(void) const;                                                   //!< Complete info about object

    protected:
        VectorString            classVector;                                                            //!< Class vector describing type
        size_t                  dim;                                                                    //!< Number of dimensions
        const std::string       elementType;                                                            //!< Element type, if any
};

#endif
