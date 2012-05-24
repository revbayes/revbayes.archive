/**
 * @file
 * This file contains the declaration of Real, which is the
 * primitive RevBayes type for real numbers.
 *
 * @brief Declaration of Real
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

#ifndef Real_H
#define Real_H

#include "RbLanguageObject.h"
#include <ostream>
#include <string>

class RbBoolean;

/**
 * Real is the class used to hold a real value. Internally, the real is represented by
 * a double.
 *
 * @note Some functions are virtual because RealPos is derived from Real
 */

class Real : public RbLanguageObject {

    public:

                                    Real(void);                                                             //!< Default constructor (0.0)
                                    Real(const double v);                                                   //!< Construct from double
                                    Real(const int v);                                                      //!< Construct from int 
                                    Real(const unsigned int v);                                             //!< Construct from unsigned int 
                                    Real(const bool v);                                                     //!< Construct from bool
                                    Real(const Real& x);                                                    //!< Copy constructor
    
        // the value type definition
        typedef double valueType;
    
        // Overloaded operators
                                    operator double(void) const { return value; }                           //!< Type conversion to double for convenience
        bool                        operator==(const Real& x) const;                                        //!< Equals operator
        bool                        operator!=(const Real& x) const;                                        //!< Not-Equals operator
        Real&                       operator+=(const Real& A);                                              //!< operator += 
        Real&                       operator+=(double A);                                                   //!< operator += 
        Real&                       operator-=(const Real& A);                                              //!< operator -= 
        Real&                       operator-=(double A);                                                   //!< operator -= 
        Real&                       operator*=(const Real& A);                                              //!< operator *= 
        Real&                       operator*=(double A);                                                   //!< operator *= 
        Real&                       operator/=(const Real& A);                                              //!< operator /= 
        Real&                       operator/=(double A);                                                   //!< operator /= 
        bool                        operator<(const Real& x) const;                                         //!< operator <
        bool                        operator<(const double& x) const;                                         //!< operator <
        
        // Basic utility functions
        virtual Real*               clone(void) const;                                                      //!< Clone object
        virtual RbObject*           convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        static const std::string&   getClassName(void);                                                     //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                 //!< Get class type spec
        virtual const TypeSpec&     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void*               getLeanValue(std::vector<size_t> &length) const;                        //!< Transform the object into a basic element pointer for fast access.
        virtual double&             getValueReference(void) { return value; }                               //!< Get value reference
        size_t                      memorySize() const { return sizeof( double ); }                         //!< Get the size
        virtual bool                isConvertibleTo(const TypeSpec& type) const;                            //!< Is convertible to type?
        virtual void                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        void                        setLeanValue(const RbValue<void*> &val);                                //!< Set the value of the object from a basic (lean) element pointer.

        // Getters and setters
        virtual void                setValue(double x) { value = x; }                                       //!< Set value
        virtual double              getValue(void) const { return value; }                                  //!< Get value

	protected:
        double                      value;                                                                  //!< Value member
    
};

        // Operators defined outside of the class
        Real                        operator+ (const Real& A);                                              //!< Unary operator + 
        Real                        operator- (const Real& A);                                              //!< Unary operator - 
        RbBoolean                     operator! (const Real& A);                                              //!< Unary operator !

#endif

