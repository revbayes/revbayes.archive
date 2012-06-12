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

#include "RbBoolean.h"
#include "RbLanguageObject.h"

#include <ostream>
#include <string>


class Integer : public RbLanguageObject {

public:
    Integer(void);                                                                                      //!< Default constructor
    Integer(bool v);                                                                                    //!< Constructor from bool
    Integer(int v);                                                                                     //!< Constructor from int
    Integer(unsigned int v);                                                                            //!< Constructor from unsigned int
    Integer(unsigned long v);                                                                           //!< Constructor from unsigned long

    // Overloaded operator
    operator int(void) const { return value; }                                                          //!< Type conversion to int
    bool                        operator==(const Integer& x) const;                                     //!< Equals operator
    bool                        operator!=(const Integer& x) const;                                     //!< Not-Equals operator
    bool                        operator<(const Integer& x) const;                                      //!< Smaller comparison
    
    // the value type definition
    typedef int valueType;

    // Basic utility functions
    virtual Integer*            clone(void) const;                                                      //!< Clone object
    virtual RbObject*           convertTo(const TypeSpec& type) const;                                  //!< Convert to type
    static const std::string&   getClassName(void);                                                     //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                                 //!< Get class type spec
    virtual const TypeSpec&     getTypeSpec(void) const;                                                //!< Get language type of the object
    virtual RbValue<void*>      getLeanValue(void) const;                                               //!< Transform the object into a basic element pointer for fast access.
    virtual bool                isConvertibleTo(const TypeSpec& type) const;                            //!< Is convertible to type and dim?
    size_t                      memorySize() const { return sizeof( int ); }                            //!< Get the size
    virtual void                printValue(std::ostream& o) const;                                      //!< Print value (for user)
    void                        setLeanValue(const RbValue<void*> &val);                                //!< Set the value of the object from a basic (lean) element pointer.

    // Getters and setters
    virtual void                setValue(int x) { value = x; }                                          //!< Set value
    virtual int                 getValue(void) const { return value; }                                  //!< Get value

protected:
    int                         value;                                                                  //!< Value member
    
};

        // Operators defined outside of the class
        Integer                     operator+ (const Integer& A);                                           //!< Unary operator + 
        Integer                     operator- (const Integer& A);                                           //!< Unary operator - 
        RbBoolean                   operator! (const Integer& A);                                           //!< Unary operator !

#if 0
        Integer                     operator+ (const Integer& A, const Integer& B);                         //!< operator + 
        Integer                     operator- (const Integer& A, const Integer& B);                         //!< operator - 
        Integer                     operator* (const Integer& A, const Integer& B);                         //!< operator *
        Integer                     operator/ (const Integer& A, const Integer& B);                         //!< operator / 
        RbBoolean                   operator==(const Integer& A, const Integer& B);                         //!< operator == 
        RbBoolean                   operator!=(const Integer& A, const Integer& B);                         //!< operator != 
        RbBoolean                   operator< (const Integer& A, const Integer& B);                         //!< operator < 
        RbBoolean                   operator<=(const Integer& A, const Integer& B);                         //!< operator <= 
        RbBoolean                   operator> (const Integer& A, const Integer& B);                         //!< operator > 
        RbBoolean                   operator>=(const Integer& A, const Integer& B);                         //!< operator >= 
        Integer                     operator+ (const Integer& A);                                           //!< unary operator + 
        Integer                     operator- (const Integer& A);                                           //!< unary operator - 
#endif

#endif

