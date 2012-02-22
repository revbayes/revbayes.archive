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
#include "RealPos.h"

#include <ostream>
#include <string>


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
                                    Natural(bool x);                                                        //!< Constructor from bool
                                    Natural(int x);                                                         //!< Constructor from int
                                    Natural(unsigned int x);                                                //!< Constructor from int
                                    Natural(unsigned long x);                                               //!< Constructor from size_t
                                    Natural(const Integer& x);                                              //!< Constructor from Integer
                                    Natural(const RbBoolean& x);                                            //!< Constructor from Bool

        // Overloaded operators
                                    operator int(void) const { return value; }                              //!< Type conversion to int
//                                    operator unsigned int(void) const { return value; }                     //!< Type conversion to unsigned int

        // Basic utility functions
        Natural*                    clone(void) const;                                                      //!< Clone object
        RbObject*                   convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        static const std::string&   getClassName(void);                                                     //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                                //!< Get language type of the object
        bool                        isConvertibleTo(const TypeSpec& type) const;                            //!< Is convertible to type?
        void                        printValue(std::ostream& o) const;                                      //!< Print value (for user)

        // Getters and setters
        unsigned int                getUnsignedValue(void) const { return value; }                          //!< Get value (unsigned int)
        int                         getValue(void) const { return value; }                                  //!< Get value (int)
        void                        setValue(int x);                                                        //!< Set value from int (after checking)
        void                        setValue(unsigned int x);                                               //!< Set value from unsigned int
        void                        setValue(size_t x);                                                     //!< Set value from unsigned int
    

};

// Operators defined outside of the class
//        Natural                     operator+ (const Natural& A);                                           //!< Unary operator + 
//        Natural                     operator+ (const Natural& A, const Natural& B);                         //!< operator + 
//        Integer                     operator- (const Integer& A, const Natural& B);                         //!< operator - 
//        Natural                     operator* (const Natural& A, const Natural& B);                         //!< operator *
//        RealPos                     operator/ (const Natural& A, const Natural& B);                         //!< operator / 



#endif

