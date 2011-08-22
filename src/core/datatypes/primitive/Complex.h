/**
 * @file
 * This file contains the declaration of Complex, which is the
 * primitive RevBayes type for complex numbers.
 *
 * @brief Declaration of Complex
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

#ifndef Complex_H
#define Complex_H

#include "RbBoolean.h"
#include "RbLanguageObject.h"

#include <complex>
#include <ostream>
#include <string>

class VectorString;



const std::string Complex_name = "Complex";

class Complex : public RbLanguageObject {

    public:
//        friend class                VectorComplex;                                                          //!< Give VecorReal direct access to value

                                    Complex(void);                                                          //!< Default constructor (0.0, 0.0)
                                    Complex(const Complex& c);                                              //!< Copy constructor
                                    Complex(const double x);                                                //!< Construct from double
                                    Complex(const int x);                                                   //!< Construct from int 
                                    Complex(const unsigned int x);                                          //!< Construct from unsigned int 
                                    Complex(const bool x);                                                  //!< Construct from bool
                                    Complex(const std::complex<double>& c);                                 //!< Construct from complex

        // Overloaded operators
                                    operator std::complex<double>(void) const { return value; }             //!< Type conversion to double for convenience
        
        // Basic utility functions
        virtual Complex*            clone(void) const;                                                      //!< Clone object
        RbLanguageObject*           convertTo(const std::string& type) const;                               //!< Convert to type and dim
        virtual const VectorString& getClass(void) const;                                                   //!< Get class vector
        std::complex<double>&       getValueReference(void) { return value; }                               //!< Get value reference
        bool                        isConvertibleTo(const std::string& type, bool once) const;              //!< Is convertible to type and dim?
        void                        printValue(std::ostream& o) const;                                      //!< Print value (for user)
        virtual std::string         richInfo(void) const;                                                   //!< Complete info about object

        // Getters and setters
        void                        setValue(std::complex<double> x) { value = x; }                         //!< Set value
        std::complex<double>        getValue(void) const { return value; }                                  //!< Get value

	protected:
        std::complex<double>        value;                                                                  //!< Value member
};

// Operators defined outside of the class
Complex                     operator+ (const Complex& A);                                           //!< Unary operator + 
Complex                     operator- (const Complex& A);                                           //!< Unary operator - 
RbBoolean                   operator! (const Complex& A);                                           //!< Unary operator !
RbBoolean                   operator==(const Complex& A, const Complex& B);                         //!< operator == 
RbBoolean                   operator!=(const Complex& A, const Complex& B);                         //!< operator != 



#if 0
Complex                     operator+ (const Complex& A, const Complex& B);                         //!< operator + 
Complex                     operator- (const Complex& A, const Complex& B);                         //!< operator - 
Complex                     operator* (const Complex& A, const Complex& B);                         //!< operator *
Complex                     operator/ (const Complex& A, const Complex& B);                         //!< operator / 
RbBoolean                     operator==(const Complex& A, const Complex& B);                         //!< operator == 
RbBoolean                     operator!=(const Complex& A, const Complex& B);                         //!< operator != 
RbBoolean                     operator< (const Complex& A, const Complex& B);                         //!< operator < 
RbBoolean                     operator<=(const Complex& A, const Complex& B);                         //!< operator <= 
RbBoolean                     operator> (const Complex& A, const Complex& B);                         //!< operator > 
RbBoolean                     operator>=(const Complex& A, const Complex& B);                         //!< operator >= 
Complex                     operator+ (const Complex& A);                                           //!< unary operator + 
Complex                     operator- (const Complex& A);                                           //!< unary operator - 
#endif

#endif

