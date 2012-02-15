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


/**
 * Primitive type for Natural numbers (including 0).
 *
 * Note that we derive this from Integer. To make
 * sure inheritance is safe, we restrict the range
 * of natural numbers from 0 to to INT_MAX
 */
const std::string Natural_name = "Natural";

class Natural : public Integer {

    public:
                            Natural(void);                                                          //!< Default constructor (value is 0)
                            Natural(bool x);                                                        //!< Constructor from bool
                            Natural(int x);                                                         //!< Constructor from int
                            Natural(unsigned int x);                                                //!< Constructor from int
                            Natural(unsigned long x);                                               //!< Constructor from size_t
                            Natural(const Integer& x);                                              //!< Constructor from Integer
                            Natural(const RbBoolean& x);                                            //!< Constructor from Bool

        // Overloaded operators
                            operator int(void) const { return value; }                              //!< Type conversion to int
                            operator unsigned int(void) const { return value; }                     //!< Type conversion to unsigned int

        // Basic utility functions
        std::string         briefInfo(void) const;                                                  //!< Brief info about the object
        Natural*            clone(void) const;                                                      //!< Clone object
        RbObject*           convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        const VectorString& getClass(void) const;                                                   //!< Get class vector
        const TypeSpec&     getTypeSpec(void) const;                                                //!< Get language type of the object
        bool                isConvertibleTo(const TypeSpec& type) const;                            //!< Is convertible to type?
        void                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        std::string         richInfo(void) const;                                                   //!< Complete info about object

        // Getters and setters
        unsigned int        getUnsignedValue(void) const { return value; }                          //!< Get value (unsigned int)
        int                 getValue(void) const { return value; }                                  //!< Get value (int)
        void                setValue(int x);                                                        //!< Set value from int (after checking)
        void                setValue(unsigned int x);                                               //!< Set value from unsigned int
        void                setValue(size_t x);                                                     //!< Set value from unsigned int
    
    
    private:
        static const TypeSpec       typeSpec;
};



#endif

