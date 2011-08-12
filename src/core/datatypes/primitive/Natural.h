/**
 * @file
 * This file contains the declaration of Natural, which is
 * the primitive RevBayes type for natural numbers (including 0).
 *
 * @brief Declaration of Natural
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Natural_H
#define Natural_H

#include "Integer.h"

#include <ostream>
#include <string>

class Boolean;


/**
 * Primitive type for Natural numbers (including 0).
 *
 * Note that we derive this from Integer. To make
 * sure inheritance is safe, we restrict the range
 * of natural numbers from 0 to to INT_MAX
 */
class Natural : public Integer {

    public:
                            Natural(void);                                                          //!< Default constructor (value is 0)
                            Natural(unsigned int x);                                                //!< Constructor from unsigned int
                            Natural(int x);                                                         //!< Constructor from int
                            Natural(bool x);                                                        //!< Constructor from bool
                            Natural(const Integer& x);                                              //!< Constructor from Integer
                            Natural(const Boolean& x);                                              //!< Constructor from Bool

        // Overloaded operators
                            operator int(void) const { return value; }                              //!< Type conversion to int
                            operator unsigned int(void) const { return value; }                     //!< Type conversion to unsigned int

        // Basic utility functions
        std::string         briefInfo(void) const;                                                  //!< Brief info about the object
        Natural*            clone(void) const;                                                      //!< Clone object
        RbLanguageObject*   convertTo(const std::string& type) const;                               //!< Convert to type and dim
        const VectorString& getClass(void) const;                                                   //!< Get class vector
        bool                isConvertibleTo(const std::string& type, bool once) const;              //!< Is convertible to type and dim?
        void                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        std::string         richInfo(void) const;                                                   //!< Complete info about object

        // Getters and setters
        unsigned int        getUnsignedValue(void) const { return value; }                          //!< Get value (unsigned int)
        int                 getValue(void) const { return value; }                                  //!< Get value (int)
        void                setValue(int x);                                                        //!< Set value from int (after checking)
        void                setValue(unsigned int x);                                               //!< Set value from unsigned int
};

#endif

