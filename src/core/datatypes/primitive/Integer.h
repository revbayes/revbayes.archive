/**
 * @file
 * This file contains the declaration of Integer, which is
 * a RevBayes wrapper around a regular int.
 *
 * @brief Declaration of Integer
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

#ifndef Integer_H
#define Integer_H

#include "RbLanguageObject.h"

#include <ostream>
#include <string>

class Boolean;
class VectorString;

class Integer : public RbLanguageObject {

    public:
//        friend class                VectorInteger;                                                          //!< Give VectorInteger direct access to value

                                    Integer(void);                                                          //!< Default constructor
                                    Integer(const unsigned int v);                                          //!< Constructor from unsigned int
                                    Integer(const int v);                                                   //!< Constructor from int
                                    Integer(const bool v);                                                  //!< Constructor from bool

        // Overloaded operator
                                    operator int(void) const { return value; }                              //!< Type conversion to int

        // Basic utility functions
        std::string                 briefInfo(void) const;                                                  //!< Brief info about the object
        Integer*                    clone(void) const;                                                      //!< Clone object
        RbLanguageObject*           convertTo(const std::string& type) const;                               //!< Convert to type
        virtual const VectorString& getClass(void) const;                                                   //!< Get class vector
        int&                        getValueReference(void) { return value; }                               //!< Get value reference for VectorInteger
        bool                        isConvertibleTo(const std::string& type, bool once) const;              //!< Is convertible to type and dim?
        void                        printValue(std::ostream& o) const;                                      //!< Print value (for user)
        std::string                 richInfo(void) const;                                                   //!< Complete info about object

        // Getters and setters
        void                        setValue(int x) { value = x; }                                          //!< Set value
        int                         getValue(void) const { return value; }                                  //!< Get value

    protected:
        int                         value;                                                                  //!< Value member
};

        // Operators defined outside of the class
        Integer                     operator+ (const Integer& A);                                           //!< Unary operator + 
        Integer                     operator- (const Integer& A);                                           //!< Unary operator - 
        Boolean                     operator! (const Integer& A);                                           //!< Unary operator !

#if 0
        Integer                     operator+ (const Integer& A, const Integer& B);                         //!< operator + 
        Integer                     operator- (const Integer& A, const Integer& B);                         //!< operator - 
        Integer                     operator* (const Integer& A, const Integer& B);                         //!< operator *
        Integer                     operator/ (const Integer& A, const Integer& B);                         //!< operator / 
        Boolean                     operator==(const Integer& A, const Integer& B);                         //!< operator == 
        Boolean                     operator!=(const Integer& A, const Integer& B);                         //!< operator != 
        Boolean                     operator< (const Integer& A, const Integer& B);                         //!< operator < 
        Boolean                     operator<=(const Integer& A, const Integer& B);                         //!< operator <= 
        Boolean                     operator> (const Integer& A, const Integer& B);                         //!< operator > 
        Boolean                     operator>=(const Integer& A, const Integer& B);                         //!< operator >= 
        Integer                     operator+ (const Integer& A);                                           //!< unary operator + 
        Integer                     operator- (const Integer& A);                                           //!< unary operator - 
#endif

#endif

