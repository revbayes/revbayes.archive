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

class Boolean;
class VectorString;

/**
 * Real is the class used to hold a real value. Internally, the real is represented by
 * a double.
 *
 * @note Some functions are virtual because RealPos is derived from double
 */
class Real : public RbLanguageObject {

    public:
//        friend class                VectorReal;                                                             //!< Give VecorReal direct access to value

                                    Real(void);                                                             //!< Default constructor (0.0)
                                    Real(const double v);                                                   //!< Construct from double
                                    Real(const int v);                                                      //!< Construct from int 
                                    Real(const unsigned int v);                                             //!< Construct from unsigned int 
                                    Real(const bool v);                                                     //!< Construct from bool

        // Overloaded operators
                                    operator double(void) const { return value; }                           //!< Type conversion to double for convenience
        
        // Basic utility functions
        virtual std::string         briefInfo(void) const;                                                  //!< Brief info about the object
        virtual Real*               clone(void) const;                                                      //!< Clone object
        RbLanguageObject*           convertTo(const std::string& type) const;                               //!< Convert to type and dim
        virtual const VectorString& getClass(void) const;                                                   //!< Get class vector
        double&                     getValueReference(void) { return value; }                               //!< Get value reference
        bool                        isConvertibleTo(const std::string& type, bool once) const;              //!< Is convertible to type?
        void                        printValue(std::ostream& o) const;                                      //!< Print value (for user)
        virtual std::string         richInfo(void) const;                                                   //!< Complete info about object

        // Getters and setters
        virtual void                setValue(double x) { value = x; }                                       //!< Set value
        double                      getValue(void) const { return value; }                                  //!< Get value

	protected:
        double                      value;                                                                  //!< Value member
};

        // Operators defined outside of the class
        Real                        operator+ (const Real& A);                                              //!< Unary operator + 
        Real                        operator- (const Real& A);                                              //!< Unary operator - 
        Boolean                     operator! (const Real& A);                                              //!< Unary operator !

#endif

