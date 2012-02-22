/**
 * @file
 * This file contains the declaration of RbBoolean, which is
 * a RevBayes wrapper around a regular bool.
 *
 * @brief Declaration of RbBoolean
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */

#ifndef Boolean_H
#define Boolean_H

#include "RbLanguageObject.h"

#include <ostream>
#include <string>


class RbBoolean : public RbLanguageObject {

    public:
                                        RbBoolean(void);                                                        //!< Default constructor
                                        RbBoolean(const bool v);                                                //!< Construct from bool

        // Operators
                                        operator bool(void) const { return value; }                             //!< Type conversion to bool

        // Basic utility functions
        RbBoolean*                      clone(void) const;                                                      //!< Clone object
        RbObject*                       convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        static const std::string&       getClassName(void);                                                     //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object
        bool&                           getValueReference(void) { return value; }                               //!< Get value reference
        bool                            isConvertibleTo(const TypeSpec& type) const;                            //!< Is convertible to type?
        void                            printValue(std::ostream& o) const;                                      //!< Print value (for user)

        // Getters and setters
        void                            setValue(bool x) { value = x; }                                         //!< Set value
        bool                            getValue(void) const { return value; }                                  //!< Get value

    private:

        bool                    value;                                                                  //!< Value member

};

#endif

